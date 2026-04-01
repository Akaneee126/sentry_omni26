// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sentry_interfaces/msg/armor_detection_array.h"


#ifndef SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_H_
#define SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_H_

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
// Member 'detections'
#include "sentry_interfaces/msg/detail/armor_detection__struct.h"

/// Struct defined in msg/ArmorDetectionArray in the package sentry_interfaces.
typedef struct sentry_interfaces__msg__ArmorDetectionArray
{
  std_msgs__msg__Header header;
  uint8_t camera_id;
  sentry_interfaces__msg__ArmorDetection__Sequence detections;
} sentry_interfaces__msg__ArmorDetectionArray;

// Struct for a sequence of sentry_interfaces__msg__ArmorDetectionArray.
typedef struct sentry_interfaces__msg__ArmorDetectionArray__Sequence
{
  sentry_interfaces__msg__ArmorDetectionArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sentry_interfaces__msg__ArmorDetectionArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENTRY_INTERFACES__MSG__DETAIL__ARMOR_DETECTION_ARRAY__STRUCT_H_
