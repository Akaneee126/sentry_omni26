cmake_minimum_required(VERSION 3.8)
project(sentry_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ArmorDetection.msg"
  "msg/ArmorDetectionArray.msg"
  "msg/EnemyTarget.msg"
  "msg/EnemyTargetArray.msg"
  DEPENDENCIES std_msgs builtin_interfaces
)

ament_package()
