cmake_minimum_required(VERSION 3.14)

project(GTestAllureUtilities)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
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

# nlohmann/json (header-only)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Adjust compilation flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=implicit-fallthrough")
endif()

# Configure include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)

# Add main library (always built)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Only build tests and examples if this is the main project
# This allows the library to be used via FetchContent without building tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "Building GTestAllureUtilities tests and examples")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/TestUtilities)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/UnitTest)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/IntegrationTest)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/SampleTestProject)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
else()
    message(STATUS "GTestAllureUtilities: Skipping tests and examples (included as subdirectory)")
endif()
