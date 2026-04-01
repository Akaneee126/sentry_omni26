// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from sentry_interfaces:msg/ArmorDetectionArray.idl
// generated code does not contain a copyright notice

#include "sentry_interfaces/msg/detail/armor_detection_array__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_sentry_interfaces
const rosidl_type_hash_t *
sentry_interfaces__msg__ArmorDetectionArray__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd5, 0x66, 0x97, 0x8e, 0x3c, 0x9c, 0x1d, 0x07,
      0x76, 0x67, 0xf5, 0x2f, 0xfa, 0xe2, 0x7b, 0x6b,
      0xd3, 0xe7, 0x40, 0xc7, 0x5e, 0xc9, 0x80, 0x3d,
      0x09, 0x34, 0x7b, 0x9a, 0x66, 0x4c, 0x27, 0x0c,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "std_msgs/msg/detail/header__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "sentry_interfaces/msg/detail/armor_detection__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t sentry_interfaces__msg__ArmorDetection__EXPECTED_HASH = {1, {
    0xec, 0xdb, 0xaf, 0x3b, 0xa0, 0x12, 0x8d, 0x04,
    0xaa, 0x82, 0x9c, 0xb5, 0xdc, 0xa1, 0x8a, 0x73,
    0xc3, 0x72, 0x4f, 0x6e, 0xa3, 0x4a, 0x31, 0x73,
    0x38, 0x50, 0x5e, 0x74, 0xa6, 0xfc, 0x8f, 0x91,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char sentry_interfaces__msg__ArmorDetectionArray__TYPE_NAME[] = "sentry_interfaces/msg/ArmorDetectionArray";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char sentry_interfaces__msg__ArmorDetection__TYPE_NAME[] = "sentry_interfaces/msg/ArmorDetection";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__header[] = "header";
static char sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__camera_id[] = "camera_id";
static char sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__detections[] = "detections";

static rosidl_runtime_c__type_description__Field sentry_interfaces__msg__ArmorDetectionArray__FIELDS[] = {
  {
    {sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__camera_id, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {sentry_interfaces__msg__ArmorDetectionArray__FIELD_NAME__detections, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {sentry_interfaces__msg__ArmorDetection__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription sentry_interfaces__msg__ArmorDetectionArray__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {sentry_interfaces__msg__ArmorDetection__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
sentry_interfaces__msg__ArmorDetectionArray__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {sentry_interfaces__msg__ArmorDetectionArray__TYPE_NAME, 41, 41},
      {sentry_interfaces__msg__ArmorDetectionArray__FIELDS, 3, 3},
    },
    {sentry_interfaces__msg__ArmorDetectionArray__REFERENCED_TYPE_DESCRIPTIONS, 3, 3},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&sentry_interfaces__msg__ArmorDetection__EXPECTED_HASH, sentry_interfaces__msg__ArmorDetection__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = sentry_interfaces__msg__ArmorDetection__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "std_msgs/Header header\n"
  "uint8 camera_id\n"
  "sentry_interfaces/ArmorDetection[] detections";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
sentry_interfaces__msg__ArmorDetectionArray__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {sentry_interfaces__msg__ArmorDetectionArray__TYPE_NAME, 41, 41},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 85, 85},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
sentry_interfaces__msg__ArmorDetectionArray__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[4];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 4, 4};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *sentry_interfaces__msg__ArmorDetectionArray__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *sentry_interfaces__msg__ArmorDetection__get_individual_type_description_source(NULL);
    sources[3] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
