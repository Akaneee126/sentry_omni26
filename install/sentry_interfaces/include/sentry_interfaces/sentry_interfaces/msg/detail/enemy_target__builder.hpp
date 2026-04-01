// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sentry_interfaces:msg/EnemyTarget.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__BUILDER_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sentry_interfaces/msg/detail/enemy_target__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sentry_interfaces
{

namespace msg
{

namespace builder
{

class Init_EnemyTarget_is_tracking
{
public:
  explicit Init_EnemyTarget_is_tracking(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  ::sentry_interfaces::msg::EnemyTarget is_tracking(::sentry_interfaces::msg::EnemyTarget::_is_tracking_type arg)
  {
    msg_.is_tracking = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_confidence
{
public:
  explicit Init_EnemyTarget_confidence(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_is_tracking confidence(::sentry_interfaces::msg::EnemyTarget::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_EnemyTarget_is_tracking(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_yaw
{
public:
  explicit Init_EnemyTarget_yaw(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_confidence yaw(::sentry_interfaces::msg::EnemyTarget::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_EnemyTarget_confidence(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_z
{
public:
  explicit Init_EnemyTarget_z(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_yaw z(::sentry_interfaces::msg::EnemyTarget::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_EnemyTarget_yaw(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_y
{
public:
  explicit Init_EnemyTarget_y(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_z y(::sentry_interfaces::msg::EnemyTarget::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_EnemyTarget_z(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_x
{
public:
  explicit Init_EnemyTarget_x(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_y x(::sentry_interfaces::msg::EnemyTarget::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_EnemyTarget_y(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_id
{
public:
  explicit Init_EnemyTarget_id(::sentry_interfaces::msg::EnemyTarget & msg)
  : msg_(msg)
  {}
  Init_EnemyTarget_x id(::sentry_interfaces::msg::EnemyTarget::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_EnemyTarget_x(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

class Init_EnemyTarget_header
{
public:
  Init_EnemyTarget_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_EnemyTarget_id header(::sentry_interfaces::msg::EnemyTarget::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_EnemyTarget_id(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTarget msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sentry_interfaces::msg::EnemyTarget>()
{
  return sentry_interfaces::msg::builder::Init_EnemyTarget_header();
}

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__BUILDER_HPP_
