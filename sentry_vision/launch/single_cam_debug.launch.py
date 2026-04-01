import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import TimerAction, SetEnvironmentVariable
from launch_ros.actions import Node


def generate_launch_description():
    """单相机调试模式：1 台相机 + 感知 + debug 可视化，一键启动。"""

    pkg_dir = get_package_share_directory('sentry_vision')
    config_dir = os.path.join(pkg_dir, 'config')

    camera_yaml = os.path.join(config_dir, 'camera.yaml')
    single_cam_yaml = os.path.join(config_dir, 'single_cam.yaml')

    common_params = [
        camera_yaml,
        single_cam_yaml,
        {'intrinsic_path': os.path.join(config_dir, 'Intrinsic.yaml')},
    ]

    # ========== 1. 海康相机驱动（不再需要，detector 直接开相机）==========
    # 如果 detector 的 HIK SDK 初始化失败, 它会自动回退到 ROS topic 订阅
    # 那时可以手动启动 hik_camera_node

    # ========== 2. 装甲板检测节点（单相机，直接通过 SDK 取图）==========
    armor_detector_node = Node(
        package='sentry_vision',
        executable='armor_detector_node',
        name='armor_detector_node',
        output='screen',
        parameters=common_params,
    )

    # ========== 3. 目标融合节点 ==========
    target_combine_node = Node(
        package='sentry_vision',
        executable='target_combine_node',
        name='target_combine_node',
        output='screen',
        parameters=common_params,
    )

    # ========== 4. 可视化调试节点 ==========
    debug_node = Node(
        package='sentry_vision',
        executable='sentry_debug_node',
        name='sentry_debug_node',
        output='screen',
        parameters=[
            camera_yaml,
            single_cam_yaml,
            {
                'show_windows': True,
                'bev_range': 10.0,
                'bev_size': 600,
                'camera_topics': ['/mvsua_cam/image_raw2'],
            },
        ],
    )

    # ========== 组装 ==========
    actions = []

    # 设置海康 MVS SDK 环境变量
    actions.append(SetEnvironmentVariable('MVCAM_COMMON_RUNENV', '/opt/MVS/lib/64'))
    actions.append(SetEnvironmentVariable('MVCAM_SDK_PATH', '/opt/MVS'))

    # 直接启动 detector（内部自己开相机，无需等待）
    actions.append(armor_detector_node)
    actions.append(target_combine_node)

    # 延迟 2 秒启动调试可视化（等 detector 先发布图像 topic）
    actions.append(TimerAction(
        period=2.0,
        actions=[debug_node],
    ))

    return LaunchDescription(actions)
