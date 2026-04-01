// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "sentry_interfaces/msg/detail/armor_detection_array__functions.h"
#include "sentry_interfaces/msg/detail/armor_detection_array__struct.hpp"
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

void ArmorDetectionArray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) sentry_interfaces::msg::ArmorDetectionArray(_init);
}

void ArmorDetectionArray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<sentry_interfaces::msg::ArmorDetectionArray *>(message_memory);
  typed_message->~ArmorDetectionArray();
}

size_t size_function__ArmorDetectionArray__detections(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<sentry_interfaces::msg::ArmorDetection> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ArmorDetectionArray__detections(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<sentry_interfaces::msg::ArmorDetection> *>(untyped_member);
  return &member[index];
}

void * get_function__ArmorDetectionArray__detections(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<sentry_interfaces::msg::ArmorDetection> *>(untyped_member);
  return &member[index];
}

void fetch_function__ArmorDetectionArray__detections(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const sentry_interfaces::msg::ArmorDetection *>(
    get_const_function__ArmorDetectionArray__detections(untyped_member, index));
  auto & value = *reinterpret_cast<sentry_interfaces::msg::ArmorDetection *>(untyped_value);
  value = item;
}

void assign_function__ArmorDetectionArray__detections(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<sentry_interfaces::msg::ArmorDetection *>(
    get_function__ArmorDetectionArray__detections(untyped_member, index));
  const auto & value = *reinterpret_cast<const sentry_interfaces::msg::ArmorDetection *>(untyped_value);
  item = value;
}

void resize_function__ArmorDetectionArray__detections(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<sentry_interfaces::msg::ArmorDetection> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ArmorDetectionArray_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sentry_interfaces::msg::ArmorDetectionArray, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "camera_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sentry_interfaces::msg::ArmorDetectionArray, camera_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "detections",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<sentry_interfaces::msg::ArmorDetection>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sentry_interfaces::msg::ArmorDetectionArray, detections),  // bytes offset in struct
    nullptr,  // default value
    size_function__ArmorDetectionArray__detections,  // size() function pointer
    get_const_function__ArmorDetectionArray__detections,  // get_const(index) function pointer
    get_function__ArmorDetectionArray__detections,  // get(index) function pointer
    fetch_function__ArmorDetectionArray__detections,  // fetch(index, &value) function pointer
    assign_function__ArmorDetectionArray__detections,  // assign(index, value) function pointer
    resize_function__ArmorDetectionArray__detections  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ArmorDetectionArray_message_members = {
  "sentry_interfaces::msg",  // message namespace
  "ArmorDetectionArray",  // message name
  3,  // number of fields
  sizeof(sentry_interfaces::msg::ArmorDetectionArray),
  false,  // has_any_key_member_
  ArmorDetectionArray_message_member_array,  // message members
  ArmorDetectionArray_init_function,  // function to initialize message memory (memory has to be allocated)
  ArmorDetectionArray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ArmorDetectionArray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ArmorDetectionArray_message_members,
  get_message_typesupport_handle_function,
  &sentry_interfaces__msg__ArmorDetectionArray__get_type_hash,
  &sentry_interfaces__msg__ArmorDetectionArray__get_type_description,
  &sentry_interfaces__msg__ArmorDetectionArray__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace sentry_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<sentry_interfaces::msg::ArmorDetectionArray>()
{
  return &::sentry_interfaces::msg::rosidl_typesupport_introspection_cpp::ArmorDetectionArray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, sentry_interfaces, msg, ArmorDetectionArray)() {
  return &::sentry_interfaces::msg::rosidl_typesupport_introspection_cpp::ArmorDetectionArray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
