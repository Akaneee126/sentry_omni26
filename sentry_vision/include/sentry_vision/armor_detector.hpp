#ifndef SENTRY_VISION__ARMOR_DETECTOR_HPP_
#define SENTRY_VISION__ARMOR_DETECTOR_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <openvino/openvino.hpp>

#include "sentry_vision/types.hpp"

namespace sentry_vision
{

class ParamManager;  // 前向声明

class ArmorDetector
{
public:
    explicit ArmorDetector(const ParamManager & params);
    ~ArmorDetector() = default;

    /// 对一张图做推理，结果写入 detections
    /// @return 是否成功
    bool infer(const cv::Mat & image, int camera_id,
               std::vector<DetectionBox> & detections);

private:
    // ---------- 模型加载 ----------
    void loadModel(const std::string & model_path);

    // ---------- 前处理 ----------
    /// 把原图缩放/letterbox 到网络输入尺寸
    void preprocess(const cv::Mat & src, cv::Mat & blob);

    // ---------- 后处理 ----------
    /// YOLOv5 后处理 (kpt-first 布局)
    void postprocess_yolov5(const float * data, int num_boxes, int stride,
                            const cv::Size & img_size,
                            std::vector<DetectionBox> & detections);

    /// YOLOv8/11 后处理 (cx,cy,w,h-first 布局)
    void postprocess_yolov8(const float * data, int num_boxes, int stride,
                            int num_classes, int num_keypoints, int kpt_channels,
                            bool has_objectness, bool apply_sigmoid,
                            const cv::Size & img_size,
                            std::vector<DetectionBox> & detections);

    /// NMS
    void nms(std::vector<DetectionBox> & detections);

private:
    // OpenVINO
    ov::Core core_;
    ov::CompiledModel compiled_model_;
    ov::InferRequest infer_request_;

    int input_w_ = 640;
    int input_h_ = 640;

    // 阈值
    float conf_threshold_ = 0.8f;
    float nms_threshold_  = 0.45f;

    // 设备 (CPU / GPU)
    std::string device_ = "GPU";

    // YOLO 变体名称 (yolov5 / yolov8 / yolo11)
    std::string yolo_name_ = "yolov5";
    // 后处理格式（从yaml配置读取）
    int num_classes_ = 13;
    int num_keypoints_ = 4;   // 装甲板4角点
    int kpt_channels_ = 2;   // 2=(x,y)
    bool has_objectness_ = true; // YOLOv5=true, v8/v11=false
    bool apply_sigmoid_ = true;  // 输出是 raw logit 时需 sigmoid

    // YOLOv5 专用参数
    static constexpr int kNumColorClasses = 4;   // 蓝/红/灰/紫
    static constexpr int kNumNumberClasses = 9;  // 编号类别数

    // 是否已加载
    bool model_loaded_ = false;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__ARMOR_DETECTOR_HPP_
