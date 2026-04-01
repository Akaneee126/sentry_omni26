// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__TRAITS_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sentry_interfaces/msg/detail/armor_detection_array__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'detections'
#include "sentry_interfaces/msg/detail/armor_detection__traits.hpp"

namespace sentry_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const ArmorDetectionArray & msg,
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

  // member: detections
  {
    if (msg.detections.size() == 0) {
      out << "detections: []";
    } else {
      out << "detections: [";
      size_t pending_items = msg.detections.size();
      for (auto item : msg.detections) {
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
  const ArmorDetectionArray & msg,
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

  // member: detections
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.detections.size() == 0) {
      out << "detections: []\n";
    } else {
      out << "detections:\n";
      for (auto item : msg.detections) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ArmorDetectionArray & msg, bool use_flow_style = false)
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
  const sentry_interfaces::msg::ArmorDetectionArray & msg,
  std::ostream & out, size_t indentation = 0)
{
  sentry_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sentry_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const sentry_interfaces::msg::ArmorDetectionArray & msg)
{
  return sentry_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sentry_interfaces::msg::ArmorDetectionArray>()
{
  return "sentry_interfaces::msg::ArmorDetectionArray";
}

template<>
inline const char * name<sentry_interfaces::msg::ArmorDetectionArray>()
{
  return "sentry_interfaces/msg/ArmorDetectionArray";
}

template<>
struct has_fixed_size<sentry_interfaces::msg::ArmorDetectionArray>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sentry_interfaces::msg::ArmorDetectionArray>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sentry_interfaces::msg::ArmorDetectionArray>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__TRAITS_HPP_
