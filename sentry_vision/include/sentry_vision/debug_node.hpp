#ifndef SENTRY_VISION__DEBUG_NODE_HPP_
#define SENTRY_VISION__DEBUG_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

#include <deque>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "sentry_interfaces/msg/armor_detection_array.hpp"
#include "sentry_interfaces/msg/enemy_target_array.hpp"

namespace sentry_vision
{

/// 每路相机的运行时统计
struct CameraStats
{
    int frame_count = 0;
    double fps = 0.0;
    int det_count = 0;             // 最近一帧检测数
    rclcpp::Time last_image_time;  // 最近一帧 stamp
    std::deque<rclcpp::Time> fps_window;
};

class DebugNode : public rclcpp::Node
{
public:
    DebugNode();

private:
    // ---------- 回调 ----------
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int cam_id);
    void detectionCallback(const sentry_interfaces::msg::ArmorDetectionArray::SharedPtr msg);
    void targetCallback(const sentry_interfaces::msg::EnemyTargetArray::SharedPtr msg);
    void timerCallback();

    // ---------- 可视化绘制 ----------
    void drawDetections(cv::Mat & img, int cam_id);
    void drawStats(cv::Mat & img, int cam_id);
    void drawBirdEyeView();
    void drawParamPanel();

    // ---------- 工具 ----------
    double computeFps(CameraStats & stats);
    cv::Scalar colorForId(uint8_t color);
    std::string numberLabel(uint8_t number);

    // ---------- 参数 ----------
    std::vector<std::string> topic_names_;
    bool show_windows_;    // 是否 imshow（无头机可关闭，只发 topic）
    double bev_range_;     // BEV 显示范围 (米)
    int bev_size_;         // BEV 画布像素

    // ---------- 订阅 / 发布 ----------
    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> img_subs_;
    rclcpp::Subscription<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::EnemyTargetArray>::SharedPtr target_sub_;

    // 发布标注后的图像（供 rqt_image_view 使用）
    std::vector<rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr> annotated_pubs_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr bev_pub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr panel_pub_;

    rclcpp::TimerBase::SharedPtr timer_;

    // ---------- 数据缓存 ----------
    std::mutex mutex_;
    std::unordered_map<int, cv::Mat> latest_images_;
    std::unordered_map<int, std::vector<sentry_interfaces::msg::ArmorDetection>> latest_dets_;
    std::vector<sentry_interfaces::msg::EnemyTarget> latest_targets_;
    std::unordered_map<int, CameraStats> cam_stats_;
    int total_fused_targets_ = 0;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__DEBUG_NODE_HPP_
