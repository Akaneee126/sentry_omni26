#ifndef SENTRY_VISION__MULTI_CAMERA_BUFFER_HPP_
#define SENTRY_VISION__MULTI_CAMERA_BUFFER_HPP_

#include <mutex>
#include <vector>
#include <unordered_map>
#include <rclcpp/rclcpp.hpp>

#include "sentry_vision/types.hpp"
#include "sentry_interfaces/msg/armor_detection_array.hpp"

namespace sentry_vision
{

class ParamManager;

/// 每个相机的最新一帧检测结果缓存
struct CameraDetectionStamped
{
    int camera_id = -1;
    rclcpp::Time stamp;
    std_msgs::msg::Header header;
    std::vector<DetectionBox> detections;
};

class MultiCameraBuffer
{
public:
    explicit MultiCameraBuffer(const ParamManager & params);

    /// 来自 sentry_perception_node：直接用内部结构更新
    void update(int camera_id, const std_msgs::msg::Header & header,
                const std::vector<DetectionBox> & detections);

    /// 来自 target_combine_node：用 ROS 消息更新
    void update(const sentry_interfaces::msg::ArmorDetectionArray & msg);

    /// 获取当前时刻还有效的（未超时的）所有相机检测
    std::vector<CameraDetectionStamped> getValidDetections(const rclcpp::Time & now) const;

    int camera_num() const { return camera_num_; }

private:
    int camera_num_;
    double sync_timeout_;

    mutable std::mutex mutex_;
    std::unordered_map<int, CameraDetectionStamped> buffer_;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__MULTI_CAMERA_BUFFER_HPP_
