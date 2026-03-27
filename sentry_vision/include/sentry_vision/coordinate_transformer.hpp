#ifndef SENTRY_VISION__COORDINATE_TRANSFORMER_HPP_
#define SENTRY_VISION__COORDINATE_TRANSFORMER_HPP_

#include <opencv2/opencv.hpp>
#include "sentry_vision/types.hpp"

namespace sentry_vision
{

class ParamManager;

class CoordinateTransformer
{
public:
    explicit CoordinateTransformer(const ParamManager & params);

    /// 从像素坐标 + 框高估距，计算车体系下的 3D 坐标
    /// @param det       像素检测框
    /// @param camera_id 相机 id（用于选择内参/外参）
    /// @param x_out, y_out, z_out  车体系坐标 (前/左/上 或你自己的定义)
    bool pixelToBody(const DetectionBox & det, int camera_id,
                     float & x_out, float & y_out, float & z_out) const;

private:
    /// 利用装甲板框高度估计深度
    float estimateDepth(float bbox_height, int camera_id) const;

    /// 像素坐标 -> 相机系 3D
    cv::Point3f pixelToCamera(float px, float py, float depth, int camera_id) const;

    /// 相机系 -> 车体系
    cv::Point3f cameraToBody(const cv::Point3f & cam_pt, int camera_id) const;

    std::vector<CameraParam> camera_params_;

    // 装甲板真实高度（小装甲板 55mm, 大装甲板 127mm, 先用一个统一值）
    float armor_real_height_ = 0.055f;  // 单位：米
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__COORDINATE_TRANSFORMER_HPP_
