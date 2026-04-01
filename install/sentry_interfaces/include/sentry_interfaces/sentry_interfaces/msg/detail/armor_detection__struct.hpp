// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sentry_interfaces:msg/ArmorDetection.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__sentry_interfaces__msg__ArmorDetection __attribute__((deprecated))
#else
# define DEPRECATED__sentry_interfaces__msg__ArmorDetection __declspec(deprecated)
#endif

namespace sentry_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ArmorDetection_
{
  using Type = ArmorDetection_<ContainerAllocator>;

  explicit ArmorDetection_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->camera_id = 0;
      this->target_color = 0;
      this->target_number = 0;
      this->confidence = 0.0f;
      this->x = 0.0f;
      this->y = 0.0f;
      this->width = 0.0f;
      this->height = 0.0f;
      this->distance = 0.0f;
    }
  }

  explicit ArmorDetection_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->camera_id = 0;
      this->target_color = 0;
      this->target_number = 0;
      this->confidence = 0.0f;
      this->x = 0.0f;
      this->y = 0.0f;
      this->width = 0.0f;
      this->height = 0.0f;
      this->distance = 0.0f;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _camera_id_type =
    uint8_t;
  _camera_id_type camera_id;
  using _target_color_type =
    uint8_t;
  _target_color_type target_color;
  using _target_number_type =
    uint8_t;
  _target_number_type target_number;
  using _confidence_type =
    float;
  _confidence_type confidence;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _width_type =
    float;
  _width_type width;
  using _height_type =
    float;
  _height_type height;
  using _distance_type =
    float;
  _distance_type distance;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__camera_id(
    const uint8_t & _arg)
  {
    this->camera_id = _arg;
    return *this;
  }
  Type & set__target_color(
    const uint8_t & _arg)
  {
    this->target_color = _arg;
    return *this;
  }
  Type & set__target_number(
    const uint8_t & _arg)
  {
    this->target_number = _arg;
    return *this;
  }
  Type & set__confidence(
    const float & _arg)
  {
    this->confidence = _arg;
    return *this;
  }
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__width(
    const float & _arg)
  {
    this->width = _arg;
    return *this;
  }
  Type & set__height(
    const float & _arg)
  {
    this->height = _arg;
    return *this;
  }
  Type & set__distance(
    const float & _arg)
  {
    this->distance = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> *;
  using ConstRawPtr =
    const sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sentry_interfaces__msg__ArmorDetection
    std::shared_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sentry_interfaces__msg__ArmorDetection
    std::shared_ptr<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ArmorDetection_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->camera_id != other.camera_id) {
      return false;
    }
    if (this->target_color != other.target_color) {
      return false;
    }
    if (this->target_number != other.target_number) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->width != other.width) {
      return false;
    }
    if (this->height != other.height) {
      return false;
    }
    if (this->distance != other.distance) {
      return false;
    }
    return true;
  }
  bool operator!=(const ArmorDetection_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ArmorDetection_

// alias to use template instance with default allocator
using ArmorDetection =
  sentry_interfaces::msg::ArmorDetection_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_HPP_
