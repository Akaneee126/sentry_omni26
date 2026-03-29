import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    hik_camera_dir = get_package_share_directory('hik_camera')
    sentry_vision_dir = get_package_share_directory('sentry_vision')

    # ========== 1. 海康相机驱动 ==========
    camera_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(hik_camera_dir, 'launch', 'hik_camera.launch.py')
        )
    )

    # ========== 2. 感知节点（延迟 2 秒等相机就绪）==========
    # 你可以选择两种启动方式：
    # 方案一：拆分节点（armor_detector_node + target_combine_node）
    perception_launch = TimerAction(
        period=2.0,
        actions=[
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    os.path.join(sentry_vision_dir, 'launch', 'sentry_perception.launch.py')
                )
            )
        ],
    )

    # 方案二：一体化节点
    # perception_launch = TimerAction(
    #     period=2.0,
    #     actions=[
    #         IncludeLaunchDescription(
    #             PythonLaunchDescriptionSource(
    #                 os.path.join(sentry_vision_dir, 'launch', 'sentry_vision.launch.py')
    #             )
    #         )
    #     ],
    # )

    return LaunchDescription([
        camera_launch,
        perception_launch,
    ])
