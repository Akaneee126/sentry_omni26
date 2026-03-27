#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>
#include <string>

#include "sentry_interfaces/msg/armor_detection.hpp"
#include "sentry_interfaces/msg/armor_detection_array.hpp"
#include "sentry_vision/param_manager.hpp"
#include "sentry_vision/armor_detector.hpp"

namespace sentry_vision
{

class ArmorDetectorNode : public rclcpp::Node
{
public:
    ArmorDetectorNode()
    : Node("armor_detector_node")
    {
        // 参数管理
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());

        // 检测器
        detector_ = std::make_unique<ArmorDetector>(*param_manager_);

        // 发布检测结果
        det_pub_ = this->create_publisher<sentry_interfaces::msg::ArmorDetectionArray>(
            "/armor_detections", 10);

        // 订阅多路相机图像
        auto camera_topics = param_manager_->camera_topics();
        for (size_t i = 0; i < camera_topics.size(); ++i) {
            auto sub = this->create_subscription<sensor_msgs::msg::Image>(
                camera_topics[i], 10,
                [this, i](const sensor_msgs::msg::Image::SharedPtr msg) {
                    this->imageCallback(msg, static_cast<int>(i));
                });
            image_subs_.push_back(sub);
            RCLCPP_INFO(this->get_logger(), "Subscribed to %s (cam%zu)",
                        camera_topics[i].c_str(), i);
        }

        RCLCPP_INFO(this->get_logger(), "ArmorDetectorNode started");
    }

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int camera_id)
    {
        cv::Mat image;
        try {
            image = cv_bridge::toCvCopy(msg, "bgr8")->image;
        } catch (const cv_bridge::Exception & e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge error: %s", e.what());
            return;
        }

        std::vector<DetectionBox> detections;
        detector_->infer(image, camera_id, detections);

        // 组装 ROS 消息
        sentry_interfaces::msg::ArmorDetectionArray out_msg;
        out_msg.header = msg->header;
        out_msg.camera_id = static_cast<uint8_t>(camera_id);

        for (const auto & det : detections) {
            sentry_interfaces::msg::ArmorDetection item;
            item.header = msg->header;
            item.camera_id = static_cast<uint8_t>(camera_id);
            item.target_color = det.color;
            item.target_number = det.number;
            item.confidence = det.confidence;
            item.x = det.x;
            item.y = det.y;
            item.width = det.width;
            item.height = det.height;
            out_msg.detections.push_back(item);
        }

        det_pub_->publish(out_msg);
    }

    std::shared_ptr<ParamManager> param_manager_;
    std::unique_ptr<ArmorDetector> detector_;
    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> image_subs_;
    rclcpp::Publisher<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_pub_;
};

}  // namespace sentry_vision

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sentry_vision::ArmorDetectorNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
