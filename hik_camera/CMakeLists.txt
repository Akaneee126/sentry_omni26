cmake_minimum_required(VERSION 3.8)
project(hik_camera)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# ==================== ROS2 依赖 ====================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)

set(ROS_DEPENDENCIES
  rclcpp
  sensor_msgs
  cv_bridge
  image_transport
  OpenCV
)

# ==================== 海康 MVS SDK ====================
set(HIK_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../hikrobot")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  set(HIK_LIB_SEARCH_PATHS "${HIK_LOCAL_DIR}/lib/arm64" "/opt/MVS/lib/64")
else()
  set(HIK_LIB_SEARCH_PATHS "${HIK_LOCAL_DIR}/lib/amd64" "/opt/MVS/lib/64")
endif()

find_library(MVS_LIB MvCameraControl PATHS ${HIK_LIB_SEARCH_PATHS})
if(NOT MVS_LIB)
  message(FATAL_ERROR "libMvCameraControl.so not found! Searched: ${HIK_LIB_SEARCH_PATHS}")
endif()

find_path(HIK_INCLUDE_DIR MvCameraControl.h PATHS "${HIK_LOCAL_DIR}/include" "/opt/MVS/include")
if(NOT HIK_INCLUDE_DIR)
  message(FATAL_ERROR "MvCameraControl.h not found!")
endif()

message(STATUS "Found MvCameraControl: ${MVS_LIB}")
message(STATUS "Found HIK headers: ${HIK_INCLUDE_DIR}")

# ==================== 节点可执行文件 ====================
add_executable(hik_camera_node
  src/hik_camera_node.cpp
)

target_include_directories(hik_camera_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${HIK_INCLUDE_DIR}
)

target_link_libraries(hik_camera_node ${MVS_LIB})
ament_target_dependencies(hik_camera_node ${ROS_DEPENDENCIES})

# ==================== 安装 ====================
install(TARGETS hik_camera_node
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

# 把 SDK .so 也装到 lib 下，这样 LD_LIBRARY_PATH 不需要额外设置
install(FILES ${MVS_LIB}
  DESTINATION lib
)

# SDK 内部会 dlopen 传输层库（在 libMvCameraControl.so 同目录查找）
get_filename_component(_mvs_lib_dir "${MVS_LIB}" DIRECTORY)
set(_MVS_EXTRA_LIBS
  "${_mvs_lib_dir}/libMvUsb3vTL.so"
  "${_mvs_lib_dir}/libMvProducerVIR.so"
  "${_mvs_lib_dir}/libMvSDKVersion.so"
  "${_mvs_lib_dir}/libMediaProcess.so"
)
foreach(_lib ${_MVS_EXTRA_LIBS})
  if(EXISTS "${_lib}")
    install(FILES "${_lib}" DESTINATION lib)
    message(STATUS "  Installing MVS extra lib: ${_lib}")
  else()
    message(WARNING "  MVS extra lib not found: ${_lib}")
  endif()
endforeach()

ament_package()
