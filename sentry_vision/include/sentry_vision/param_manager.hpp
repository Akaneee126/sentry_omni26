#ifndef SENTRY_VISION__PARAM_MANAGER_HPP_
#define SENTRY_VISION__PARAM_MANAGER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <opencv2/core.hpp>

#include "sentry_vision/types.hpp"

namespace sentry_vision
{

class ParamManager
{
public:
    explicit ParamManager(rclcpp::Node::SharedPtr node);

    // -------- 相机话题 --------
    const std::vector<std::string> & camera_topics() const { return camera_topics_; }
    int camera_num() const { return static_cast<int>(camera_topics_.size()); }

    // -------- 图像参数 --------
    int image_width() const { return image_width_; }
    int image_height() const { return image_height_; }

    // -------- 检测器参数 --------
    const std::string & model_path() const { return model_path_; }
    const std::string & yolo_name() const { return yolo_name_; }
    const std::string & device() const { return device_; }
    float conf_threshold() const { return conf_threshold_; }
    float nms_threshold() const { return nms_threshold_; }
    int input_width() const { return input_width_; }
    int input_height() const { return input_height_; }
    int num_classes() const { return num_classes_; }
    int num_keypoints() const { return num_keypoints_; }
    int kpt_channels() const { return kpt_channels_; }
    bool has_objectness() const { return has_objectness_; }
    bool apply_sigmoid() const { return apply_sigmoid_; }

    // -------- 相机内参/畸变 --------
    const CameraParam & camera_param(int cam_id) const { return camera_params_.at(cam_id); }
    const std::vector<CameraParam> & camera_params() const { return camera_params_; }

    // -------- 融合参数 --------
    double sync_timeout() const { return sync_timeout_; }
    double duplicate_distance_thresh() const { return duplicate_distance_thresh_; }
    double publish_rate() const { return publish_rate_; }

private:
    void declareAndReadParams();
    void loadIntrinsics(const std::string & intrinsic_path);
    void resolveModelPath();

    rclcpp::Node::SharedPtr node_;

    // 相机话题
    std::vector<std::string> camera_topics_;

    // 图像
    int image_width_ = 1280;
    int image_height_ = 720;

    // 检测器
    std::string yolo_name_;
    std::string model_path_;
    std::string device_;
    float conf_threshold_ = 0.8f;
    float nms_threshold_ = 0.45f;
    int input_width_ = 640;
    int input_height_ = 640;
    int num_classes_ = 5;
    int num_keypoints_ = 4;
    int kpt_channels_ = 3;
    bool has_objectness_ = true;
    bool apply_sigmoid_ = true;

    // 相机内参
    std::vector<CameraParam> camera_params_;

    // 融合
    double sync_timeout_ = 0.08;
    double duplicate_distance_thresh_ = 0.4;
    double publish_rate_ = 30.0;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__PARAM_MANAGER_HPP_
