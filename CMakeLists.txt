cmake_minimum_required(VERSION 3.14)

project(GTestAllureUtilities)

# Configure environment
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch external dependencies
include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# RapidJSON (header-only)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(rapidjson)

# Note: Systelab JSON adapters are vendored in vendor/ directory
# See vendor/THIRD_PARTY_LICENSES.md for attribution
# See vendor/MODIFICATIONS.md for list of changes we made

# Adjust compilation flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=implicit-fallthrough")
endif()

# Configure include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor)
include_directories(${rapidjson_SOURCE_DIR}/include)

# Add main library (always built)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/GTestAllureUtilities)

# Only build tests and examples if this is the main project
# This allows the library to be used via FetchContent without building tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "Building GTestAllureUtilities tests and examples")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/TestUtilities)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/UnitTest)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/IntegrationTest)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/SampleTestProject)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/api-comparison)
else()
    message(STATUS "GTestAllureUtilities: Skipping tests and examples (included as subdirectory)")
endif()
