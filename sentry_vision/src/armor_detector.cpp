#include "sentry_vision/armor_detector.hpp"
#include "sentry_vision/param_manager.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>

namespace sentry_vision
{

// ============================================================
// 构造：从 ParamManager 拿参数
// ============================================================
ArmorDetector::ArmorDetector(const ParamManager & params)
{
    conf_threshold_ = params.conf_threshold();
    nms_threshold_  = params.nms_threshold();
    input_w_ = params.input_width();
    input_h_ = params.input_height();

    loadModel(params.model_path());
}

// ============================================================
// 加载模型
// ============================================================
void ArmorDetector::loadModel(const std::string & model_path)
{
    // --- 方案 A：用 OpenCV DNN 加载 ONNX ---
    //  net_ = cv::dnn::readNetFromONNX(model_path);

    // --- 方案 B：用 OpenCV DNN 加载 OpenVINO IR（.xml + .bin）---
     net_ = cv::dnn::readNet(model_path);          // .xml 路径
     net_.setPreferableBackend(cv::dnn::DNN_BACKEND_INFERENCE_ENGINE);
     net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);  // 或 DNN_TARGET_OPENCL

    // --- 方案 C：直接调 OpenVINO C++ API ---
    // 如果你要用 TensorRT / OpenVINO 原生 API，这里替换成对应的加载代码。
    // 参考 OpenVINO：ov::Core core; auto model = core.read_model(xml); ...

    // TODO: 选一种方案取消注释并补全
    // 加载成功后设置标志
    // model_loaded_ = true;

    std::cout << "[ArmorDetector] loadModel: " << model_path << std::endl;
}

// ============================================================
// 主推理入口
// ============================================================
bool ArmorDetector::infer(const cv::Mat & image, int camera_id,
                          std::vector<DetectionBox> & detections)
{
    detections.clear();

    if (image.empty()) {
        return false;
    }
    // if (!model_loaded_) { return false; }

    // 1. 前处理：图像 -> blob
    cv::Mat blob;
    preprocess(image, blob);

    // 2. 推理
    // net_.setInput(blob);
    // cv::Mat output = net_.forward();
    // 如果你的网络有多个输出头，用 net_.forward(output_names) 获取

    // 3. 后处理：网络输出 -> DetectionBox 列表
    // postprocess(output, image.size(), detections);

    // 4. NMS 去重
    // nms(detections);

    // ---------- 临时：先返回空结果让节点能跑通 ----------
    std::cout << "[ArmorDetector] infer cam=" << camera_id
              << "  img=" << image.cols << "x" << image.rows
              << std::endl;

    return true;
}

// ============================================================
// 前处理：letterbox + normalize + blob
// ============================================================
void ArmorDetector::preprocess(const cv::Mat & src, cv::Mat & blob)
{
    // YOLOv5/v8 标准前处理流程：
    //
    // 1) letterbox 缩放——保持宽高比，空白填充 114
    //    计算缩放比例
    float scale = std::min(
        static_cast<float>(input_w_) / src.cols,
        static_cast<float>(input_h_) / src.rows);
    int new_w = static_cast<int>(src.cols * scale);
    int new_h = static_cast<int>(src.rows * scale);

    cv::Mat resized;
    cv::resize(src, resized, cv::Size(new_w, new_h));

    //    创建 letterbox 画布
    cv::Mat canvas(input_h_, input_w_, CV_8UC3, cv::Scalar(114, 114, 114));
    int dx = (input_w_ - new_w) / 2;
    int dy = (input_h_ - new_h) / 2;
    resized.copyTo(canvas(cv::Rect(dx, dy, new_w, new_h)));

    // 2) BGR -> RGB,归一化到 [0, 1],转成 NCHW blob
    cv::dnn::blobFromImage(canvas, blob, 1.0 / 255.0,
                           cv::Size(input_w_, input_h_),
                           cv::Scalar(0, 0, 0), true, false);
}

// ============================================================
// 后处理：解析网络输出
// ============================================================
void ArmorDetector::postprocess(const cv::Mat & output,
                                const cv::Size & img_size,
                                std::vector<DetectionBox> & detections)
{
    // ----- 这里的解析逻辑取决于你用的 YOLO 版本 -----
    //
    // YOLOv5 输出格式 (batch=1)：
    //   shape = [1, num_detections, 5 + num_classes]
    //   每行 = [cx, cy, w, h, obj_conf, class0_conf, class1_conf, ...]
    //
    // YOLOv8/v11 输出格式 (batch=1)：
    //   shape = [1, 4 + num_classes, num_detections]  (需要转置)
    //   每列 = [cx, cy, w, h, class0_conf, class1_conf, ...]
    //   没有单独的 obj_conf
    //
    // TODO: 根据你使用的模型版本，选择对应的解析方式
    //       下面以 YOLOv5 格式为例

    // letterbox 还原参数
    float scale = std::min(
        static_cast<float>(input_w_) / img_size.width,
        static_cast<float>(input_h_) / img_size.height);
    float dx = (input_w_ - img_size.width * scale) / 2.0f;
    float dy = (input_h_ - img_size.height * scale) / 2.0f;

    // 解析每一行
    const int cols = 5 + num_classes_;
    const float * data = reinterpret_cast<const float *>(output.data);
    int rows = output.size[1];  // num_detections

    for (int i = 0; i < rows; ++i) {
        const float * row = data + i * cols;
        float obj_conf = row[4];
        if (obj_conf < conf_threshold_) {
            continue;
        }

        // 找最大类别分数
        int best_cls = 0;
        float best_score = 0.0f;
        for (int c = 0; c < num_classes_; ++c) {
            float s = row[5 + c];
            if (s > best_score) {
                best_score = s;
                best_cls = c;
            }
        }

        float final_conf = obj_conf * best_score;
        if (final_conf < conf_threshold_) {
            continue;
        }

        // 还原到原图像素坐标
        float cx = (row[0] - dx) / scale;
        float cy = (row[1] - dy) / scale;
        float w  = row[2] / scale;
        float h  = row[3] / scale;

        DetectionBox det;
        det.x = cx;
        det.y = cy;
        det.width = w;
        det.height = h;
        det.confidence = final_conf;
        det.class_id = best_cls;
        decodeClassId(best_cls, det.color, det.number);

        detections.push_back(det);
    }
}

// ============================================================
// NMS
// ============================================================
void ArmorDetector::nms(std::vector<DetectionBox> & detections)
{
    if (detections.empty()) return;

    // 转成 OpenCV NMS 需要的格式
    std::vector<cv::Rect2f> boxes;
    std::vector<float> scores;
    for (const auto & d : detections) {
        boxes.emplace_back(d.x - d.width / 2, d.y - d.height / 2,
                           d.width, d.height);
        scores.push_back(d.confidence);
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, scores, conf_threshold_, nms_threshold_, indices);

    std::vector<DetectionBox> result;
    result.reserve(indices.size());
    for (int idx : indices) {
        result.push_back(detections[idx]);
    }
    detections = std::move(result);
}

// ============================================================
// 把 YOLO class_id 拆成颜色 + 编号
// ============================================================
void ArmorDetector::decodeClassId(int class_id, uint8_t & color, uint8_t & number)
{
    // TODO: 根据你训练 YOLO 时的类别映射来改
    // 常见做法：class_id = color * 8 + number
    // 例如 0~7 = 红色 1~基地,  8~15 = 蓝色 1~基地
    color  = static_cast<uint8_t>(class_id / 8);   // 0=红, 1=蓝
    number = static_cast<uint8_t>(class_id % 8 + 1);
}

}  // namespace sentry_vision
