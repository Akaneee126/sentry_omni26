// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from sentry_interfaces:msg/EnemyTarget.idl
// generated code does not contain a copyright notice
#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "sentry_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "sentry_interfaces/msg/detail/enemy_target__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
bool cdr_serialize_sentry_interfaces__msg__EnemyTarget(
  const sentry_interfaces__msg__EnemyTarget * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
bool cdr_deserialize_sentry_interfaces__msg__EnemyTarget(
  eprosima::fastcdr::Cdr &,
  sentry_interfaces__msg__EnemyTarget * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
size_t get_serialized_size_sentry_interfaces__msg__EnemyTarget(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
size_t max_serialized_size_sentry_interfaces__msg__EnemyTarget(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
bool cdr_serialize_key_sentry_interfaces__msg__EnemyTarget(
  const sentry_interfaces__msg__EnemyTarget * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
size_t get_serialized_size_key_sentry_interfaces__msg__EnemyTarget(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
size_t max_serialized_size_key_sentry_interfaces__msg__EnemyTarget(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_sentry_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, sentry_interfaces, msg, EnemyTarget)();

#ifdef __cplusplus
}
#endif

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
