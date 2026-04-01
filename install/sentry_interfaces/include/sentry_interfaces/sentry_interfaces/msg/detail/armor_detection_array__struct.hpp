// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_HPP_

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
// Member 'detections'
#include "sentry_interfaces/msg/detail/armor_detection__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__sentry_interfaces__msg__ArmorDetectionArray __attribute__((deprecated))
#else
# define DEPRECATED__sentry_interfaces__msg__ArmorDetectionArray __declspec(deprecated)
#endif

namespace sentry_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ArmorDetectionArray_
{
  using Type = ArmorDetectionArray_<ContainerAllocator>;

  explicit ArmorDetectionArray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->camera_id = 0;
    }
  }

  explicit ArmorDetectionArray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->camera_id = 0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _camera_id_type =
    uint8_t;
  _camera_id_type camera_id;
  using _detections_type =
    std::vector<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>>;
  _detections_type detections;

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
  Type & set__detections(
    const std::vector<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sentry_interfaces::msg::ArmorDetection_<ContainerAllocator>>> & _arg)
  {
    this->detections = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> *;
  using ConstRawPtr =
    const sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sentry_interfaces__msg__ArmorDetectionArray
    std::shared_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sentry_interfaces__msg__ArmorDetectionArray
    std::shared_ptr<sentry_interfaces::msg::ArmorDetectionArray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ArmorDetectionArray_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->camera_id != other.camera_id) {
      return false;
    }
    if (this->detections != other.detections) {
      return false;
    }
    return true;
  }
  bool operator!=(const ArmorDetectionArray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ArmorDetectionArray_

// alias to use template instance with default allocator
using ArmorDetectionArray =
  sentry_interfaces::msg::ArmorDetectionArray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_HPP_
