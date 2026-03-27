#ifndef SENTRY_VISION__TYPES_HPP_
#define SENTRY_VISION__TYPES_HPP_

#include <cstdint>
#include <vector>
#include <opencv2/core.hpp>

namespace sentry_vision
{

// 单个装甲板检测框（前端输出）
struct DetectionBox
{
    float x;           // bbox 中心 x（像素）
    float y;           // bbox 中心 y（像素）
    float width;       // bbox 宽（像素）
    float height;      // bbox 高（像素）
    float confidence;  // 置信度
    uint8_t color;     // 装甲板颜色  0=红 1=蓝
    uint8_t number;    // 装甲板数字  1~5, 前哨站=6, 基地=7
    int class_id;      // YOLO 原始类别 id
};

// 融合后的目标状态（后端输出）
struct TargetState
{
    uint8_t id;
    float x;
    float y;
    float z;
    float yaw;
    float confidence;
    bool is_tracking;
};

// 单个相机参数
struct CameraParam
{
    cv::Mat intrinsic;    // 3x3
    cv::Mat distortion;   // 5x1
    cv::Mat extrinsic;    // 4x4  cam -> body
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__TYPES_HPP_
