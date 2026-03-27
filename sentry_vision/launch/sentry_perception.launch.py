import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('sentry_vision')
    config_dir = os.path.join(pkg_dir, 'config')

    # 加载 yaml 参数文件
    camera_yaml = os.path.join(config_dir, 'camera.yaml')
    ml_yaml = os.path.join(config_dir, 'ml.yaml')

    # ========== 方案一：拆分节点 ==========
    armor_detector_node = Node(
        package='sentry_vision',
        executable='armor_detector_node',
        name='armor_detector_node',
        output='screen',
        parameters=[
            camera_yaml,
            ml_yaml,
            {'intrinsic_path': os.path.join(config_dir, 'Intrinsic.yaml')},
        ],
    )

    target_combine_node = Node(
        package='sentry_vision',
        executable='target_combine_node',
        name='target_combine_node',
        output='screen',
        parameters=[
            camera_yaml,
            ml_yaml,
            {'intrinsic_path': os.path.join(config_dir, 'Intrinsic.yaml')},
        ],
    )

    return LaunchDescription([
        armor_detector_node,
        target_combine_node,
    ])
