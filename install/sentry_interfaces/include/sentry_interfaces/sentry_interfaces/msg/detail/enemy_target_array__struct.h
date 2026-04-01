// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sentry_interfaces:msg/EnemyTargetArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/enemy_target_array.h"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_H_
#define SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_H_

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
// Member 'targets'
#include "sentry_interfaces/msg/detail/enemy_target__struct.h"

/// Struct defined in msg/EnemyTargetArray in the package sentry_interfaces.
typedef struct sentry_interfaces__msg__EnemyTargetArray
{
  std_msgs__msg__Header header;
  sentry_interfaces__msg__EnemyTarget__Sequence targets;
} sentry_interfaces__msg__EnemyTargetArray;

// Struct for a sequence of sentry_interfaces__msg__EnemyTargetArray.
typedef struct sentry_interfaces__msg__EnemyTargetArray__Sequence
{
  sentry_interfaces__msg__EnemyTargetArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sentry_interfaces__msg__EnemyTargetArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ENEMY_TARGET_ARRAY__STRUCT_H_
