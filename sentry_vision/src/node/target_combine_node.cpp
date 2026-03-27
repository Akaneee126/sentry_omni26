#include <memory>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "sentry_interfaces/msg/armor_detection_array.hpp"
#include "sentry_interfaces/msg/enemy_target_array.hpp"

class TargetCombineNode : public rclcpp::Node
{
public:
    TargetCombineNode() : Node("target_combine_node")
    {
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());
        buffer_ = std::make_unique<MultiCameraBuffer>(*param_manager_);
        combine_ = std::make_unique<TargetCombine>(*param_manager_);

        det_sub_ = this->create_subscription<sentry_interfaces::msg::ArmorDetectionArray>(
            "/armor_detections", 20,
            std::bind(&TargetCombineNode::detectionCallback, this, std::placeholders::_1));

        target_pub_ = this->create_publisher<sentry_interfaces::msg::EnemyTargetArray>(
            "/enemy_targets", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33),
            std::bind(&TargetCombineNode::timerCallback, this));
    }

private:
    void detectionCallback(const sentry_interfaces::msg::ArmorDetectionArray::SharedPtr msg)
    {
        buffer_->update(*msg);
    }
    void timerCallback()
    {
        auto valid_inputs = buffer_->getValidDetections(this->now());
        std::vector<TargetState> fused_targets;
        combine_->fuse(valid_inputs, fused_targets);
        sentry_interfaces::msg::EnemyTargetArray out_msg;
        out_msg.header.stamp = this->now();
        out_msg.header.frame_id = "base_link";

        for (const auto &target : fused_targets)
        {
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

private:
    std::shared_ptr<ParamManager> param_manager_;
    std::unique_ptr<MultiCameraBuffer> buffer_;
    std::unique_ptr<TargetCombine> combine_;

    rclcpp::Subscription<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_sub_;
    rclcpp::Publisher<sentry_interfaces::msg::EnemyTargetArray>::SharedPtr target_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};