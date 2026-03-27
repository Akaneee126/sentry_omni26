#include "sentry_vision/coordinate_transformer.hpp"
#include "sentry_vision/param_manager.hpp"

#include <cmath>

namespace sentry_vision
{

CoordinateTransformer::CoordinateTransformer(const ParamManager & params)
: camera_params_(params.camera_params()),
  armor_real_height_(0.055f)
{
}

bool CoordinateTransformer::pixelToBody(const DetectionBox & det, int camera_id,
                                        float & x_out, float & y_out, float & z_out) const
{
    if (camera_id < 0 || camera_id >= static_cast<int>(camera_params_.size())) {
        return false;
    }
    if (camera_params_[camera_id].intrinsic.empty()) {
        return false;
    }

    // 1. 用框高度估算深度
    float depth = estimateDepth(det.height, camera_id);
    if (depth <= 0.0f || depth > 20.0f) {
        return false;
    }

    // 2. 像素 -> 相机系
    cv::Point3f cam_pt = pixelToCamera(det.x, det.y, depth, camera_id);

    // 3. 相机系 -> 车体系
    cv::Point3f body_pt = cameraToBody(cam_pt, camera_id);

    x_out = body_pt.x;
    y_out = body_pt.y;
    z_out = body_pt.z;
    return true;
}

float CoordinateTransformer::estimateDepth(float bbox_height, int camera_id) const
{
    // depth = fy * real_height / bbox_height_pixels
    const cv::Mat & K = camera_params_[camera_id].intrinsic;
    double fy = K.at<double>(1, 1);

    if (bbox_height < 1.0f) {
        return -1.0f;
    }
    return static_cast<float>(fy * armor_real_height_ / bbox_height);
}

cv::Point3f CoordinateTransformer::pixelToCamera(float px, float py, float depth,
                                                  int camera_id) const
{
    // P_cam = depth * K^{-1} * [px, py, 1]^T
    const cv::Mat & K = camera_params_[camera_id].intrinsic;
    double fx = K.at<double>(0, 0);
    double fy = K.at<double>(1, 1);
    double cx = K.at<double>(0, 2);
    double cy = K.at<double>(1, 2);

    float x = depth * static_cast<float>((px - cx) / fx);
    float y = depth * static_cast<float>((py - cy) / fy);
    float z = depth;
    return {x, y, z};
}

cv::Point3f CoordinateTransformer::cameraToBody(const cv::Point3f & cam_pt,
                                                 int camera_id) const
{
    const cv::Mat & T = camera_params_[camera_id].extrinsic;  // 4x4 cam->body

    // 如果外参是单位阵（未标定），直接返回相机系坐标
    cv::Mat p_cam = (cv::Mat_<double>(4, 1) <<
                     cam_pt.x, cam_pt.y, cam_pt.z, 1.0);
    cv::Mat p_body = T * p_cam;

    return {static_cast<float>(p_body.at<double>(0)),
            static_cast<float>(p_body.at<double>(1)),
            static_cast<float>(p_body.at<double>(2))};
}

}  // namespace sentry_vision
