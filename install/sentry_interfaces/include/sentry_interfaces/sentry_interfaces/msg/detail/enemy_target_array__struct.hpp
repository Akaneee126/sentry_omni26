// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sentry_interfaces:msg/EnemyTargetArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target_array.hpp"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_HPP_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_HPP_

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
// Member 'targets'
#include "sentry_interfaces/msg/detail/enemy_target__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__sentry_interfaces__msg__EnemyTargetArray __attribute__((deprecated))
#else
# define DEPRECATED__sentry_interfaces__msg__EnemyTargetArray __declspec(deprecated)
#endif

namespace sentry_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct EnemyTargetArray_
{
  using Type = EnemyTargetArray_<ContainerAllocator>;

  explicit EnemyTargetArray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit EnemyTargetArray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _targets_type =
    std::vector<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>>;
  _targets_type targets;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__targets(
    const std::vector<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sentry_interfaces::msg::EnemyTarget_<ContainerAllocator>>> & _arg)
  {
    this->targets = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> *;
  using ConstRawPtr =
    const sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sentry_interfaces__msg__EnemyTargetArray
    std::shared_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sentry_interfaces__msg__EnemyTargetArray
    std::shared_ptr<sentry_interfaces::msg::EnemyTargetArray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EnemyTargetArray_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->targets != other.targets) {
      return false;
    }
    return true;
  }
  bool operator!=(const EnemyTargetArray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EnemyTargetArray_

// alias to use template instance with default allocator
using EnemyTargetArray =
  sentry_interfaces::msg::EnemyTargetArray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sentry_interfaces

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_HPP_
