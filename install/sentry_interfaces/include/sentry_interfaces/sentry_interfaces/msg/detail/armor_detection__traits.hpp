// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sentry_interfaces:msg/ArmorDetection.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__TRAITS_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sentry_interfaces/msg/detail/armor_detection__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace sentry_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const ArmorDetection & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: camera_id
  {
    out << "camera_id: ";
    rosidl_generator_traits::value_to_yaml(msg.camera_id, out);
    out << ", ";
  }

  // member: target_color
  {
    out << "target_color: ";
    rosidl_generator_traits::value_to_yaml(msg.target_color, out);
    out << ", ";
  }

  // member: target_number
  {
    out << "target_number: ";
    rosidl_generator_traits::value_to_yaml(msg.target_number, out);
    out << ", ";
  }

  // member: confidence
  {
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: width
  {
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << ", ";
  }

  // member: distance
  {
    out << "distance: ";
    rosidl_generator_traits::value_to_yaml(msg.distance, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ArmorDetection & msg,
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

  // member: camera_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "camera_id: ";
    rosidl_generator_traits::value_to_yaml(msg.camera_id, out);
    out << "\n";
  }

  // member: target_color
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_color: ";
    rosidl_generator_traits::value_to_yaml(msg.target_color, out);
    out << "\n";
  }

  // member: target_number
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_number: ";
    rosidl_generator_traits::value_to_yaml(msg.target_number, out);
    out << "\n";
  }

  // member: confidence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: width
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << "\n";
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }

  // member: distance
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "distance: ";
    rosidl_generator_traits::value_to_yaml(msg.distance, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ArmorDetection & msg, bool use_flow_style = false)
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
  const sentry_interfaces::msg::ArmorDetection & msg,
  std::ostream & out, size_t indentation = 0)
{
  sentry_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sentry_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const sentry_interfaces::msg::ArmorDetection & msg)
{
  return sentry_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sentry_interfaces::msg::ArmorDetection>()
{
  return "sentry_interfaces::msg::ArmorDetection";
}

template<>
inline const char * name<sentry_interfaces::msg::ArmorDetection>()
{
  return "sentry_interfaces/msg/ArmorDetection";
}

template<>
struct has_fixed_size<sentry_interfaces::msg::ArmorDetection>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<sentry_interfaces::msg::ArmorDetection>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<sentry_interfaces::msg::ArmorDetection>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__TRAITS_HPP_
