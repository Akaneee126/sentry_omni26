import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import TimerAction
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

    # ========== 1. 海康相机驱动（只开 1 路）==========
    try:
        hik_dir = get_package_share_directory('hik_camera')
        hik_config = os.path.join(hik_dir, 'config', 'hik_camera.yaml')
        hik_camera_node = Node(
            package='hik_camera',
            executable='hik_camera_node',
            name='hik_camera_node',
            output='screen',
            parameters=[
                hik_config,
                {
                    'camera_topics': ['/mvsua_cam/image_raw2'],  # 只发布 1 路
                },
            ],
        )
    except Exception:
        # 如果没装 hik_camera 包，跳过（用户可能手动发图像 topic）
        hik_camera_node = None

    # ========== 2. 装甲板检测节点（单相机）==========
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

    if hik_camera_node is not None:
        actions.append(hik_camera_node)

    # 延迟 2 秒启动感知，等相机就绪
    actions.append(TimerAction(
        period=2.0,
        actions=[armor_detector_node, target_combine_node],
    ))

    # 延迟 3 秒启动调试可视化
    actions.append(TimerAction(
        period=3.0,
        actions=[debug_node],
    ))

    return LaunchDescription(actions)
