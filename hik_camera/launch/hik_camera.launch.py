import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('hik_camera')
    config_file = os.path.join(pkg_dir, 'config', 'hik_camera.yaml')

    set_mvcam_env = SetEnvironmentVariable('MVCAM_COMMON_RUNENV', '/opt/MVS/lib/64')

    hik_camera_node = Node(
        package='hik_camera',
        executable='hik_camera_node',
        name='hik_camera_node',
        output='screen',
        parameters=[config_file],
    )

    return LaunchDescription([
        set_mvcam_env,
        hik_camera_node,
    ])
