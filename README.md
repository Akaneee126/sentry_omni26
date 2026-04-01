# sentry_omni26
烧饼宝宝的全向感知，（可惜代码能力是shit）


Adam哨兵全向感知工程

### 项目结构

```text
sentry_omni26/
├─ hik_camera/          # 海康相机驱动（多相机采集）
├─ sentry_interfaces/   # 自定义消息定义
├─ sentry_vision/       # 检测、融合、调试可视化
├─ sentry_bringup/      # 系统级启动编排
├─ assets/              # 模型文件（OpenVINO/ONNX）
└─ config/              # 参数配置（相机、内参、模型）
```
### 默认启动（拆分模式）
```
ros2 launch sentry_bringup bringup.launch.py
```

### 支持单相机测试模式
ros2 launch sentry_vision single_cam_debug.launch.py

### 支持调试模式
ros2 launch sentry_vision debug.launch.py

```

### 依赖
- ROS2
- OpenCV
- OpenVINO Runtime
- Hikrobot MVS SDK（海康工业相机）
