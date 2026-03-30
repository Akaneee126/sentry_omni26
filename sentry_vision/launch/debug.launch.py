import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('sentry_vision')
    config_dir = os.path.join(pkg_dir, 'config')

    camera_yaml = os.path.join(config_dir, 'camera.yaml')
    ml_yaml = os.path.join(config_dir, 'ml.yaml')

    debug_node = Node(
        package='sentry_vision',
        executable='sentry_debug_node',
        name='sentry_debug_node',
        output='screen',
        parameters=[
            camera_yaml,
            ml_yaml,
            {
                'show_windows': True,   # 有显示器时 True，无头机设 False
                'bev_range': 10.0,      # 鸟瞰图显示范围(米)
                'bev_size': 600,        # 鸟瞰图画布大小(像素)
            },
        ],
    )

    return LaunchDescription([
        debug_node,
    ])
