#----------------------------------------------------------------
# Generated CMake target import file for configuration "release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sentry_interfaces::sentry_interfaces__rosidl_typesupport_cpp" for configuration "release"
set_property(TARGET sentry_interfaces::sentry_interfaces__rosidl_typesupport_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sentry_interfaces::sentry_interfaces__rosidl_typesupport_cpp PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "rosidl_runtime_c::rosidl_runtime_c;rosidl_typesupport_cpp::rosidl_typesupport_cpp;rosidl_typesupport_c::rosidl_typesupport_c"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsentry_interfaces__rosidl_typesupport_cpp.so"
  IMPORTED_SONAME_RELEASE "libsentry_interfaces__rosidl_typesupport_cpp.so"
  )

list(APPEND _cmake_import_check_targets sentry_interfaces::sentry_interfaces__rosidl_typesupport_cpp )
list(APPEND _cmake_import_check_files_for_sentry_interfaces::sentry_interfaces__rosidl_typesupport_cpp "${_IMPORT_PREFIX}/lib/libsentry_interfaces__rosidl_typesupport_cpp.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
