#include "sentry_vision/multi_camera_buffer.hpp"
#include "sentry_vision/param_manager.hpp"

namespace sentry_vision
{

MultiCameraBuffer::MultiCameraBuffer(const ParamManager & params)
: camera_num_(params.camera_num()),
  sync_timeout_(params.sync_timeout())
{
}

void MultiCameraBuffer::update(int camera_id,
                               const std_msgs::msg::Header & header,
                               const std::vector<DetectionBox> & detections)
{
    std::lock_guard<std::mutex> lock(mutex_);

    CameraDetectionStamped & slot = buffer_[camera_id];
    slot.camera_id = camera_id;
    slot.stamp = rclcpp::Time(header.stamp);
    slot.header = header;
    slot.detections = detections;
}

void MultiCameraBuffer::update(const sentry_interfaces::msg::ArmorDetectionArray & msg)
{
    std::lock_guard<std::mutex> lock(mutex_);

    int cam_id = static_cast<int>(msg.camera_id);
    CameraDetectionStamped & slot = buffer_[cam_id];
    slot.camera_id = cam_id;
    slot.stamp = rclcpp::Time(msg.header.stamp);
    slot.header = msg.header;

    slot.detections.clear();
    slot.detections.reserve(msg.detections.size());
    for (const auto & d : msg.detections) {
        DetectionBox box;
        box.x = d.x;
        box.y = d.y;
        box.width = d.width;
        box.height = d.height;
        box.confidence = d.confidence;
        box.color = d.target_color;
        box.number = d.target_number;
        box.class_id = -1;
        slot.detections.push_back(box);
    }
}

std::vector<CameraDetectionStamped> MultiCameraBuffer::getValidDetections(
    const rclcpp::Time & now) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<CameraDetectionStamped> result;
    for (const auto & [cam_id, slot] : buffer_) {
        double dt = (now - slot.stamp).seconds();
        if (dt >= 0.0 && dt < sync_timeout_) {
            result.push_back(slot);
        }
    }
    return result;
}

}  // namespace sentry_vision
