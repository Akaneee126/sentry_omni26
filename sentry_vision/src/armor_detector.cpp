#include "sentry_vision/armor_detector.hpp"
#include "sentry_vision/param_manager.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

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
    yolo_name_ = params.yolo_name();
    num_classes_ = params.num_classes();
    num_keypoints_ = params.num_keypoints();
    kpt_channels_ = params.kpt_channels();
    has_objectness_ = params.has_objectness();
    apply_sigmoid_ = params.apply_sigmoid();
    device_ = params.device();

    loadModel(params.model_path());
}

// ============================================================
// 加载模型
// ============================================================
void ArmorDetector::loadModel(const std::string & model_path)
{
    try {
        auto model = core_.read_model(model_path);

        // 尝试使用配置设备，失败则回退 CPU
        std::string target_device = device_;
        try {
            compiled_model_ = core_.compile_model(model, target_device);
        } catch (const std::exception & e) {
            std::cerr << "[ArmorDetector] Failed to compile on " << target_device
                      << ": " << e.what() << std::endl;
            std::cerr << "[ArmorDetector] Falling back to CPU" << std::endl;
            target_device = "CPU";
            compiled_model_ = core_.compile_model(model, target_device);
        }
        infer_request_ = compiled_model_.create_infer_request();
        model_loaded_ = true;

        auto out_shape = model->output(0).get_partial_shape();
        std::cout << "[ArmorDetector] Model loaded (OpenVINO): " << model_path
                  << "  output_shape=" << out_shape
                  << "  device=" << target_device
                  << "  yolo_name=" << yolo_name_ << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "[ArmorDetector] Failed to load model: " << e.what() << std::endl;
        model_loaded_ = false;
    }
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
    if (!model_loaded_) {
        return false;
    }

    // 1. 前处理：图像 -> blob
    cv::Mat blob;
    preprocess(image, blob);

    // 2. 推理 (OpenVINO)
    auto input_tensor = infer_request_.get_input_tensor();
    // 注意：不要每帧调用 set_shape()，GPU 后端会触发显存重分配和内核重编译！
    // 模型输入 shape 固定为 [1,3,H,W]，编译时已确定。
    std::memcpy(input_tensor.data<float>(), blob.ptr<float>(),
                blob.total() * blob.elemSize());
    infer_request_.infer();

    auto output_tensor = infer_request_.get_output_tensor();
    const float * output_data = output_tensor.data<float>();
    auto output_shape = output_tensor.get_shape();
    // output_shape = [1, dim1, dim2]
    int dim1 = static_cast<int>(output_shape[1]);
    int dim2 = static_cast<int>(output_shape[2]);

    // YOLOv5:   [1, num_boxes, features]  features < num_boxes  (e.g. [1,25200,22])
    // YOLOv8/11: [1, features, num_boxes]  features < num_boxes  (e.g. [1,14,3549])
    bool transposed = (dim1 < dim2);

    int num_boxes, num_features;
    std::vector<float> transposed_buf;
    const float * data_ptr = output_data;

    if (transposed) {
        num_features = dim1;
        num_boxes = dim2;
        // 转置 [1, F, N] -> [N, F] 方便按行遍历
        transposed_buf.resize(static_cast<size_t>(num_boxes) * num_features);
        for (int f = 0; f < num_features; ++f) {
            for (int b = 0; b < num_boxes; ++b) {
                transposed_buf[b * num_features + f] =
                    output_data[f * num_boxes + b];
            }
        }
        data_ptr = transposed_buf.data();
    } else {
        num_boxes = dim1;
        num_features = dim2;
    }

    // 校验模型输出特征数
    int expected_features;
    if (yolo_name_ == "yolov5") {
        // YOLOv5 kpt-first 布局:
        // [kp0_x,kp0_y, kp1_x,kp1_y, kp2_x,kp2_y, kp3_x,kp3_y,
        //  obj_conf, color0-3, number0-8] = 8+1+4+9 = 22
        expected_features = num_keypoints_ * 2 + 1 + kNumColorClasses + kNumNumberClasses;
    } else {
        // YOLOv8/v11: [cx,cy,w,h,(obj),kp_data,cls]
        expected_features = (has_objectness_ ? 5 : 4) + num_keypoints_ * kpt_channels_ + num_classes_;
    }
    if (num_features != expected_features) {
        std::cerr << "[ArmorDetector] OUTPUT MISMATCH! model features="
                  << num_features << ", but config expects "
                  << expected_features
                  << " (yolo_name=" << yolo_name_ << ")"
                  << ". Please fix config in yaml!"
                  << std::endl;
        return false;
    }

    // 3. 后处理：网络输出 -> DetectionBox 列表
    if (yolo_name_ == "yolov5") {
        postprocess_yolov5(data_ptr, num_boxes, num_features,
                           image.size(), detections);
    } else {
        postprocess_yolov8(data_ptr, num_boxes, num_features, num_classes_,
                           num_keypoints_, kpt_channels_, has_objectness_,
                           apply_sigmoid_, image.size(), detections);
    }

    // 4. NMS 去重
    nms(detections);

    return true;
}

// ============================================================
// 前处理：resize + pad（左上对齐，黑色填充）
// ============================================================
void ArmorDetector::preprocess(const cv::Mat & src, cv::Mat & blob)
{
    cv::Mat input_img;

    if (src.cols == input_w_ && src.rows == input_h_) {
        // 输入已经是目标尺寸（如相机端已缩放），直接使用
        input_img = src;
    } else {
        // 等比缩放到 input_w_ × input_h_，左上角对齐，黑色填充
        float scale = std::min(
            static_cast<float>(input_w_) / src.cols,
            static_cast<float>(input_h_) / src.rows);
        int new_w = static_cast<int>(src.cols * scale);
        int new_h = static_cast<int>(src.rows * scale);

        cv::Mat resized;
        cv::resize(src, resized, cv::Size(new_w, new_h));

        cv::Mat canvas(input_h_, input_w_, CV_8UC3, cv::Scalar(0, 0, 0));
        resized.copyTo(canvas(cv::Rect(0, 0, new_w, new_h)));
        input_img = canvas;
    }

    // BGR -> RGB, /255, NCHW
    cv::dnn::blobFromImage(input_img, blob, 1.0 / 255.0,
                           cv::Size(input_w_, input_h_),
                           cv::Scalar(0, 0, 0), true, false);
}

// ============================================================
// 后处理辅助
// ============================================================
static inline float sigmoid(float x) {
    if (x > 0) return 1.0f / (1.0f + std::exp(-x));
    float ex = std::exp(x);
    return ex / (1.0f + ex);
}

// ============================================================
// YOLOv5 后处理（kpt-first 布局，参考 sp_vision_25）
//
// 每行 22 维:
//   [kp0_x, kp0_y, kp1_x, kp1_y, kp2_x, kp2_y, kp3_x, kp3_y,
//    obj_conf,
//    color0, color1, color2, color3,
//    number0 .. number8]
//
// - 位置 0~7 : 4 个关键点坐标（模型内部已 anchor+grid 解码，像素空间）
// - 位置 8   : objectness（raw logit → sigmoid）
// - 位置 9~12: 4 个颜色分类 logit（取 argmax）
// - 位置 13~21: 9 个编号分类 logit（取 argmax）
// - 没有 cx/cy/w/h，bbox 由关键点包围盒计算
// ============================================================
void ArmorDetector::postprocess_yolov5(const float * data, int num_boxes, int stride,
                                       const cv::Size & img_size,
                                       std::vector<DetectionBox> & detections)
{
    // 坐标还原：模型输出在 640×640 画布（左上对齐），需 / scale
    float scale = std::min(
        static_cast<float>(input_w_) / img_size.width,
        static_cast<float>(input_h_) / img_size.height);

    constexpr int kObjIdx = 8;
    constexpr int kColorOffset = 9;
    constexpr int kNumberOffset = 13;

    for (int i = 0; i < num_boxes; ++i) {
        const float * row = data + i * stride;

        // 1. objectness（raw logit → sigmoid）
        float obj_conf = sigmoid(row[kObjIdx]);
        if (obj_conf < conf_threshold_) {
            continue;
        }

        // 2. 找最大颜色分类
        int best_color = 0;
        float best_color_score = row[kColorOffset];
        for (int c = 1; c < kNumColorClasses; ++c) {
            if (row[kColorOffset + c] > best_color_score) {
                best_color_score = row[kColorOffset + c];
                best_color = c;
            }
        }

        // 3. 找最大编号分类
        int best_num = 0;
        float best_num_score = row[kNumberOffset];
        for (int c = 1; c < kNumNumberClasses; ++c) {
            if (row[kNumberOffset + c] > best_num_score) {
                best_num_score = row[kNumberOffset + c];
                best_num = c;
            }
        }

        // 4. 提取 4 个关键点（模型输出已在像素空间，只需还原 scale）
        //    顺序与 sp_vision_25 一致：[0]=(0,1), [1]=(6,7), [2]=(4,5), [3]=(2,3)
        std::vector<cv::Point2f> kps(4);
        kps[0] = cv::Point2f(row[0] / scale, row[1] / scale);
        kps[1] = cv::Point2f(row[6] / scale, row[7] / scale);
        kps[2] = cv::Point2f(row[4] / scale, row[5] / scale);
        kps[3] = cv::Point2f(row[2] / scale, row[3] / scale);

        // 5. 从关键点计算 bbox
        float min_x = kps[0].x, max_x = kps[0].x;
        float min_y = kps[0].y, max_y = kps[0].y;
        for (int k = 1; k < 4; ++k) {
            min_x = std::min(min_x, kps[k].x);
            max_x = std::max(max_x, kps[k].x);
            min_y = std::min(min_y, kps[k].y);
            max_y = std::max(max_y, kps[k].y);
        }

        DetectionBox det;
        det.x = (min_x + max_x) / 2.0f;
        det.y = (min_y + max_y) / 2.0f;
        det.width  = max_x - min_x;
        det.height = max_y - min_y;
        det.confidence = obj_conf;
        det.color = static_cast<uint8_t>(best_color);
        det.number = static_cast<uint8_t>(best_num);
        det.class_id = best_num;
        det.keypoints = std::move(kps);

        detections.push_back(det);
    }
}

// ============================================================
// YOLOv8/v11 后处理（标准 cx,cy,w,h-first 布局）
// ============================================================
void ArmorDetector::postprocess_yolov8(const float * data, int num_boxes, int stride,
                                       int num_classes, int num_keypoints, int kpt_channels,
                                       bool has_objectness, bool apply_sigmoid,
                                       const cv::Size & img_size,
                                       std::vector<DetectionBox> & detections)
{
    // 每行布局:
    //   YOLOv8: [cx, cy, w, h, kp_data..., cls0, cls1, ...]
    //   如果 has_objectness: [cx, cy, w, h, obj, kp_data..., cls0, cls1, ...]
    float scale = std::min(
        static_cast<float>(input_w_) / img_size.width,
        static_cast<float>(input_h_) / img_size.height);
    // 左上对齐，无 dx/dy

    int cls_offset = (has_objectness ? 5 : 4) + num_keypoints * kpt_channels;

    for (int i = 0; i < num_boxes; ++i) {
        const float * row = data + i * stride;

        float obj_conf = has_objectness ? row[4] : 1.0f;
        if (apply_sigmoid && has_objectness) {
            obj_conf = sigmoid(obj_conf);
        }
        if (obj_conf < conf_threshold_) {
            continue;
        }

        int best_cls = 0;
        float best_score = 0.0f;
        for (int c = 0; c < num_classes; ++c) {
            float s = row[cls_offset + c];
            if (apply_sigmoid) {
                s = sigmoid(s);
            }
            if (s > best_score) {
                best_score = s;
                best_cls = c;
            }
        }

        float final_conf = obj_conf * best_score;
        if (final_conf < conf_threshold_) {
            continue;
        }

        float cx = row[0] / scale;
        float cy = row[1] / scale;
        float w  = row[2] / scale;
        float h  = row[3] / scale;

        DetectionBox det;
        det.x = cx;
        det.y = cy;
        det.width = w;
        det.height = h;
        det.confidence = final_conf;
        det.class_id = best_cls;
        det.color = 0;
        det.number = static_cast<uint8_t>(best_cls);

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
    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    for (const auto & d : detections) {
        boxes.emplace_back(
            static_cast<int>(d.x - d.width / 2),
            static_cast<int>(d.y - d.height / 2),
            static_cast<int>(d.width),
            static_cast<int>(d.height));
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

}  // namespace sentry_vision
