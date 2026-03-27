

class SentryVisionNode : public rclcpp::Node
{
public:
    SentryVisionNode() : Node("sentry_vision_node")
    {
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());
        detector_ = std::make_unique<ArmorDetector>(*param_manager_);
        buffer_ = std::make_unique<MultiCameraBuffer>(*param_manager_);
        combine_ = std::make_unique<TargetCombine>(*param_manager_);
        target_pub_ = this->create_publisher<sentry_interfaces::msg::EnemyTargetArray>("/enemy_targets", 10);
        auto camera_topics = param_manager_->camera_topics();
        for (size_t i = 0; i < camera_topics.size(); ++i)
        {
            auto sub = this->create_subscription<sensor_msgs::msg::Image>(
                camera_topics[i], 10,
                [this, i](const sensor_msgs::msg::Image::SharedPtr msg)
                {
                    this->imageCallback(msg, static_cast<int>(i));
                });
            image_subs_.push_back(sub);
        }

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33),
            std::bind(&SentryVisionNode::timerCallback, this));
    }

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int camera_id)
    {
        cv::Mat image = cv_bridge::toCvCopy(msg, "bgr8")->image;

        std::vector<DetectionBox> detections;
        detector_->infer(image, camera_id, detections);

        buffer_->update(camera_id, msg->header, detections);
    }
    void timerCallback()
    {
        auto valid_inputs = buffer_->getValidDetections(this->now());
        std::vector<TargetState> fused_targets;
        fusion_->fuse(valid_inputs, fused_targets);
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
    std::unique_ptr<ArmorDetector> detector_;
    std::unique_ptr<MultiCameraBuffer> buffer_;
    std::unique_ptr<TargetCombine> fusion_;

    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> image_subs_;
    rclcpp::Publisher<sentry_interfaces::msg::EnemyTargetArray>::SharedPtr target_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};