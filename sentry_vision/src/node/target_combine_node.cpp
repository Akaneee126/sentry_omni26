#include <memory>
#include <vector>
#include <chrono>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "sentry_interfaces/msg/armor_detection_array.hpp"
#include "sentry_interfaces/msg/enemy_target.hpp"
#include "sentry_interfaces/msg/enemy_target_array.hpp"

#include "sentry_vision/param_manager.hpp"
#include "sentry_vision/multi_camera_buffer.hpp"
#include "sentry_vision/target_combine.hpp"

namespace sentry_vision
{

class TargetCombineNode : public rclcpp::Node
{
public:
    TargetCombineNode()
    : Node("target_combine_node")
    {
    }

    void init()
    {
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());
        buffer_ = std::make_unique<MultiCameraBuffer>(*param_manager_);
        combine_ = std::make_unique<TargetCombine>(*param_manager_);

        // 订阅检测结果
        det_sub_ = this->create_subscription<sentry_interfaces::msg::ArmorDetectionArray>(
            "/armor_detections", 20,
            std::bind(&TargetCombineNode::detectionCallback, this, std::placeholders::_1));

        // 发布融合目标
        target_pub_ = this->create_publisher<sentry_interfaces::msg::EnemyTargetArray>(
            "/enemy_targets", 10);

        // 定时融合（约 30Hz）
        int period_ms = static_cast<int>(1000.0 / param_manager_->publish_rate());
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(period_ms),
            std::bind(&TargetCombineNode::timerCallback, this));

        RCLCPP_INFO(this->get_logger(), "TargetCombineNode started, fuse rate=%dHz", 1000 / period_ms);
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
    std::unique_ptr<MultiCameraBuffer> buffer_;
    std::unique_ptr<TargetCombine> combine_;

    rclcpp::Subscription<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_sub_;
    rclcpp::Publisher<sentry_interfaces::msg::EnemyTargetArray>::SharedPtr target_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace sentry_vision

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sentry_vision::TargetCombineNode>();
    node->init();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}