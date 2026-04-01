// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice
#include "sentry_interfaces/msg/detail/armor_detection_array__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `detections`
#include "sentry_interfaces/msg/detail/armor_detection__functions.h"

bool
sentry_interfaces__msg__ArmorDetectionArray__init(sentry_interfaces__msg__ArmorDetectionArray * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    sentry_interfaces__msg__ArmorDetectionArray__fini(msg);
    return false;
  }
  // camera_id
  // detections
  if (!sentry_interfaces__msg__ArmorDetection__Sequence__init(&msg->detections, 0)) {
    sentry_interfaces__msg__ArmorDetectionArray__fini(msg);
    return false;
  }
  return true;
}

void
sentry_interfaces__msg__ArmorDetectionArray__fini(sentry_interfaces__msg__ArmorDetectionArray * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // camera_id
  // detections
  sentry_interfaces__msg__ArmorDetection__Sequence__fini(&msg->detections);
}

bool
sentry_interfaces__msg__ArmorDetectionArray__are_equal(const sentry_interfaces__msg__ArmorDetectionArray * lhs, const sentry_interfaces__msg__ArmorDetectionArray * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // camera_id
  if (lhs->camera_id != rhs->camera_id) {
    return false;
  }
  // detections
  if (!sentry_interfaces__msg__ArmorDetection__Sequence__are_equal(
      &(lhs->detections), &(rhs->detections)))
  {
    return false;
  }
  return true;
}

bool
sentry_interfaces__msg__ArmorDetectionArray__copy(
  const sentry_interfaces__msg__ArmorDetectionArray * input,
  sentry_interfaces__msg__ArmorDetectionArray * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // camera_id
  output->camera_id = input->camera_id;
  // detections
  if (!sentry_interfaces__msg__ArmorDetection__Sequence__copy(
      &(input->detections), &(output->detections)))
  {
    return false;
  }
  return true;
}

sentry_interfaces__msg__ArmorDetectionArray *
sentry_interfaces__msg__ArmorDetectionArray__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetectionArray * msg = (sentry_interfaces__msg__ArmorDetectionArray *)allocator.allocate(sizeof(sentry_interfaces__msg__ArmorDetectionArray), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sentry_interfaces__msg__ArmorDetectionArray));
  bool success = sentry_interfaces__msg__ArmorDetectionArray__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sentry_interfaces__msg__ArmorDetectionArray__destroy(sentry_interfaces__msg__ArmorDetectionArray * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sentry_interfaces__msg__ArmorDetectionArray__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sentry_interfaces__msg__ArmorDetectionArray__Sequence__init(sentry_interfaces__msg__ArmorDetectionArray__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetectionArray * data = NULL;

  if (size) {
    data = (sentry_interfaces__msg__ArmorDetectionArray *)allocator.zero_allocate(size, sizeof(sentry_interfaces__msg__ArmorDetectionArray), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sentry_interfaces__msg__ArmorDetectionArray__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sentry_interfaces__msg__ArmorDetectionArray__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
sentry_interfaces__msg__ArmorDetectionArray__Sequence__fini(sentry_interfaces__msg__ArmorDetectionArray__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      sentry_interfaces__msg__ArmorDetectionArray__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

sentry_interfaces__msg__ArmorDetectionArray__Sequence *
sentry_interfaces__msg__ArmorDetectionArray__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetectionArray__Sequence * array = (sentry_interfaces__msg__ArmorDetectionArray__Sequence *)allocator.allocate(sizeof(sentry_interfaces__msg__ArmorDetectionArray__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sentry_interfaces__msg__ArmorDetectionArray__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sentry_interfaces__msg__ArmorDetectionArray__Sequence__destroy(sentry_interfaces__msg__ArmorDetectionArray__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sentry_interfaces__msg__ArmorDetectionArray__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sentry_interfaces__msg__ArmorDetectionArray__Sequence__are_equal(const sentry_interfaces__msg__ArmorDetectionArray__Sequence * lhs, const sentry_interfaces__msg__ArmorDetectionArray__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sentry_interfaces__msg__ArmorDetectionArray__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sentry_interfaces__msg__ArmorDetectionArray__Sequence__copy(
  const sentry_interfaces__msg__ArmorDetectionArray__Sequence * input,
  sentry_interfaces__msg__ArmorDetectionArray__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sentry_interfaces__msg__ArmorDetectionArray);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sentry_interfaces__msg__ArmorDetectionArray * data =
      (sentry_interfaces__msg__ArmorDetectionArray *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sentry_interfaces__msg__ArmorDetectionArray__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sentry_interfaces__msg__ArmorDetectionArray__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sentry_interfaces__msg__ArmorDetectionArray__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
