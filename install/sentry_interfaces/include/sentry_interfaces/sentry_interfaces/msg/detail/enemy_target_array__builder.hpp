// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sentry_interfaces:msg/EnemyTargetArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__BUILDER_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sentry_interfaces/msg/detail/enemy_target_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sentry_interfaces
{

namespace msg
{

namespace builder
{

class Init_EnemyTargetArray_targets
{
public:
  explicit Init_EnemyTargetArray_targets(::sentry_interfaces::msg::EnemyTargetArray & msg)
  : msg_(msg)
  {}
  ::sentry_interfaces::msg::EnemyTargetArray targets(::sentry_interfaces::msg::EnemyTargetArray::_targets_type arg)
  {
    msg_.targets = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTargetArray msg_;
};

class Init_EnemyTargetArray_header
{
public:
  Init_EnemyTargetArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_EnemyTargetArray_targets header(::sentry_interfaces::msg::EnemyTargetArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_EnemyTargetArray_targets(msg_);
  }

private:
  ::sentry_interfaces::msg::EnemyTargetArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sentry_interfaces::msg::EnemyTargetArray>()
{
  return sentry_interfaces::msg::builder::Init_EnemyTargetArray_header();
}

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__BUILDER_HPP_
