cmake_minimum_required(VERSION 3.8)
project(sentry_vision)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# ==================== 依赖 ====================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(sentry_interfaces REQUIRED)
find_package(OpenCV REQUIRED)

set(DEPENDENCIES
  rclcpp
  std_msgs
  sensor_msgs
  cv_bridge
  image_transport
  sentry_interfaces
  OpenCV
)

# ==================== 核心库 ====================
add_library(sentry_vision_core SHARED
  src/param_manager.cpp
  src/armor_detector.cpp
  src/multi_camera_buffer.cpp
  src/coordinate_transformer.cpp
  src/target_combine.cpp
)
target_include_directories(sentry_vision_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(sentry_vision_core ${DEPENDENCIES})

# ==================== 节点可执行文件 ====================

# armor_detector_node
add_executable(armor_detector_node
  src/node/armor_detector_node.cpp
)
target_link_libraries(armor_detector_node sentry_vision_core)
ament_target_dependencies(armor_detector_node ${DEPENDENCIES})

# target_combine_node
add_executable(target_combine_node
  src/node/target_combine_node.cpp
)
target_link_libraries(target_combine_node sentry_vision_core)
ament_target_dependencies(target_combine_node ${DEPENDENCIES})

# sentry_vision_node (一体化)
add_executable(sentry_vision_node
  src/node/sentry_vision_node.cpp
)
target_link_libraries(sentry_vision_node sentry_vision_core)
ament_target_dependencies(sentry_vision_node ${DEPENDENCIES})

# debug_node (可视化调试)
add_executable(sentry_debug_node
  src/node/debug_node.cpp
)
target_include_directories(sentry_debug_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(sentry_debug_node ${DEPENDENCIES})

# ==================== 安装 ====================
install(TARGETS
  sentry_vision_core
  armor_detector_node
  target_combine_node
  sentry_vision_node
  sentry_debug_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

ament_package()
