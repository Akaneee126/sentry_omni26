// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__BUILDER_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sentry_interfaces/msg/detail/armor_detection_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sentry_interfaces
{

namespace msg
{

namespace builder
{

class Init_ArmorDetectionArray_detections
{
public:
  explicit Init_ArmorDetectionArray_detections(::sentry_interfaces::msg::ArmorDetectionArray & msg)
  : msg_(msg)
  {}
  ::sentry_interfaces::msg::ArmorDetectionArray detections(::sentry_interfaces::msg::ArmorDetectionArray::_detections_type arg)
  {
    msg_.detections = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetectionArray msg_;
};

class Init_ArmorDetectionArray_camera_id
{
public:
  explicit Init_ArmorDetectionArray_camera_id(::sentry_interfaces::msg::ArmorDetectionArray & msg)
  : msg_(msg)
  {}
  Init_ArmorDetectionArray_detections camera_id(::sentry_interfaces::msg::ArmorDetectionArray::_camera_id_type arg)
  {
    msg_.camera_id = std::move(arg);
    return Init_ArmorDetectionArray_detections(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetectionArray msg_;
};

class Init_ArmorDetectionArray_header
{
public:
  Init_ArmorDetectionArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ArmorDetectionArray_camera_id header(::sentry_interfaces::msg::ArmorDetectionArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ArmorDetectionArray_camera_id(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetectionArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sentry_interfaces::msg::ArmorDetectionArray>()
{
  return sentry_interfaces::msg::builder::Init_ArmorDetectionArray_header();
}

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__BUILDER_HPP_
