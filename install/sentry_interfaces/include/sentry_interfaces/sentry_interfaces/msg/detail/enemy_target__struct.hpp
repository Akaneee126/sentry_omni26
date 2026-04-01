// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sentry_interfaces:msg/EnemyTarget.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_HPP_

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
# define DEPRECATED__sentry_interfaces__msg__EnemyTarget __attribute__((deprecated))
#else
# define DEPRECATED__sentry_interfaces__msg__EnemyTarget __declspec(deprecated)
#endif

namespace sentry_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct EnemyTarget_
{
  using Type = EnemyTarget_<ContainerAllocator>;

  explicit EnemyTarget_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->yaw = 0.0f;
      this->confidence = 0.0f;
      this->is_tracking = false;
    }
  }

  explicit EnemyTarget_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->yaw = 0.0f;
      this->confidence = 0.0f;
      this->is_tracking = false;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _id_type =
    uint8_t;
  _id_type id;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _z_type =
    float;
  _z_type z;
  using _yaw_type =
    float;
  _yaw_type yaw;
  using _confidence_type =
    float;
  _confidence_type confidence;
  using _is_tracking_type =
    bool;
  _is_tracking_type is_tracking;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__id(
    const uint8_t & _arg)
  {
    this->id = _arg;
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
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }
  Type & set__yaw(
    const float & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__confidence(
    const float & _arg)
  {
    this->confidence = _arg;
    return *this;
  }
  Type & set__is_tracking(
    const bool & _arg)
  {
    this->is_tracking = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> *;
  using ConstRawPtr =
    const sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sentry_interfaces__msg__EnemyTarget
    std::shared_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sentry_interfaces__msg__EnemyTarget
    std::shared_ptr<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EnemyTarget_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->id != other.id) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->z != other.z) {
      return false;
    }
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->confidence != other.confidence) {
      return false;
    }
    if (this->is_tracking != other.is_tracking) {
      return false;
    }
    return true;
  }
  bool operator!=(const EnemyTarget_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EnemyTarget_

// alias to use template instance with default allocator
using EnemyTarget =
  sentry_interfaces::msg::EnemyTarget_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_HPP_
