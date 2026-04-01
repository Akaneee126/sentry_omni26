// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sentry_interfaces:msg/EnemyTargetArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__TRAITS_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sentry_interfaces/msg/detail/enemy_target_array__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'targets'
#include "sentry_interfaces/msg/detail/enemy_target__traits.hpp"

namespace sentry_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const EnemyTargetArray & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: targets
  {
    if (msg.targets.size() == 0) {
      out << "targets: []";
    } else {
      out << "targets: [";
      size_t pending_items = msg.targets.size();
      for (auto item : msg.targets) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const EnemyTargetArray & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: targets
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.targets.size() == 0) {
      out << "targets: []\n";
    } else {
      out << "targets:\n";
      for (auto item : msg.targets) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const EnemyTargetArray & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace sentry_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use sentry_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const sentry_interfaces::msg::EnemyTargetArray & msg,
  std::ostream & out, size_t indentation = 0)
{
  sentry_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sentry_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const sentry_interfaces::msg::EnemyTargetArray & msg)
{
  return sentry_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sentry_interfaces::msg::EnemyTargetArray>()
{
  return "sentry_interfaces::msg::EnemyTargetArray";
}

template<>
inline const char * name<sentry_interfaces::msg::EnemyTargetArray>()
{
  return "sentry_interfaces/msg/EnemyTargetArray";
}

template<>
struct has_fixed_size<sentry_interfaces::msg::EnemyTargetArray>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sentry_interfaces::msg::EnemyTargetArray>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sentry_interfaces::msg::EnemyTargetArray>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__TRAITS_HPP_
