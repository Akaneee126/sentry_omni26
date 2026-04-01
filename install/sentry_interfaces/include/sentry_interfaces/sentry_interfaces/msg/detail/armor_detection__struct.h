// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sentry_interfaces:msg/ArmorDetection.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection.h"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_H_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_H_

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

/// Struct defined in msg/ArmorDetection in the package sentry_interfaces.
typedef struct sentry_interfaces__msg__ArmorDetection
{
  std_msgs__msg__Header header;
  uint8_t camera_id;
  uint8_t target_color;
  uint8_t target_number;
  float confidence;
  float x;
  float y;
  float width;
  float height;
  float distance;
} sentry_interfaces__msg__ArmorDetection;

// Struct for a sequence of sentry_interfaces__msg__ArmorDetection.
typedef struct sentry_interfaces__msg__ArmorDetection__Sequence
{
  sentry_interfaces__msg__ArmorDetection * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sentry_interfaces__msg__ArmorDetection__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION__STRUCT_H_
