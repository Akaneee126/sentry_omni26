// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sentry_interfaces:msg/ArmorDetection.idl
// generated code does not contain a copyright notice
#include "sentry_interfaces/msg/detail/armor_detection__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
sentry_interfaces__msg__ArmorDetection__init(sentry_interfaces__msg__ArmorDetection * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    sentry_interfaces__msg__ArmorDetection__fini(msg);
    return false;
  }
  // camera_id
  // target_color
  // target_number
  // confidence
  // x
  // y
  // width
  // height
  // distance
  return true;
}

void
sentry_interfaces__msg__ArmorDetection__fini(sentry_interfaces__msg__ArmorDetection * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // camera_id
  // target_color
  // target_number
  // confidence
  // x
  // y
  // width
  // height
  // distance
}

bool
sentry_interfaces__msg__ArmorDetection__are_equal(const sentry_interfaces__msg__ArmorDetection * lhs, const sentry_interfaces__msg__ArmorDetection * rhs)
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
  // target_color
  if (lhs->target_color != rhs->target_color) {
    return false;
  }
  // target_number
  if (lhs->target_number != rhs->target_number) {
    return false;
  }
  // confidence
  if (lhs->confidence != rhs->confidence) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // width
  if (lhs->width != rhs->width) {
    return false;
  }
  // height
  if (lhs->height != rhs->height) {
    return false;
  }
  // distance
  if (lhs->distance != rhs->distance) {
    return false;
  }
  return true;
}

bool
sentry_interfaces__msg__ArmorDetection__copy(
  const sentry_interfaces__msg__ArmorDetection * input,
  sentry_interfaces__msg__ArmorDetection * output)
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
  // target_color
  output->target_color = input->target_color;
  // target_number
  output->target_number = input->target_number;
  // confidence
  output->confidence = input->confidence;
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // width
  output->width = input->width;
  // height
  output->height = input->height;
  // distance
  output->distance = input->distance;
  return true;
}

sentry_interfaces__msg__ArmorDetection *
sentry_interfaces__msg__ArmorDetection__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetection * msg = (sentry_interfaces__msg__ArmorDetection *)allocator.allocate(sizeof(sentry_interfaces__msg__ArmorDetection), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sentry_interfaces__msg__ArmorDetection));
  bool success = sentry_interfaces__msg__ArmorDetection__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sentry_interfaces__msg__ArmorDetection__destroy(sentry_interfaces__msg__ArmorDetection * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sentry_interfaces__msg__ArmorDetection__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sentry_interfaces__msg__ArmorDetection__Sequence__init(sentry_interfaces__msg__ArmorDetection__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetection * data = NULL;

  if (size) {
    data = (sentry_interfaces__msg__ArmorDetection *)allocator.zero_allocate(size, sizeof(sentry_interfaces__msg__ArmorDetection), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sentry_interfaces__msg__ArmorDetection__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sentry_interfaces__msg__ArmorDetection__fini(&data[i - 1]);
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
sentry_interfaces__msg__ArmorDetection__Sequence__fini(sentry_interfaces__msg__ArmorDetection__Sequence * array)
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
      sentry_interfaces__msg__ArmorDetection__fini(&array->data[i]);
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

sentry_interfaces__msg__ArmorDetection__Sequence *
sentry_interfaces__msg__ArmorDetection__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sentry_interfaces__msg__ArmorDetection__Sequence * array = (sentry_interfaces__msg__ArmorDetection__Sequence *)allocator.allocate(sizeof(sentry_interfaces__msg__ArmorDetection__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sentry_interfaces__msg__ArmorDetection__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sentry_interfaces__msg__ArmorDetection__Sequence__destroy(sentry_interfaces__msg__ArmorDetection__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sentry_interfaces__msg__ArmorDetection__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sentry_interfaces__msg__ArmorDetection__Sequence__are_equal(const sentry_interfaces__msg__ArmorDetection__Sequence * lhs, const sentry_interfaces__msg__ArmorDetection__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sentry_interfaces__msg__ArmorDetection__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sentry_interfaces__msg__ArmorDetection__Sequence__copy(
  const sentry_interfaces__msg__ArmorDetection__Sequence * input,
  sentry_interfaces__msg__ArmorDetection__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sentry_interfaces__msg__ArmorDetection);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sentry_interfaces__msg__ArmorDetection * data =
      (sentry_interfaces__msg__ArmorDetection *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sentry_interfaces__msg__ArmorDetection__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sentry_interfaces__msg__ArmorDetection__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sentry_interfaces__msg__ArmorDetection__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
