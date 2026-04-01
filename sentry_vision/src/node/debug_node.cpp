// debug_node.cpp —— 全向感知可视化调试节点
//
// 功能：
//   1) 订阅 3 路相机图像，叠加检测框显示
//   2) 订阅装甲板检测结果 /armor_detections
//   3) 订阅融合目标 /enemy_targets
//   4) 绘制鸟瞰图 (BEV) 展示融合后的全向感知态势
//   5) 绘制参数面板，实时显示当前参数 + 调节提示
//   6) 发布标注图像 topic（可用 rqt_image_view 远程查看）
//   7) 统计每路相机 FPS、检测延迟、检测数

#include "sentry_vision/debug_node.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace sentry_vision
{

// 颜色常量
static const cv::Scalar COLOR_RED    = {0, 0, 255};
static const cv::Scalar COLOR_BLUE   = {255, 120, 0};
static const cv::Scalar COLOR_GREEN  = {0, 220, 0};
static const cv::Scalar COLOR_YELLOW = {0, 255, 255};
static const cv::Scalar COLOR_WHITE  = {255, 255, 255};
static const cv::Scalar COLOR_GRAY   = {160, 160, 160};
static const cv::Scalar COLOR_BG     = {30, 30, 30};

// ============================================================
// 构造
// ============================================================
DebugNode::DebugNode()
: Node("sentry_debug_node")
{
    // ---------- 参数 ----------
    declare_parameter<bool>("show_windows", true);
    declare_parameter<double>("bev_range", 10.0);
    declare_parameter<int>("bev_size", 600);
    declare_parameter<int>("image_width", 1440);
    declare_parameter<int>("image_height", 1080);
    declare_parameter<std::vector<std::string>>("camera_topics", {
        "/mvsua_cam/image_raw2",
        "/mvsua_cam/image_raw3",
        "/mvsua_cam/image_raw4"
    });

    show_windows_ = get_parameter("show_windows").as_bool();
    bev_range_ = get_parameter("bev_range").as_double();
    bev_size_ = get_parameter("bev_size").as_int();
    source_width_ = get_parameter("image_width").as_int();
    source_height_ = get_parameter("image_height").as_int();
    topic_names_ = get_parameter("camera_topics").as_string_array();

    // ---------- 订阅相机图像（QoS 匹配相机发布端的 SensorDataQoS）----------
    auto img_qos = rclcpp::SensorDataQoS().keep_last(1);
    for (size_t i = 0; i < topic_names_.size(); ++i) {
        int cam_id = static_cast<int>(i);
        auto sub = create_subscription<sensor_msgs::msg::Image>(
            topic_names_[i], img_qos,
            [this, cam_id](const sensor_msgs::msg::Image::SharedPtr msg) {
                imageCallback(msg, cam_id);
            });
        img_subs_.push_back(sub);

        // 每路相机一个标注图 publisher
        std::string annotated_topic = "/debug/cam" + std::to_string(i) + "/annotated";
        annotated_pubs_.push_back(
            create_publisher<sensor_msgs::msg::Image>(annotated_topic, 5));

        RCLCPP_INFO(get_logger(), "Debug subscribing: %s => %s",
                    topic_names_[i].c_str(), annotated_topic.c_str());
    }

    // ---------- 订阅检测 / 目标 ----------
    det_sub_ = create_subscription<sentry_interfaces::msg::ArmorDetectionArray>(
        "/armor_detections", 10,
        std::bind(&DebugNode::detectionCallback, this, std::placeholders::_1));

    target_sub_ = create_subscription<sentry_interfaces::msg::EnemyTargetArray>(
        "/enemy_targets", 10,
        std::bind(&DebugNode::targetCallback, this, std::placeholders::_1));

    // ---------- 发布 BEV 和参数面板 ----------
    bev_pub_ = create_publisher<sensor_msgs::msg::Image>("/debug/bev", 5);
    panel_pub_ = create_publisher<sensor_msgs::msg::Image>("/debug/param_panel", 5);

    // ---------- 定时刷新 (30 Hz) ----------
    timer_ = create_wall_timer(
        std::chrono::milliseconds(33),
        std::bind(&DebugNode::timerCallback, this));

    RCLCPP_INFO(get_logger(), "=== SentryDebugNode started ===");
    RCLCPP_INFO(get_logger(), "  Annotated images: /debug/cam{0,1,2}/annotated");
    RCLCPP_INFO(get_logger(), "  Bird-eye view:    /debug/bev");
    RCLCPP_INFO(get_logger(), "  Param panel:      /debug/param_panel");
    if (show_windows_) {
        RCLCPP_INFO(get_logger(), "  OpenCV windows:   ENABLED (press 'q' to quit)");
    }
}

// ============================================================
// 回调：相机图像
// ============================================================
void DebugNode::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int cam_id)
{
    cv::Mat img;
    try {
        img = cv_bridge::toCvCopy(msg, "bgr8")->image;
    } catch (const cv_bridge::Exception & e) {
        RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 5000,
            "cv_bridge error cam%d: %s", cam_id, e.what());
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    latest_images_[cam_id] = img;

    // 更新 FPS 统计
    auto & stats = cam_stats_[cam_id];
    stats.frame_count++;
    auto now_time = this->now();
    stats.last_image_time = now_time;
    stats.fps_window.push_back(now_time);
    while (stats.fps_window.size() > 30) {
        stats.fps_window.pop_front();
    }
}

// ============================================================
// 回调：装甲板检测
// ============================================================
void DebugNode::detectionCallback(
    const sentry_interfaces::msg::ArmorDetectionArray::SharedPtr msg)
{
    std::lock_guard<std::mutex> lock(mutex_);
    int cam_id = static_cast<int>(msg->camera_id);
    latest_dets_[cam_id] = msg->detections;
    cam_stats_[cam_id].det_count = static_cast<int>(msg->detections.size());
}

// ============================================================
// 回调：融合目标
// ============================================================
void DebugNode::targetCallback(
    const sentry_interfaces::msg::EnemyTargetArray::SharedPtr msg)
{
    std::lock_guard<std::mutex> lock(mutex_);
    latest_targets_ = msg->targets;
    total_fused_targets_ = static_cast<int>(msg->targets.size());
}

// ============================================================
// 定时刷新可视化
// ============================================================
void DebugNode::timerCallback()
{
    std::lock_guard<std::mutex> lock(mutex_);

    // 1) 每路相机的标注图
    for (size_t i = 0; i < topic_names_.size(); ++i) {
        int cam_id = static_cast<int>(i);
        if (latest_images_.find(cam_id) == latest_images_.end()) continue;

        cv::Mat vis = latest_images_[cam_id].clone();
        drawDetections(vis, cam_id);
        drawStats(vis, cam_id);

        // 发布
        auto ros_img = cv_bridge::CvImage(
            std_msgs::msg::Header(), "bgr8", vis).toImageMsg();
        ros_img->header.stamp = now();
        annotated_pubs_[i]->publish(*ros_img);

        // 本地窗口
        if (show_windows_) {
            std::string win = "cam" + std::to_string(cam_id);
            cv::imshow(win, vis);
        }
    }

    // 2) 鸟瞰图
    drawBirdEyeView();

    // 3) 参数面板
    drawParamPanel();

    if (show_windows_) {
        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            RCLCPP_INFO(get_logger(), "Quit requested");
            rclcpp::shutdown();
        }
    }
}

// ============================================================
// 在图像上画检测框
// ============================================================
void DebugNode::drawDetections(cv::Mat & img, int cam_id)
{
    auto it = latest_dets_.find(cam_id);
    if (it == latest_dets_.end()) return;

    // 检测坐标基于原图 (source_width_ x source_height_)，
    // 实际图像可能已被 resize，需要缩放映射
    float sx = static_cast<float>(img.cols) / static_cast<float>(source_width_);
    float sy = static_cast<float>(img.rows) / static_cast<float>(source_height_);

    for (const auto & det : it->second) {
        cv::Scalar color = colorForId(det.target_color);
        float cx = det.x * sx;
        float cy = det.y * sy;
        float w  = det.width * sx;
        float h  = det.height * sy;
        float half_w = w / 2.0f;
        float half_h = h / 2.0f;
        cv::Rect2f box(cx - half_w, cy - half_h, w, h);

        // 检测框
        cv::rectangle(img, box, color, 2);

        // 标签：编号 + 置信度
        std::ostringstream label;
        label << numberLabel(det.target_number) << " "
              << std::fixed << std::setprecision(2) << det.confidence;

        int baseline = 0;
        cv::Size text_sz = cv::getTextSize(label.str(), cv::FONT_HERSHEY_SIMPLEX,
                                           0.5, 1, &baseline);
        cv::Point tl(static_cast<int>(box.x), static_cast<int>(box.y) - text_sz.height - 4);
        cv::rectangle(img, tl,
                      tl + cv::Point(text_sz.width + 4, text_sz.height + 6),
                      color, cv::FILLED);
        cv::putText(img, label.str(),
                    tl + cv::Point(2, text_sz.height + 2),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, COLOR_WHITE, 1);

        // 如果有距离信息，显示在框下方
        if (det.distance > 0.01f) {
            std::ostringstream dist_str;
            dist_str << std::fixed << std::setprecision(2) << det.distance << "m";
            cv::putText(img, dist_str.str(),
                        cv::Point(static_cast<int>(box.x), static_cast<int>(box.y + box.height + 16)),
                        cv::FONT_HERSHEY_SIMPLEX, 0.45, COLOR_YELLOW, 1);
        }
    }
}

// ============================================================
// 在图像左上角画统计信息
// ============================================================
void DebugNode::drawStats(cv::Mat & img, int cam_id)
{
    auto & stats = cam_stats_[cam_id];
    double fps = computeFps(stats);

    // 半透明背景
    cv::Mat overlay = img.clone();
    cv::rectangle(overlay, cv::Rect(5, 5, 260, 85), COLOR_BG, cv::FILLED);
    cv::addWeighted(overlay, 0.6, img, 0.4, 0, img);

    int y = 22;
    auto put = [&](const std::string & text, cv::Scalar color) {
        cv::putText(img, text, cv::Point(12, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
        y += 20;
    };

    std::string cam_label = "CAM" + std::to_string(cam_id) + " [" + topic_names_[cam_id] + "]";
    // 截断过长的 topic
    if (cam_label.size() > 35) cam_label = cam_label.substr(0, 35) + "...";
    put(cam_label, COLOR_GREEN);

    // FPS + 状态颜色
    std::ostringstream fps_str;
    fps_str << "FPS: " << std::fixed << std::setprecision(1) << fps;
    cv::Scalar fps_color = (fps > 20) ? COLOR_GREEN : (fps > 5 ? COLOR_YELLOW : COLOR_RED);
    put(fps_str.str(), fps_color);

    // 如果 FPS 过低，给出提醒
    if (fps < 5 && fps > 0) {
        put("! LOW FPS - check camera", COLOR_RED);
    } else {
        std::ostringstream det_str;
        det_str << "Detections: " << stats.det_count;
        put(det_str.str(), COLOR_WHITE);
    }
}

// ============================================================
// 鸟瞰图 (Bird-Eye View)
// ============================================================
void DebugNode::drawBirdEyeView()
{
    cv::Mat bev(bev_size_, bev_size_, CV_8UC3, COLOR_BG);
    int center = bev_size_ / 2;
    double scale = center / bev_range_;  // 像素/米

    // 网格线
    for (double r = 2.0; r <= bev_range_; r += 2.0) {
        int radius = static_cast<int>(r * scale);
        cv::circle(bev, {center, center}, radius, COLOR_GRAY, 1);
        // 标注距离
        std::ostringstream dist;
        dist << static_cast<int>(r) << "m";
        cv::putText(bev, dist.str(),
                    cv::Point(center + radius + 3, center - 3),
                    cv::FONT_HERSHEY_SIMPLEX, 0.35, COLOR_GRAY, 1);
    }
    // 十字线
    cv::line(bev, {center, 0}, {center, bev_size_}, COLOR_GRAY, 1);
    cv::line(bev, {0, center}, {bev_size_, center}, COLOR_GRAY, 1);

    // 哨兵本体（中心三角形表示前方）
    std::vector<cv::Point> tri = {
        {center, center - 12},
        {center - 8, center + 8},
        {center + 8, center + 8}
    };
    cv::fillConvexPoly(bev, tri, COLOR_GREEN);
    cv::putText(bev, "SENTRY", cv::Point(center - 25, center + 25),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, COLOR_GREEN, 1);

    // 画 3 路相机 FOV 覆盖区（简易扇形，每路 120 度）
    for (int i = 0; i < static_cast<int>(topic_names_.size()); ++i) {
        double angle_start = (i * 120.0 - 60.0) * CV_PI / 180.0;
        double angle_end = (i * 120.0 + 60.0) * CV_PI / 180.0;
        int fov_radius = static_cast<int>(bev_range_ * 0.8 * scale);

        // 半透明扇形
        cv::Mat fov_overlay = bev.clone();
        cv::ellipse(fov_overlay, cv::Point(center, center),
                    cv::Size(fov_radius, fov_radius),
                    -90.0,  // OpenCV 以右为 0 度，我们以上为前
                    i * 120.0 - 60.0, i * 120.0 + 60.0,
                    cv::Scalar(40, 60, 40), cv::FILLED);
        cv::addWeighted(fov_overlay, 0.3, bev, 0.7, 0, bev);

        // 相机标签
        double mid_angle = (angle_start + angle_end) / 2.0;
        int lx = center + static_cast<int>(fov_radius * 0.5 * std::sin(mid_angle));
        int ly = center - static_cast<int>(fov_radius * 0.5 * std::cos(mid_angle));
        cv::putText(bev, "C" + std::to_string(i),
                    cv::Point(lx - 8, ly + 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, COLOR_GREEN, 1);
    }

    // 画融合目标
    for (const auto & t : latest_targets_) {
        // 坐标转换：x=前, y=左 => BEV 上: px=center-y*scale, py=center-x*scale
        int px = center - static_cast<int>(t.y * scale);
        int py = center - static_cast<int>(t.x * scale);

        if (px < 5 || px > bev_size_ - 5 || py < 5 || py > bev_size_ - 5) continue;

        cv::Scalar target_color = t.is_tracking ? COLOR_RED : COLOR_YELLOW;
        cv::circle(bev, {px, py}, 8, target_color, cv::FILLED);
        cv::circle(bev, {px, py}, 10, COLOR_WHITE, 1);

        // 目标标签
        std::ostringstream lbl;
        lbl << "ID" << static_cast<int>(t.id) << " "
            << std::fixed << std::setprecision(1)
            << std::sqrt(t.x * t.x + t.y * t.y) << "m";
        cv::putText(bev, lbl.str(), cv::Point(px + 12, py + 4),
                    cv::FONT_HERSHEY_SIMPLEX, 0.38, COLOR_WHITE, 1);

        // yaw 方向箭头
        int ax = px + static_cast<int>(20 * std::sin(t.yaw));
        int ay = py - static_cast<int>(20 * std::cos(t.yaw));
        cv::arrowedLine(bev, {px, py}, {ax, ay}, target_color, 2);
    }

    // 底部状态栏
    cv::rectangle(bev, cv::Rect(0, bev_size_ - 28, bev_size_, 28), COLOR_BG, cv::FILLED);
    std::ostringstream status;
    status << "Targets: " << total_fused_targets_
           << " | BEV range: " << std::fixed << std::setprecision(0) << bev_range_ << "m"
           << " | Cameras active: ";
    int active = 0;
    for (auto & [id, s] : cam_stats_) {
        if (computeFps(s) > 0.5) active++;
    }
    status << active << "/" << topic_names_.size();
    cv::putText(bev, status.str(), cv::Point(8, bev_size_ - 8),
                cv::FONT_HERSHEY_SIMPLEX, 0.42, COLOR_GREEN, 1);

    // 发布
    auto bev_msg = cv_bridge::CvImage(
        std_msgs::msg::Header(), "bgr8", bev).toImageMsg();
    bev_msg->header.stamp = now();
    bev_pub_->publish(*bev_msg);

    if (show_windows_) {
        cv::imshow("BEV - Omnidirectional", bev);
    }
}

// ============================================================
// 参数面板：显示所有可调参数 + 调节方法
// ============================================================
void DebugNode::drawParamPanel()
{
    const int W = 520, H = 580;
    cv::Mat panel(H, W, CV_8UC3, COLOR_BG);

    int y = 0;
    auto title = [&](const std::string & text) {
        y += 28;
        cv::putText(panel, text, cv::Point(10, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.55, COLOR_GREEN, 1);
        y += 4;
        cv::line(panel, {10, y}, {W - 10, y}, COLOR_GRAY, 1);
    };
    auto row = [&](const std::string & name, const std::string & value,
                    const std::string & hint, cv::Scalar val_color = COLOR_WHITE) {
        y += 20;
        cv::putText(panel, name, cv::Point(14, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.42, COLOR_GRAY, 1);
        cv::putText(panel, value, cv::Point(200, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.42, val_color, 1);
        cv::putText(panel, hint, cv::Point(320, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.33, COLOR_YELLOW, 1);
    };

    // ---- 标题 ----
    y += 30;
    cv::putText(panel, "SENTRY OMNI PERCEPTION - PARAM PANEL", cv::Point(10, y),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, COLOR_YELLOW, 1);

    // ---- 相机参数 ----
    title("[Camera - hik_camera.yaml]");
    row("exposure_time", "500 us", "dark->inc, bright->dec");
    row("gain", "10", "noise->dec, dark->inc");
    row("frame_rate", "120 fps", "bandwidth limit");

    // ---- 检测参数 ----
    title("[Detection - ml.yaml]");

    // 动态读取当前节点能看到的参数（如果感知节点在同一进程则可读）
    // 这里用默认值展示，实际运行后会从 topic 数据反推
    row("min_confidence", "0.80", "miss->dec, FP->inc");
    row("nms_threshold", "0.45", "overlap->dec");
    row("yolo_name", "yolov5", "v5/v8/11");
    row("device", "GPU", "GPU/CPU");
    row("input_size", "640x640", "speed<->accuracy");

    // ---- 融合参数 ----
    title("[Fusion - ml.yaml]");
    row("sync_timeout", "0.08s", "delay->inc, ghost->dec");
    row("dup_dist_thresh", "0.4m", "merge->inc, split->dec");
    row("publish_rate", "30 Hz", "smooth vs latency");

    // ---- 坐标变换 ----
    title("[CoordTransform - code]");
    row("armor_real_h", "0.055m", "55mm=small, 127mm=big");
    row("extrinsic", "IDENTITY", "!! CALIBRATE !!", COLOR_RED);

    // ---- 运行状态 ----
    title("[Runtime Status]");
    for (size_t i = 0; i < topic_names_.size(); ++i) {
        auto it = cam_stats_.find(static_cast<int>(i));
        std::string fps_val = "N/A";
        std::string det_val = "0";
        cv::Scalar c = COLOR_RED;
        if (it != cam_stats_.end()) {
            double fps = computeFps(it->second);
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << fps << " fps";
            fps_val = ss.str();
            det_val = std::to_string(it->second.det_count);
            c = (fps > 20) ? COLOR_GREEN : (fps > 5 ? COLOR_YELLOW : COLOR_RED);
        }
        std::string cam_name = "cam" + std::to_string(i);
        row(cam_name, fps_val + " | det:" + det_val, "", c);
    }

    // 融合目标数
    {
        y += 20;
        std::string tgt = "Fused targets: " + std::to_string(total_fused_targets_);
        cv::Scalar c = (total_fused_targets_ > 0) ? COLOR_GREEN : COLOR_YELLOW;
        cv::putText(panel, tgt, cv::Point(14, y),
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, c, 1);
    }

    // ---- 底部提示 ----
    y = H - 40;
    cv::putText(panel, "Tune via: ros2 param set <node> <param> <value>",
                cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.38, COLOR_YELLOW, 1);
    y += 18;
    cv::putText(panel, "Or edit yaml then rebuild. Press 'q' to quit debug.",
                cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.35, COLOR_GRAY, 1);

    // 发布
    auto panel_msg = cv_bridge::CvImage(
        std_msgs::msg::Header(), "bgr8", panel).toImageMsg();
    panel_msg->header.stamp = now();
    panel_pub_->publish(*panel_msg);

    if (show_windows_) {
        cv::imshow("Param Panel", panel);
    }
}

// ============================================================
// 工具函数
// ============================================================
double DebugNode::computeFps(CameraStats & stats)
{
    if (stats.fps_window.size() < 2) return 0.0;
    double dt = (stats.fps_window.back() - stats.fps_window.front()).seconds();
    if (dt < 1e-6) return 0.0;
    return static_cast<double>(stats.fps_window.size() - 1) / dt;
}

cv::Scalar DebugNode::colorForId(uint8_t color)
{
    // 模型输出: 0=蓝, 1=红, 2=灰, 3=紫
    return (color == 1) ? COLOR_RED : COLOR_BLUE;
}

std::string DebugNode::numberLabel(uint8_t number)
{
    switch (number) {
        case 1: return "Hero";
        case 2: return "Eng";
        case 3: return "Inf3";
        case 4: return "Inf4";
        case 5: return "Inf5";
        case 6: return "Outpost";
        case 7: return "Base";
        default: return "?";
    }
}

}  // namespace sentry_vision

// ============================================================
// main
// ============================================================
int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sentry_vision::DebugNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
