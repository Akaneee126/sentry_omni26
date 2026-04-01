// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sentry_interfaces:msg/EnemyTarget.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target.h"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_H_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/EnemyTarget in the package sentry_interfaces.
typedef struct sentry_interfaces__msg__EnemyTarget
{
  std_msgs__msg__Header header;
  uint8_t id;
  float x;
  float y;
  float z;
  float yaw;
  float confidence;
  bool is_tracking;
} sentry_interfaces__msg__EnemyTarget;

// Struct for a sequence of sentry_interfaces__msg__EnemyTarget.
typedef struct sentry_interfaces__msg__EnemyTarget__Sequence
{
  sentry_interfaces__msg__EnemyTarget * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sentry_interfaces__msg__EnemyTarget__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET__STRUCT_H_
