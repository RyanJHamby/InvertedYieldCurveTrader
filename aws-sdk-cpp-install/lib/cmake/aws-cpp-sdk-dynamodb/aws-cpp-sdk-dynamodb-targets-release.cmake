#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "aws-cpp-sdk-dynamodb" for configuration "Release"
set_property(TARGET aws-cpp-sdk-dynamodb APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(aws-cpp-sdk-dynamodb PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "aws-cpp-sdk-core"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libaws-cpp-sdk-dynamodb.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libaws-cpp-sdk-dynamodb.dylib"
  )

list(APPEND _cmake_import_check_targets aws-cpp-sdk-dynamodb )
list(APPEND _cmake_import_check_files_for_aws-cpp-sdk-dynamodb "${_IMPORT_PREFIX}/lib/libaws-cpp-sdk-dynamodb.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
