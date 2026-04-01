// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sentry_interfaces:msg/ArmorDetection.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__BUILDER_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sentry_interfaces/msg/detail/armor_detection__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sentry_interfaces
{

namespace msg
{

namespace builder
{

class Init_ArmorDetection_distance
{
public:
  explicit Init_ArmorDetection_distance(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  ::sentry_interfaces::msg::ArmorDetection distance(::sentry_interfaces::msg::ArmorDetection::_distance_type arg)
  {
    msg_.distance = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_height
{
public:
  explicit Init_ArmorDetection_height(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_distance height(::sentry_interfaces::msg::ArmorDetection::_height_type arg)
  {
    msg_.height = std::move(arg);
    return Init_ArmorDetection_distance(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_width
{
public:
  explicit Init_ArmorDetection_width(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_height width(::sentry_interfaces::msg::ArmorDetection::_width_type arg)
  {
    msg_.width = std::move(arg);
    return Init_ArmorDetection_height(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_y
{
public:
  explicit Init_ArmorDetection_y(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_width y(::sentry_interfaces::msg::ArmorDetection::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_ArmorDetection_width(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_x
{
public:
  explicit Init_ArmorDetection_x(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_y x(::sentry_interfaces::msg::ArmorDetection::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_ArmorDetection_y(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_confidence
{
public:
  explicit Init_ArmorDetection_confidence(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_x confidence(::sentry_interfaces::msg::ArmorDetection::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_ArmorDetection_x(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_target_number
{
public:
  explicit Init_ArmorDetection_target_number(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_confidence target_number(::sentry_interfaces::msg::ArmorDetection::_target_number_type arg)
  {
    msg_.target_number = std::move(arg);
    return Init_ArmorDetection_confidence(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_target_color
{
public:
  explicit Init_ArmorDetection_target_color(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_target_number target_color(::sentry_interfaces::msg::ArmorDetection::_target_color_type arg)
  {
    msg_.target_color = std::move(arg);
    return Init_ArmorDetection_target_number(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_camera_id
{
public:
  explicit Init_ArmorDetection_camera_id(::sentry_interfaces::msg::ArmorDetection & msg)
  : msg_(msg)
  {}
  Init_ArmorDetection_target_color camera_id(::sentry_interfaces::msg::ArmorDetection::_camera_id_type arg)
  {
    msg_.camera_id = std::move(arg);
    return Init_ArmorDetection_target_color(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

class Init_ArmorDetection_header
{
public:
  Init_ArmorDetection_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ArmorDetection_camera_id header(::sentry_interfaces::msg::ArmorDetection::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ArmorDetection_camera_id(msg_);
  }

private:
  ::sentry_interfaces::msg::ArmorDetection msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sentry_interfaces::msg::ArmorDetection>()
{
  return sentry_interfaces::msg::builder::Init_ArmorDetection_header();
}

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__BUILDER_HPP_
