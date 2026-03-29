// hik_camera_node.cpp —— 海康多相机 ROS2 驱动节点
// 同时打开最多 N 台 USB 工业相机，每台发布一路 sensor_msgs/Image

#include "hik_camera/hik_camera_node.hpp"

#include <chrono>
#include <functional>
#include <unordered_map>

using namespace std::chrono_literals;

namespace hik_camera
{

// ============================================================
// 构造 / 析构
// ============================================================
HikCameraNode::HikCameraNode()
: Node("hik_camera_node")
{
    declareParams();

    // ---------- 枚举设备 ----------
    MV_CC_DEVICE_INFO_LIST device_list;
    memset(&device_list, 0, sizeof(device_list));

    unsigned int ret = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    if (ret != MV_OK) {
        RCLCPP_FATAL(get_logger(), "MV_CC_EnumDevices failed: 0x%x", ret);
        return;
    }

    int num_devices = static_cast<int>(device_list.nDeviceNum);
    RCLCPP_INFO(get_logger(), "Found %d Hikvision USB camera(s)", num_devices);

    if (num_devices == 0) {
        RCLCPP_ERROR(get_logger(), "No camera found! Node will idle.");
        return;
    }

    int num_to_open = std::min(num_devices, static_cast<int>(topic_names_.size()));

    cameras_.resize(num_to_open);

    for (int i = 0; i < num_to_open; ++i) {
        auto & cam = cameras_[i];
        cam.device_index = i;
        cam.topic_name = topic_names_[i];
        cam.frame_id = "cam" + std::to_string(i);

        // 创建 publisher
        cam.publisher = create_publisher<sensor_msgs::msg::Image>(cam.topic_name, 10);

        // 打开相机
        if (!openCamera(cam, device_list.pDeviceInfo[i])) {
            RCLCPP_ERROR(get_logger(), "Failed to open camera %d", i);
            continue;
        }

        // 启动采集线程
        cam.running = true;
        cam.capture_thread = std::thread(&HikCameraNode::captureLoop, this, i);

        RCLCPP_INFO(get_logger(), "Camera %d -> topic: %s", i, cam.topic_name.c_str());
    }

    RCLCPP_INFO(get_logger(), "HikCameraNode started with %d camera(s)", num_to_open);
}

HikCameraNode::~HikCameraNode()
{
    stopAll();
    RCLCPP_INFO(get_logger(), "HikCameraNode destroyed");
}

// ============================================================
// 参数声明
// ============================================================
void HikCameraNode::declareParams()
{
    // 相机硬件参数
    declare_parameter<double>("exposure_time", 500.0);
    declare_parameter<double>("gain", 10.0);
    declare_parameter<double>("frame_rate", 120.0);
    declare_parameter<int>("image_width", 1280);
    declare_parameter<int>("image_height", 720);

    exposure_time_ = get_parameter("exposure_time").as_double();
    gain_ = get_parameter("gain").as_double();
    frame_rate_ = get_parameter("frame_rate").as_double();
    image_width_ = get_parameter("image_width").as_int();
    image_height_ = get_parameter("image_height").as_int();

    // 发布的 topic 列表（与 sentry_vision 订阅端对齐）
    declare_parameter<std::vector<std::string>>("camera_topics", {
        "/mvsua_cam/image_raw2",
        "/mvsua_cam/image_raw3",
        "/mvsua_cam/image_raw4"
    });
    topic_names_ = get_parameter("camera_topics").as_string_array();

    RCLCPP_INFO(get_logger(),
        "Params: exposure=%.0fus, gain=%.1f, fps=%.0f, %dx%d, %zu topics",
        exposure_time_, gain_, frame_rate_, image_width_, image_height_,
        topic_names_.size());
}

// ============================================================
// 打开单台相机并配置参数
// ============================================================
bool HikCameraNode::openCamera(CameraInstance & cam, MV_CC_DEVICE_INFO * device_info)
{
    unsigned int ret;

    ret = MV_CC_CreateHandle(&cam.handle, device_info);
    if (ret != MV_OK) {
        RCLCPP_ERROR(get_logger(), "cam%d: MV_CC_CreateHandle failed: 0x%x",
                     cam.device_index, ret);
        return false;
    }

    ret = MV_CC_OpenDevice(cam.handle);
    if (ret != MV_OK) {
        RCLCPP_ERROR(get_logger(), "cam%d: MV_CC_OpenDevice failed: 0x%x",
                     cam.device_index, ret);
        MV_CC_DestroyHandle(cam.handle);
        cam.handle = nullptr;
        return false;
    }

    // 白平衡自动
    MV_CC_SetEnumValue(cam.handle, "BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_CONTINUOUS);
    // 手动曝光
    MV_CC_SetEnumValue(cam.handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    MV_CC_SetFloatValue(cam.handle, "ExposureTime", static_cast<float>(exposure_time_));
    // 手动增益
    MV_CC_SetEnumValue(cam.handle, "GainAuto", MV_GAIN_MODE_OFF);
    MV_CC_SetFloatValue(cam.handle, "Gain", static_cast<float>(gain_));
    // 帧率
    MV_CC_SetFrameRate(cam.handle, static_cast<float>(frame_rate_));

    // 开始采集
    ret = MV_CC_StartGrabbing(cam.handle);
    if (ret != MV_OK) {
        RCLCPP_ERROR(get_logger(), "cam%d: MV_CC_StartGrabbing failed: 0x%x",
                     cam.device_index, ret);
        MV_CC_CloseDevice(cam.handle);
        MV_CC_DestroyHandle(cam.handle);
        cam.handle = nullptr;
        return false;
    }

    return true;
}

// ============================================================
// 采集循环（每台相机一个线程）
// ============================================================
void HikCameraNode::captureLoop(int cam_index)
{
    auto & cam = cameras_[cam_index];

    // Bayer → BGR 转换表
    static const std::unordered_map<MvGvspPixelType, cv::ColorConversionCodes> bayer_map = {
        {PixelType_Gvsp_BayerGR8, cv::COLOR_BayerGR2BGR},
        {PixelType_Gvsp_BayerRG8, cv::COLOR_BayerRG2BGR},
        {PixelType_Gvsp_BayerGB8, cv::COLOR_BayerGB2BGR},
        {PixelType_Gvsp_BayerBG8, cv::COLOR_BayerBG2BGR},
    };

    RCLCPP_INFO(get_logger(), "cam%d: capture thread started", cam_index);

    MV_FRAME_OUT raw;
    memset(&raw, 0, sizeof(raw));

    while (rclcpp::ok() && cam.running) {
        unsigned int ret = MV_CC_GetImageBuffer(cam.handle, &raw, 100);
        if (ret != MV_OK) {
            // 超时属于正常（短暂无帧），不刷屏
            if (ret != MV_E_NODATA) {
                RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 3000,
                    "cam%d: MV_CC_GetImageBuffer failed: 0x%x", cam_index, ret);
            }
            std::this_thread::sleep_for(1ms);
            continue;
        }

        // 原始 Bayer 灰度图
        cv::Mat bayer(raw.stFrameInfo.nHeight, raw.stFrameInfo.nWidth,
                      CV_8UC1, raw.pBufAddr);

        // 转 BGR
        cv::Mat bgr;
        auto it = bayer_map.find(raw.stFrameInfo.enPixelType);
        if (it != bayer_map.end()) {
            cv::cvtColor(bayer, bgr, it->second);
        } else if (raw.stFrameInfo.enPixelType == PixelType_Gvsp_Mono8) {
            cv::cvtColor(bayer, bgr, cv::COLOR_GRAY2BGR);
        } else {
            // 未知格式，强制当 BayerRG 处理
            cv::cvtColor(bayer, bgr, cv::COLOR_BayerRG2BGR);
        }

        // 发布 ROS2 Image 消息
        auto msg = cv_bridge::CvImage(
            std_msgs::msg::Header(), "bgr8", bgr).toImageMsg();
        msg->header.stamp = now();
        msg->header.frame_id = cam.frame_id;

        cam.publisher->publish(*msg);

        // 释放 SDK 缓冲区
        MV_CC_FreeImageBuffer(cam.handle, &raw);
    }

    RCLCPP_INFO(get_logger(), "cam%d: capture thread stopped", cam_index);
}

// ============================================================
// 关闭所有相机
// ============================================================
void HikCameraNode::stopAll()
{
    for (auto & cam : cameras_) {
        cam.running = false;
        if (cam.capture_thread.joinable()) {
            cam.capture_thread.join();
        }
        if (cam.handle) {
            MV_CC_StopGrabbing(cam.handle);
            MV_CC_CloseDevice(cam.handle);
            MV_CC_DestroyHandle(cam.handle);
            cam.handle = nullptr;
        }
    }
}

}  // namespace hik_camera

// ============================================================
// main
// ============================================================
int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<hik_camera::HikCameraNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
