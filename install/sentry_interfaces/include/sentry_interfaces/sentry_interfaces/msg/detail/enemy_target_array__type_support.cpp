// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from sentry_interfaces:msg/EnemyTargetArray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "sentry_interfaces/msg/detail/enemy_target_array__functions.h"
#include "sentry_interfaces/msg/detail/enemy_target_array__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace sentry_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void EnemyTargetArray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) sentry_interfaces::msg::EnemyTargetArray(_init);
}

void EnemyTargetArray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<sentry_interfaces::msg::EnemyTargetArray *>(message_memory);
  typed_message->~EnemyTargetArray();
}

size_t size_function__EnemyTargetArray__targets(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<sentry_interfaces::msg::EnemyTarget> *>(untyped_member);
  return member->size();
}

const void * get_const_function__EnemyTargetArray__targets(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<sentry_interfaces::msg::EnemyTarget> *>(untyped_member);
  return &member[index];
}

void * get_function__EnemyTargetArray__targets(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<sentry_interfaces::msg::EnemyTarget> *>(untyped_member);
  return &member[index];
}

void fetch_function__EnemyTargetArray__targets(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const sentry_interfaces::msg::EnemyTarget *>(
    get_const_function__EnemyTargetArray__targets(untyped_member, index));
  auto & value = *reinterpret_cast<sentry_interfaces::msg::EnemyTarget *>(untyped_value);
  value = item;
}

void assign_function__EnemyTargetArray__targets(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<sentry_interfaces::msg::EnemyTarget *>(
    get_function__EnemyTargetArray__targets(untyped_member, index));
  const auto & value = *reinterpret_cast<const sentry_interfaces::msg::EnemyTarget *>(untyped_value);
  item = value;
}

void resize_function__EnemyTargetArray__targets(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<sentry_interfaces::msg::EnemyTarget> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember EnemyTargetArray_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sentry_interfaces::msg::EnemyTargetArray, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "targets",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<sentry_interfaces::msg::EnemyTarget>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sentry_interfaces::msg::EnemyTargetArray, targets),  // bytes offset in struct
    nullptr,  // default value
    size_function__EnemyTargetArray__targets,  // size() function pointer
    get_const_function__EnemyTargetArray__targets,  // get_const(index) function pointer
    get_function__EnemyTargetArray__targets,  // get(index) function pointer
    fetch_function__EnemyTargetArray__targets,  // fetch(index, &value) function pointer
    assign_function__EnemyTargetArray__targets,  // assign(index, value) function pointer
    resize_function__EnemyTargetArray__targets  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers EnemyTargetArray_message_members = {
  "sentry_interfaces::msg",  // message namespace
  "EnemyTargetArray",  // message name
  2,  // number of fields
  sizeof(sentry_interfaces::msg::EnemyTargetArray),
  false,  // has_any_key_member_
  EnemyTargetArray_message_member_array,  // message members
  EnemyTargetArray_init_function,  // function to initialize message memory (memory has to be allocated)
  EnemyTargetArray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t EnemyTargetArray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &EnemyTargetArray_message_members,
  get_message_typesupport_handle_function,
  &sentry_interfaces__msg__EnemyTargetArray__get_type_hash,
  &sentry_interfaces__msg__EnemyTargetArray__get_type_description,
  &sentry_interfaces__msg__EnemyTargetArray__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace sentry_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<sentry_interfaces::msg::EnemyTargetArray>()
{
  return &::sentry_interfaces::msg::rosidl_typesupport_introspection_cpp::EnemyTargetArray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, sentry_interfaces, msg, EnemyTargetArray)() {
  return &::sentry_interfaces::msg::rosidl_typesupport_introspection_cpp::EnemyTargetArray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
