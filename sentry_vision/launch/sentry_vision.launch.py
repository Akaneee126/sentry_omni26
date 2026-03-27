import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('sentry_vision')
    config_dir = os.path.join(pkg_dir, 'config')

    camera_yaml = os.path.join(config_dir, 'camera.yaml')
    ml_yaml = os.path.join(config_dir, 'ml.yaml')

    # ========== 方案二：一体化节点 ==========
    sentry_vision_node = Node(
        package='sentry_vision',
        executable='sentry_vision_node',
        name='sentry_vision_node',
        output='screen',
        parameters=[
            camera_yaml,
            ml_yaml,
            {'intrinsic_path': os.path.join(config_dir, 'Intrinsic.yaml')},
        ],
    )

    return LaunchDescription([
        sentry_vision_node,
    ])
