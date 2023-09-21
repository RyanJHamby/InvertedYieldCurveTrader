# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.13)

# set aws cpp sdk pathname
set(CMAKE_PREFIX_PATH "/Users/ryanhamby/Desktop/aws-sdk-cpp-install")

# Set the AWS service components used by this project.
set(SERVICE_COMPONENTS athena)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# Set this project's name.
project("InvertedYieldCurveTrader")

# Set the C++ standard to use to build this target.
# At least C++ 11 is required for the AWS SDK for C++.
set(CMAKE_CXX_STANDARD 20)

# Use the MSVC variable to determine if this is a Windows build.
set(WINDOWS_BUILD ${MSVC})

if (WINDOWS_BUILD) # Set the location where CMake can find the installed libraries for the AWS SDK.
    string(REPLACE ";" "/aws-cpp-sdk-all;" SYSTEM_MODULE_PATH "${CMAKE_SYSTEM_PREFIX_PATH}/aws-cpp-sdk-all")
    list(APPEND CMAKE_PREFIX_PATH ${SYSTEM_MODULE_PATH})
endif ()

# Find the AWS SDK for C++ package.
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})
find_package(GTest REQUIRED)

if (WINDOWS_BUILD)
    # Copy relevant AWS SDK for C++ libraries into the current binary directory for running and debugging.

    # set(BIN_SUB_DIR "/Debug") # if you are building from the command line you may need to uncomment this
    # and set the proper subdirectory to the executables' location.

    AWSSDK_CPY_DYN_LIBS(SERVICE_COMPONENTS "" ${CMAKE_CURRENT_BINARY_DIR}${BIN_SUB_DIR})
endif ()

set(TESTS_PARENT_DIR "${CMAKE_SOURCE_DIR}/..")
file(GLOB TEST_SOURCES
    "${TESTS_PARENT_DIR}/test/*Test.cpp"
    )
add_executable(test ${TEST_SOURCES})
add_executable(${PROJECT_NAME}
        MultiVarAnalysisWorkflow.cpp)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories(${AWSSDK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        ${AWSSDK_LINK_LIBRARIES}
        ${AWSSDK_LIBRARIES})
        
target_link_libraries(test
        ${AWSSDK_LINK_LIBRARIES}
        ${AWSSDK_LIBRARIES}
        ${GTEST_BOTH_LIBRARIES})
