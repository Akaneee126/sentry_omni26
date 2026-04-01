// sentry_vision_node.cpp —— 一体化节点：检测 + 融合 + 发布
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "sentry_interfaces/msg/enemy_target.hpp"
#include "sentry_interfaces/msg/enemy_target_array.hpp"

#include "sentry_vision/param_manager.hpp"
#include "sentry_vision/armor_detector.hpp"
#include "sentry_vision/multi_camera_buffer.hpp"
#include "sentry_vision/target_combine.hpp"

namespace sentry_vision
{

class SentryVisionNode : public rclcpp::Node
{
public:
    SentryVisionNode()
    : Node("sentry_vision_node")
    {
    }

    void init()
    {
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());
        detector_ = std::make_unique<ArmorDetector>(*param_manager_);
        buffer_   = std::make_unique<MultiCameraBuffer>(*param_manager_);
        combine_  = std::make_unique<TargetCombine>(*param_manager_);

        // 发布最终目标
        target_pub_ = this->create_publisher<sentry_interfaces::msg::EnemyTargetArray>(
            "/enemy_targets", 10);

        // 订阅多路相机
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

        // 定时融合
        int period_ms = static_cast<int>(1000.0 / param_manager_->publish_rate());
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(period_ms),
            std::bind(&SentryVisionNode::timerCallback, this));

        RCLCPP_INFO(this->get_logger(), "SentryVisionNode started");
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

        buffer_->update(camera_id, msg->header, detections);
    }

    void timerCallback()
    {
        auto valid_inputs = buffer_->getValidDetections(this->now());

        std::vector<TargetState> fused_targets;
        combine_->fuse(valid_inputs, fused_targets);

        sentry_interfaces::msg::EnemyTargetArray out_msg;
        out_msg.header.stamp = this->now();
        out_msg.header.frame_id = "base_link";

        for (const auto & target : fused_targets) {
            sentry_interfaces::msg::EnemyTarget item;
            item.header = out_msg.header;
            item.id = target.id;
            item.x = target.x;
            item.y = target.y;
            item.z = target.z;
            item.yaw = target.yaw;
            item.confidence = target.confidence;
            item.is_tracking = target.is_tracking;
            out_msg.targets.push_back(item);
        }

        target_pub_->publish(out_msg);
    }

    std::shared_ptr<ParamManager> param_manager_;
    std::unique_ptr<ArmorDetector> detector_;
    std::unique_ptr<MultiCameraBuffer> buffer_;
    std::unique_ptr<TargetCombine> combine_;

    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> image_subs_;
    rclcpp::Publisher<sentry_interfaces::msg::EnemyTargetArray>::SharedPtr target_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace sentry_vision

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sentry_vision::SentryVisionNode>();
    node->init();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}