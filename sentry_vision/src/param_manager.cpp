#include "sentry_vision/param_manager.hpp"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/core.hpp>

namespace sentry_vision
{

// ============================================================
// 构造
// ============================================================
ParamManager::ParamManager(rclcpp::Node::SharedPtr node)
: node_(node)
{
    declareAndReadParams();
}

// ============================================================
// 声明 & 读取 ROS2 参数
// ============================================================
void ParamManager::declareAndReadParams()
{
    auto & n = *node_;

    // -------- 相机话题（来自 ml.yaml）--------
    n.declare_parameter<std::string>("img1_topic_name", "/mvsua_cam/image_raw2");
    n.declare_parameter<std::string>("img2_topic_name", "/mvsua_cam/image_raw3");
    n.declare_parameter<std::string>("img3_topic_name", "/mvsua_cam/image_raw4");

    camera_topics_.push_back(n.get_parameter("img1_topic_name").as_string());
    camera_topics_.push_back(n.get_parameter("img2_topic_name").as_string());
    camera_topics_.push_back(n.get_parameter("img3_topic_name").as_string());

    RCLCPP_INFO(n.get_logger(), "Loaded %zu camera topics", camera_topics_.size());
    for (size_t i = 0; i < camera_topics_.size(); ++i) {
        RCLCPP_INFO(n.get_logger(), "  cam%zu: %s", i, camera_topics_[i].c_str());
    }

    // -------- 图像参数（来自 camera.yaml）--------
    n.declare_parameter<int>("image_width", 1280);
    n.declare_parameter<int>("image_height", 720);
    image_width_  = n.get_parameter("image_width").as_int();
    image_height_ = n.get_parameter("image_height").as_int();

    // -------- 检测器参数（来自 ml.yaml）--------
    n.declare_parameter<std::string>("yolo_name", "yolov5");
    n.declare_parameter<std::string>("yolov5_model_path", "assets/yolov5.xml");
    n.declare_parameter<std::string>("yolov8_model_path", "assets/yolov8.xml");
    n.declare_parameter<std::string>("yolo11_model_path", "assets/yolo11.xml");
    n.declare_parameter<std::string>("device", "GPU");
    n.declare_parameter<double>("min_confidence", 0.8);
    n.declare_parameter<double>("nms_threshold", 0.45);
    n.declare_parameter<int>("input_width", 640);
    n.declare_parameter<int>("input_height", 640);

    yolo_name_      = n.get_parameter("yolo_name").as_string();
    device_         = n.get_parameter("device").as_string();
    conf_threshold_ = static_cast<float>(n.get_parameter("min_confidence").as_double());
    nms_threshold_  = static_cast<float>(n.get_parameter("nms_threshold").as_double());
    input_width_    = n.get_parameter("input_width").as_int();
    input_height_   = n.get_parameter("input_height").as_int();

    resolveModelPath();
    RCLCPP_INFO(n.get_logger(), "Model: %s (%s) on %s, conf=%.2f, nms=%.2f",
                yolo_name_.c_str(), model_path_.c_str(), device_.c_str(),
                conf_threshold_, nms_threshold_);

    // -------- 融合参数 --------
    n.declare_parameter<double>("sync_timeout", 0.08);
    n.declare_parameter<double>("duplicate_distance_thresh", 0.4);
    n.declare_parameter<double>("publish_rate", 30.0);
    sync_timeout_             = n.get_parameter("sync_timeout").as_double();
    duplicate_distance_thresh_ = n.get_parameter("duplicate_distance_thresh").as_double();
    publish_rate_             = n.get_parameter("publish_rate").as_double();

    // -------- 内参文件路径 --------
    n.declare_parameter<std::string>("intrinsic_path", "");
    std::string intrinsic_path = n.get_parameter("intrinsic_path").as_string();
    if (!intrinsic_path.empty()) {
        loadIntrinsics(intrinsic_path);
    } else {
        RCLCPP_WARN(n.get_logger(), "intrinsic_path not set, camera intrinsics not loaded");
    }
}

// ============================================================
// 根据 yolo_name 选择对应的模型路径
// ============================================================
void ParamManager::resolveModelPath()
{
    auto & n = *node_;
    if (yolo_name_ == "yolov5") {
        model_path_ = n.get_parameter("yolov5_model_path").as_string();
    } else if (yolo_name_ == "yolov8") {
        model_path_ = n.get_parameter("yolov8_model_path").as_string();
    } else if (yolo_name_ == "yolo11") {
        model_path_ = n.get_parameter("yolo11_model_path").as_string();
    } else {
        RCLCPP_ERROR(n.get_logger(), "Unknown yolo_name: %s, falling back to yolov5",
                     yolo_name_.c_str());
        model_path_ = n.get_parameter("yolov5_model_path").as_string();
    }
}

// ============================================================
// 加载 Intrinsic.yaml（JSON 格式）中的 相机内参和畸变
// ============================================================
void ParamManager::loadIntrinsics(const std::string & intrinsic_path)
{
    // 你的 Intrinsic.yaml 实际上是 JSON 格式
    // 用 OpenCV FileStorage 打开（它支持 JSON/XML/YAML）
    cv::FileStorage fs(intrinsic_path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        RCLCPP_ERROR(node_->get_logger(),
                     "Failed to open intrinsic file: %s", intrinsic_path.c_str());
        return;
    }

    // Intrinsic.yaml 结构:
    // { "Hikvision1280_1024": { "Prime6mm1": { "Intrinsic": {...}, "Distortion": {...} },
    //                           "Prime6mm2": {...}, "Prime6mm3": {...} } }
    cv::FileNode root = fs["Hikvision1280_1024"];
    if (root.empty()) {
        RCLCPP_ERROR(node_->get_logger(), "Intrinsic file missing 'Hikvision1280_1024' key");
        return;
    }

    // 按 Prime6mm1, Prime6mm2, Prime6mm3 的顺序对应 cam0, cam1, cam2
    const std::vector<std::string> cam_keys = {"Prime6mm1", "Prime6mm2", "Prime6mm3"};
    camera_params_.clear();

    for (size_t i = 0; i < cam_keys.size(); ++i) {
        cv::FileNode cam_node = root[cam_keys[i]];
        if (cam_node.empty()) {
            RCLCPP_WARN(node_->get_logger(), "Missing intrinsic for %s", cam_keys[i].c_str());
            camera_params_.push_back(CameraParam{});
            continue;
        }

        CameraParam param;

        // 读取内参矩阵
        cv::FileNode intr_node = cam_node["Intrinsic"];
        int rows = static_cast<int>(intr_node["rows"]);
        int cols = static_cast<int>(intr_node["cols"]);
        std::vector<double> intr_data;
        intr_node["data"] >> intr_data;
        param.intrinsic = cv::Mat(rows, cols, CV_64F);
        std::memcpy(param.intrinsic.data, intr_data.data(),
                    intr_data.size() * sizeof(double));

        // 读取畸变系数
        cv::FileNode dist_node = cam_node["Distortion"];
        int d_rows = static_cast<int>(dist_node["rows"]);
        int d_cols = static_cast<int>(dist_node["cols"]);
        std::vector<double> dist_data;
        dist_node["data"] >> dist_data;
        param.distortion = cv::Mat(d_rows, d_cols, CV_64F);
        std::memcpy(param.distortion.data, dist_data.data(),
                    dist_data.size() * sizeof(double));

        // 外参暂时设为单位阵，后续你可以从额外 yaml 或 TF 获取
        param.extrinsic = cv::Mat::eye(4, 4, CV_64F);

        camera_params_.push_back(param);

        RCLCPP_INFO(node_->get_logger(), "Loaded intrinsic for %s (cam%zu)",
                    cam_keys[i].c_str(), i);
    }
}

}  // namespace sentry_vision
