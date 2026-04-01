// armor_detector_node.cpp
// 直接通过海康 SDK 取图 + 推理，绕过 ROS2 DDS 图像传输
// 同时可选发布低分辨率图给 debug 节点

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <cstdlib>
#include <cstring>

#ifdef HAS_HIK_SDK
#include "MvCameraControl.h"
#endif

#include "sentry_interfaces/msg/armor_detection.hpp"
#include "sentry_interfaces/msg/armor_detection_array.hpp"
#include "sentry_vision/param_manager.hpp"
#include "sentry_vision/armor_detector.hpp"

using namespace std::chrono_literals;

namespace sentry_vision
{

class ArmorDetectorNode : public rclcpp::Node
{
public:
    ArmorDetectorNode()
    : Node("armor_detector_node")
    {
    }

    ~ArmorDetectorNode() override
    {
#ifdef HAS_HIK_SDK
        stopCapture();
#endif
    }

    void init()
    {
        // 参数管理
        param_manager_ = std::make_shared<ParamManager>(shared_from_this());

        // 检测器
        detector_ = std::make_unique<ArmorDetector>(*param_manager_);

        // 发布检测结果
        det_pub_ = this->create_publisher<sentry_interfaces::msg::ArmorDetectionArray>(
            "/armor_detections", 10);

        // 为 debug 节点发布原图（低优先级，有订阅者才发）
        auto camera_topics = param_manager_->camera_topics();
        for (size_t i = 0; i < camera_topics.size(); ++i) {
            auto pub = this->create_publisher<sensor_msgs::msg::Image>(
                camera_topics[i], rclcpp::SensorDataQoS().keep_last(1));
            img_pubs_.push_back(pub);
        }

        // ------------ 直接开海康相机 ------------
        bool camera_ok = false;
#ifdef HAS_HIK_SDK
        this->declare_parameter<double>("exposure_time", 4000.0);
        this->declare_parameter<double>("gain", 10.0);
        this->declare_parameter<double>("frame_rate", 120.0);
        exposure_time_ = this->get_parameter("exposure_time").as_double();
        gain_ = this->get_parameter("gain").as_double();
        frame_rate_ = this->get_parameter("frame_rate").as_double();

        camera_ok = initCamera();
        if (!camera_ok) {
            RCLCPP_WARN(get_logger(), "Failed to init HIK camera, falling back to ROS topic subscription");
        }
#else
        RCLCPP_WARN(get_logger(), "Built without HIK SDK, using ROS topic subscription");
#endif
        if (!camera_ok) {
            auto qos = rclcpp::SensorDataQoS().keep_last(1);
            for (size_t i = 0; i < camera_topics.size(); ++i) {
                auto sub = this->create_subscription<sensor_msgs::msg::Image>(
                    camera_topics[i], qos,
                    [this, i](const sensor_msgs::msg::Image::SharedPtr msg) {
                        this->rosImageCallback(msg, static_cast<int>(i));
                    });
                image_subs_.push_back(sub);
                RCLCPP_INFO(get_logger(), "Fallback: subscribed to %s", camera_topics[i].c_str());
            }
        }

        RCLCPP_INFO(get_logger(), "ArmorDetectorNode started");
    }

private:
    // ============================================================
    // 海康 SDK 直接取图
    // ============================================================
#ifdef HAS_HIK_SDK
    bool initCamera()
    {
        setenv("MVCAM_COMMON_RUNENV", "/opt/MVS/lib/64", 1);
        setenv("MVCAM_SDK_PATH", "/opt/MVS", 1);

        RCLCPP_INFO(get_logger(), "HIK SDK init: MVCAM_COMMON_RUNENV=%s",
                    getenv("MVCAM_COMMON_RUNENV") ? getenv("MVCAM_COMMON_RUNENV") : "(null)");

        unsigned int ret = MV_CC_Initialize();
        if (ret != MV_OK) {
            RCLCPP_ERROR(get_logger(), "MV_CC_Initialize failed: 0x%x", ret);
            return false;
        }

        MV_CC_DEVICE_INFO_LIST dev_list;
        memset(&dev_list, 0, sizeof(dev_list));
        ret = MV_CC_EnumDevices(MV_USB_DEVICE, &dev_list);
        if (ret != MV_OK) {
            RCLCPP_ERROR(get_logger(), "MV_CC_EnumDevices failed: 0x%x", ret);
            return false;
        }
        if (dev_list.nDeviceNum == 0) {
            RCLCPP_ERROR(get_logger(), "MV_CC_EnumDevices ok but found 0 cameras. "
                         "Check USB permission or try: sudo chmod 666 /dev/bus/usb/*/*");
            return false;
        }

        int num_cameras = std::min(
            static_cast<int>(dev_list.nDeviceNum),
            static_cast<int>(param_manager_->camera_topics().size()));

        RCLCPP_INFO(get_logger(), "Found %d camera(s), opening %d",
                     static_cast<int>(dev_list.nDeviceNum), num_cameras);

        for (int i = 0; i < num_cameras; ++i) {
            void * handle = nullptr;
            ret = MV_CC_CreateHandle(&handle, dev_list.pDeviceInfo[i]);
            if (ret != MV_OK) continue;

            ret = MV_CC_OpenDevice(handle);
            if (ret != MV_OK) {
                MV_CC_DestroyHandle(handle);
                continue;
            }

            // 确保连续采集模式（非触发模式）
            MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_OFF);

            MV_CC_SetEnumValue(handle, "BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_CONTINUOUS);
            MV_CC_SetEnumValue(handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
            MV_CC_SetFloatValue(handle, "ExposureTime", static_cast<float>(exposure_time_));
            MV_CC_SetEnumValue(handle, "GainAuto", MV_GAIN_MODE_OFF);
            MV_CC_SetFloatValue(handle, "Gain", static_cast<float>(gain_));

            // 尝试设置 USB 相关传输参数（提升吞吐量）
            MV_CC_SetIntValue(handle, "DeviceLinkThroughputLimit", 400000000);  // 400MB/s

            MV_CC_SetBoolValue(handle, "AcquisitionFrameRateEnable", true);
            ret = MV_CC_SetFrameRate(handle, static_cast<float>(frame_rate_));
            RCLCPP_INFO(get_logger(), "SetFrameRate(%.0f) ret=0x%x", frame_rate_, ret);

            // 读回实际帧率
            MVCC_FLOATVALUE fval = {};
            if (MV_CC_GetFloatValue(handle, "AcquisitionFrameRate", &fval) == MV_OK) {
                RCLCPP_INFO(get_logger(), "Actual AcquisitionFrameRate: %.1f (max=%.1f)",
                            fval.fCurValue, fval.fMax);
            }
            // 读回结果帧率
            if (MV_CC_GetFloatValue(handle, "ResultingFrameRate", &fval) == MV_OK) {
                RCLCPP_INFO(get_logger(), "ResultingFrameRate: %.1f", fval.fCurValue);
            }
            // 读回实际曝光
            if (MV_CC_GetFloatValue(handle, "ExposureTime", &fval) == MV_OK) {
                RCLCPP_INFO(get_logger(), "Actual ExposureTime: %.0f us", fval.fCurValue);
            }

            ret = MV_CC_StartGrabbing(handle);
            if (ret != MV_OK) {
                MV_CC_CloseDevice(handle);
                MV_CC_DestroyHandle(handle);
                continue;
            }

            cam_handles_.push_back(handle);
            cam_running_.push_back(std::make_unique<std::atomic<bool>>(true));

            capture_threads_.emplace_back(
                &ArmorDetectorNode::captureAndInferLoop, this, i);

            RCLCPP_INFO(get_logger(), "Camera %d opened, capture+infer thread started", i);
        }

        return !cam_handles_.empty();
    }

    void captureAndInferLoop(int cam_index)
    {
        // HIK SDK (PFNC) 用 top-left 命名，OpenCV 4.x BayerXX 用 bottom-right 命名
        // → HIK BayerRG(RGGB) 要用 OpenCV COLOR_BayerBG2BGR(=BayerRGGB2BGR)
        static const std::unordered_map<MvGvspPixelType, cv::ColorConversionCodes> bayer_map = {
            {PixelType_Gvsp_BayerGR8, cv::COLOR_BayerGB2BGR},   // GRBG sensor
            {PixelType_Gvsp_BayerRG8, cv::COLOR_BayerBG2BGR},   // RGGB sensor
            {PixelType_Gvsp_BayerGB8, cv::COLOR_BayerGR2BGR},   // GBRG sensor
            {PixelType_Gvsp_BayerBG8, cv::COLOR_BayerRG2BGR},   // BGGR sensor
        };

        void * handle = cam_handles_[cam_index];
        MV_FRAME_OUT raw;
        memset(&raw, 0, sizeof(raw));

        int frame_count = 0;
        auto fps_start = std::chrono::steady_clock::now();

        while (rclcpp::ok() && cam_running_[cam_index]->load()) {
            auto t0 = std::chrono::steady_clock::now();

            unsigned int ret = MV_CC_GetImageBuffer(handle, &raw, 100);
            if (ret != MV_OK) {
                if (ret != MV_E_NODATA) {
                    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 3000,
                        "cam%d: GetImageBuffer failed: 0x%x", cam_index, ret);
                }
                std::this_thread::sleep_for(1ms);
                continue;
            }

            auto t1 = std::chrono::steady_clock::now();

            // Bayer -> BGR
            cv::Mat bayer(raw.stFrameInfo.nHeight, raw.stFrameInfo.nWidth,
                          CV_8UC1, raw.pBufAddr);
            cv::Mat bgr;
            auto it = bayer_map.find(raw.stFrameInfo.enPixelType);
            if (it != bayer_map.end()) {
                cv::cvtColor(bayer, bgr, it->second);
            } else if (raw.stFrameInfo.enPixelType == PixelType_Gvsp_Mono8) {
                cv::cvtColor(bayer, bgr, cv::COLOR_GRAY2BGR);
            } else {
                // 默认按 RGGB (海康常见 sensor)
                cv::cvtColor(bayer, bgr, cv::COLOR_BayerBG2BGR);
            }

            MV_CC_FreeImageBuffer(handle, &raw);

            auto t2 = std::chrono::steady_clock::now();

            // ======== 直接推理，零拷贝 ========
            std::vector<DetectionBox> detections;
            detector_->infer(bgr, cam_index, detections);

            auto t3 = std::chrono::steady_clock::now();

            // 发布检测结果（很小的消息，DDS 没问题）
            publishDetections(detections, cam_index);

            // 有 debug 订阅者时，每 N 帧发一次缩小后的图像，避免 DDS 拖慢推理
            if (cam_index < static_cast<int>(img_pubs_.size()) &&
                img_pubs_[cam_index]->get_subscription_count() > 0 &&
                frame_count % debug_every_n_ == 0) {
                cv::Mat small;
                cv::resize(bgr, small, cv::Size(640, 480));
                auto msg = cv_bridge::CvImage(
                    std_msgs::msg::Header(), "bgr8", small).toImageMsg();
                msg->header.stamp = now();
                msg->header.frame_id = "cam" + std::to_string(cam_index);
                img_pubs_[cam_index]->publish(*msg);
            }

            // FPS 统计
            frame_count++;
            if (frame_count % 100 == 0) {
                auto now_t = std::chrono::steady_clock::now();
                double elapsed = std::chrono::duration<double>(now_t - fps_start).count();
                double fps = frame_count / elapsed;

                // 上一帧各阶段耗时
                double ms_grab = std::chrono::duration<double, std::milli>(t1 - t0).count();
                double ms_cvt  = std::chrono::duration<double, std::milli>(t2 - t1).count();
                double ms_infer = std::chrono::duration<double, std::milli>(t3 - t2).count();
                RCLCPP_INFO(get_logger(),
                    "cam%d: %.1f FPS | grab=%.1fms cvt=%.1fms infer=%.1fms | %zu dets",
                    cam_index, fps, ms_grab, ms_cvt, ms_infer, detections.size());
                frame_count = 0;
                fps_start = now_t;
            }
        }
    }
#endif  // HAS_HIK_SDK

    void publishDetections(const std::vector<DetectionBox> & detections, int cam_id)
    {
        sentry_interfaces::msg::ArmorDetectionArray out_msg;
        out_msg.header.stamp = now();
        out_msg.header.frame_id = "cam" + std::to_string(cam_id);
        out_msg.camera_id = static_cast<uint8_t>(cam_id);

        for (const auto & det : detections) {
            sentry_interfaces::msg::ArmorDetection item;
            item.header = out_msg.header;
            item.camera_id = out_msg.camera_id;
            item.target_color = det.color;
            item.target_number = det.number;
            item.confidence = det.confidence;
            item.x = det.x;
            item.y = det.y;
            item.width = det.width;
            item.height = det.height;
            out_msg.detections.push_back(item);
        }

        det_pub_->publish(out_msg);
    }

    // ============================================================
    // ROS topic 回退（相机打不开时用）
    // ============================================================
    void rosImageCallback(const sensor_msgs::msg::Image::SharedPtr msg, int camera_id)
    {
        cv::Mat image;
        try {
            auto cv_ptr = cv_bridge::toCvShare(msg, "bgr8");
            image = cv_ptr->image;
        } catch (const cv_bridge::Exception & e) {
            RCLCPP_ERROR(get_logger(), "cv_bridge error: %s", e.what());
            return;
        }

        std::vector<DetectionBox> detections;
        detector_->infer(image, camera_id, detections);
        publishDetections(detections, camera_id);
    }

#ifdef HAS_HIK_SDK
    void stopCapture()
    {
        for (auto & running : cam_running_) {
            running->store(false);
        }
        for (auto & t : capture_threads_) {
            if (t.joinable()) t.join();
        }
        for (auto * h : cam_handles_) {
            if (h) {
                MV_CC_StopGrabbing(h);
                MV_CC_CloseDevice(h);
                MV_CC_DestroyHandle(h);
            }
        }
        cam_handles_.clear();
        cam_running_.clear();
        MV_CC_Finalize();
    }
#endif

    std::shared_ptr<ParamManager> param_manager_;
    std::unique_ptr<ArmorDetector> detector_;
    rclcpp::Publisher<sentry_interfaces::msg::ArmorDetectionArray>::SharedPtr det_pub_;
    std::vector<rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr> img_pubs_;
    std::vector<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr> image_subs_;
    int debug_every_n_ = 3;  // debug 图像每 N 帧发一次 (165/3≈55fps)

#ifdef HAS_HIK_SDK
    std::vector<void *> cam_handles_;
    std::vector<std::unique_ptr<std::atomic<bool>>> cam_running_;
    std::vector<std::thread> capture_threads_;
    double exposure_time_ = 4000.0;
    double gain_ = 10.0;
    double frame_rate_ = 120.0;
#endif
};

}  // namespace sentry_vision

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<sentry_vision::ArmorDetectorNode>();
    node->init();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
