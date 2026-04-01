import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    hik_camera_dir = get_package_share_directory('hik_camera')
    sentry_vision_dir = get_package_share_directory('sentry_vision')

    # ========== 启动参数 ==========
    debug_arg = DeclareLaunchArgument(
        'debug', default_value='false',
        description='Launch debug visualization node (true/false)'
    )
    single_cam_arg = DeclareLaunchArgument(
        'single_cam', default_value='false',
        description='Single camera debug mode (true/false)'
    )

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

    # ========== 3. 可视化调试节点（可选）==========
    debug_launch = TimerAction(
        period=3.0,
        actions=[
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    os.path.join(sentry_vision_dir, 'launch', 'debug.launch.py')
                ),
                condition=IfCondition(LaunchConfiguration('debug')),
            )
        ],
    )

    # ========== 4. 单相机调试模式（可选，与上面互斥）==========
    single_cam_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(sentry_vision_dir, 'launch', 'single_cam_debug.launch.py')
        ),
        condition=IfCondition(LaunchConfiguration('single_cam')),
    )

    return LaunchDescription([
        debug_arg,
        single_cam_arg,
        camera_launch,
        perception_launch,
        debug_launch,
        single_cam_launch,
    ])
