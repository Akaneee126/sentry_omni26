#ifndef HIK_CAMERA__HIK_CAMERA_NODE_HPP_
#define HIK_CAMERA__HIK_CAMERA_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "MvCameraControl.h"

namespace hik_camera
{

struct CameraInstance
{
    void * handle = nullptr;
    int device_index = -1;
    std::string topic_name;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher;       // 原图
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr small_publisher; // 640x640 小图
    std::thread capture_thread;
    std::atomic<bool> running{false};
    std::string frame_id;

    CameraInstance() = default;
    CameraInstance(CameraInstance && other) noexcept
    : handle(other.handle),
      device_index(other.device_index),
      topic_name(std::move(other.topic_name)),
      publisher(std::move(other.publisher)),
      small_publisher(std::move(other.small_publisher)),
      capture_thread(std::move(other.capture_thread)),
      running(other.running.load()),
      frame_id(std::move(other.frame_id))
    {
        other.handle = nullptr;
    }
    CameraInstance & operator=(CameraInstance && other) noexcept
    {
        if (this != &other) {
            handle = other.handle;
            device_index = other.device_index;
            topic_name = std::move(other.topic_name);
            publisher = std::move(other.publisher);
            small_publisher = std::move(other.small_publisher);
            capture_thread = std::move(other.capture_thread);
            running.store(other.running.load());
            frame_id = std::move(other.frame_id);
            other.handle = nullptr;
        }
        return *this;
    }
    CameraInstance(const CameraInstance &) = delete;
    CameraInstance & operator=(const CameraInstance &) = delete;
};

class HikCameraNode : public rclcpp::Node
{
public:
    HikCameraNode();
    ~HikCameraNode() override;

private:
    void declareParams();
    bool openCamera(CameraInstance & cam, MV_CC_DEVICE_INFO * device_info);
    void captureLoop(int cam_index);
    void stopAll();

    // 参数
    double exposure_time_;   // 微秒
    double gain_;
    double frame_rate_;
    int image_width_;
    int image_height_;

    std::vector<std::string> topic_names_;
    std::vector<CameraInstance> cameras_;
};

}  // namespace hik_camera

#endif  // HIK_CAMERA__HIK_CAMERA_NODE_HPP_
