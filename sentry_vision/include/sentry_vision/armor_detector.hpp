#ifndef SENTRY_VISION__ARMOR_DETECTOR_HPP_
#define SENTRY_VISION__ARMOR_DETECTOR_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

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
    /// 解析网络输出 -> 原始框
    void postprocess(const cv::Mat & output, const cv::Size & img_size,
                     std::vector<DetectionBox> & detections);

    /// NMS
    void nms(std::vector<DetectionBox> & detections);

    // ---------- 工具 ----------
    /// 把 YOLO class_id 拆成 color + number
    static void decodeClassId(int class_id, uint8_t & color, uint8_t & number);

private:
    // 模型
    cv::dnn::Net net_;
    int input_w_ = 640;
    int input_h_ = 640;

    // 阈值
    float conf_threshold_ = 0.8f;
    float nms_threshold_  = 0.45f;

    // 类别数（RM 装甲板一般 2色 × 8号 = 16 类，按你的训练集改）
    int num_classes_ = 16;

    // 是否已加载
    bool model_loaded_ = false;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__ARMOR_DETECTOR_HPP_
