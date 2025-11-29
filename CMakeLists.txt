cmake_minimum_required(VERSION 3.14)

project(AllureCpp)

# Configure environment
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Options to control which test frameworks to enable (default: none)
option(ALLURE_ENABLE_GOOGLETEST "Enable GoogleTest adapter" OFF)
option(ALLURE_ENABLE_CPPUTEST "Enable CppUTest adapter" OFF)
# Options to control which internal test targets to build (default: off)
option(ALLURE_BUILD_UNIT_TESTS "Build unit tests" OFF)
option(ALLURE_BUILD_INTEGRATION_TESTS "Build integration tests" OFF)
option(ALLURE_BUILD_EXAMPLES "Build example binaries" OFF)

# Fetch external dependencies
include(FetchContent)

# nlohmann/json (header-only) - always required
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# fmt library - required for modern API string formatting
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

# GoogleTest - optional
if(ALLURE_ENABLE_GOOGLETEST)
  message(STATUS "Allure-Cpp: GoogleTest adapter enabled")
  if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  else()
    # If the parent project already provided GTest targets (e.g., via FetchContent),
    # reuse them instead of insisting on a package config.
    if(NOT (TARGET GTest::gtest OR TARGET gtest))
      find_package(GTest CONFIG REQUIRED)
    endif()

    if(TARGET GTest::gtest AND NOT TARGET gtest)
      add_library(gtest ALIAS GTest::gtest)
    endif()
    if(TARGET GTest::gmock AND NOT TARGET gmock)
      add_library(gmock ALIAS GTest::gmock)
    endif()
    if(TARGET GTest::gtest_main AND NOT TARGET gtest_main)
      add_library(gtest_main ALIAS GTest::gtest_main)
    endif()
    if(TARGET GTest::gmock_main AND NOT TARGET gmock_main)
      add_library(gmock_main ALIAS GTest::gmock_main)
    endif()
  endif()
endif()

# CppUTest - optional
if(ALLURE_ENABLE_CPPUTEST)
  message(STATUS "Allure-Cpp: CppUTest adapter enabled")
  if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    FetchContent_Declare(
      cpputest
      GIT_REPOSITORY https://github.com/cpputest/cpputest.git
      GIT_TAG master
    )
    set(TESTS OFF CACHE BOOL "" FORCE)
    set(MEMORY_LEAK_DETECTION OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(cpputest)
  else()
    find_package(CppUTest CONFIG REQUIRED)
    if(TARGET CppUTest::CppUTest AND NOT TARGET CppUTest)
      add_library(CppUTest ALIAS CppUTest::CppUTest)
    endif()
    if(TARGET CppUTest::CppUTestExt AND NOT TARGET CppUTestExt)
      add_library(CppUTestExt ALIAS CppUTest::CppUTestExt)
    endif()
  endif()
endif()

# Adjust compilation flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=implicit-fallthrough")
endif()

# Configure include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add main library (always built)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Only build tests and examples if this is the main project
# This allows the library to be used via FetchContent without building tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "Building Allure-Cpp tests and examples")

    if(ALLURE_ENABLE_GOOGLETEST)
        if(ALLURE_BUILD_UNIT_TESTS OR ALLURE_BUILD_INTEGRATION_TESTS)
            include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/TestUtilities)
        endif()

        if(ALLURE_BUILD_UNIT_TESTS)
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/UnitTest)
        endif()

        if(ALLURE_BUILD_INTEGRATION_TESTS)
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/IntegrationTest)
        endif()

    endif()

    # Documentation snippets (always built when standalone to ensure docs are correct)
    if(ALLURE_ENABLE_GOOGLETEST OR ALLURE_ENABLE_CPPUTEST)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/docs-snippets)
    endif()

    if(ALLURE_BUILD_EXAMPLES)
        if(ALLURE_ENABLE_GOOGLETEST OR ALLURE_ENABLE_CPPUTEST)
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
        else()
            message(STATUS "Examples skipped: no test framework adapter enabled")
        endif()
    else()
        message(STATUS "Examples skipped: ALLURE_BUILD_EXAMPLES=OFF")
    endif()
else()
    message(STATUS "Allure-Cpp: Skipping tests and examples (included as subdirectory)")
endif()
