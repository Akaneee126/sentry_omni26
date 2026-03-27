#include <rclcpp/rclcpp.hpp>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include "sentry_interfaces/msg/armor_detection_array.hpp"

namespace sentty_vision
{
class ArmorDetectorNode : public rclcpp::Node

                          public : ArmorDetectorNode() : Node("armor_detector_node")
    {
        param_manager_ = std::make_shared<ParameterManager>(shared_from_this());
        detector_ = std::make_unique<ArmorDetectorNode>(*param_manager_);
        det_pub_ = this->create_publisher<sentry_interfaces::msg::ArmorDetectionArray>("/armor_detections", 10);
        auto camera_topics = param_manager_->camera_topics();
        for (size_t i = 0; i < camera_topics.size(); ++i) // 读取图像
        {
            auto sub = this->create_subscription<sensor_msgs::msg::Image>(
                camera_topics[i], 10,
                [this, i](const sensor_msgs::msg::Image::SharedPtr msg)
                {
                    this->imageCallback(msg, static_cast<int>(i));
                });
            image_subs_.push_back(sub);
        }
    }
}

private:
void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int camera_id)
{
    cv::Mat image = cv_bridge::toCvCopy(msg, "bgr8")->image;

    std::vector<DetectionBox> detections;
    detector_->infer(image, camera_id, detections);

    sentry_interfaces::msg::ArmorDetectionArray out_msg;
    out_msg.header = msg->header;
    out_msg.camera_id = static_cast<uint8_t>(camera_id);

    for (const auto &det : detections)
    {
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

private:
{
    std::shared_ptr<ParamManager> param_manager_;
    std::unique_ptr<ArmorDetector> detector_;

    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> image_subs_;
    rclcpp::Publisher<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_pub_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ArmorDetectorNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
