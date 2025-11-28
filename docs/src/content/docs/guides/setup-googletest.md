---
title: GoogleTest Setup
description: How to integrate Allure C++ with GoogleTest
---

## Prerequisites

- CMake 3.14 or newer
- C++17 compatible compiler
- GoogleTest installed or available via CMake

## CMake Configuration

Add allure-cpp to your project using CMake's FetchContent module. Enable the GoogleTest adapter by setting the `ALLURE_ENABLE_GOOGLETEST` option before making the content available.

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTestProject)

set(CMAKE_CXX_STANDARD 17)

# Fetch allure-cpp
include(FetchContent)
FetchContent_Declare(
  AllureCpp
  GIT_REPOSITORY https://github.com/sibkru/allure-cpp.git
  GIT_TAG        main
)

# Enable GoogleTest support (disabled by default)
set(ALLURE_ENABLE_GOOGLETEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(AllureCpp)

# Your test executable
add_executable(MyTests tests/MyTests.cpp)
target_link_libraries(MyTests PRIVATE AllureCpp gtest gtest_main)
```

The library expects you to provide your own GoogleTest target. It only fetches GoogleTest when building the allure-cpp repository standalone.

## Main File Setup

Create your test runner's main file with the Allure adapter initialized. The adapter registers itself as a GoogleTest event listener to capture test results.

```cpp 
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    allure::adapters::googletest::AllureGTest allureHelper;
    return RUN_ALL_TESTS();
}
```

The `AllureGTest` constructor configures the Allure output directory, registers the event listener with GoogleTest's test framework, and sets up framework metadata. It automatically captures test execution and writes results to the Allure format.

## Using gtest_main

If you prefer to use GoogleTest's built-in `gtest_main` instead of writing your own main function, you can still integrate Allure by using a custom environment setup.

First, create a test listener initialization file:

```cpp 
namespace {
    // Initialize Allure before any tests run
    class AllureEnvironment : public ::testing::Environment {
    public:
        void SetUp() override {
            // Allure adapter is initialized as a static member
            // to ensure it lives for the entire test run
            static allure::adapters::googletest::AllureGTest allureHelper;
        }
    };

    // Register the environment
    ::testing::Environment* const allure_env =
        ::testing::AddGlobalTestEnvironment(new AllureEnvironment);
}
```

Then link against `gtest_main` instead of providing your own main:

```cmake
add_executable(MyTests tests/MyTests.cpp tests/AllureEnvironment.cpp)
target_link_libraries(MyTests PRIVATE AllureCpp gtest gtest_main)
```

This approach allows GoogleTest to handle command-line parsing and test discovery while still enabling Allure reporting.
