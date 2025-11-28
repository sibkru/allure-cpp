---
title: CppUTest Setup
description: How to integrate Allure C++ with CppUTest
---

## Prerequisites

- CMake 3.14 or newer
- C++17 compatible compiler
- CppUTest installed or available via CMake

## CMake Configuration

Add allure-cpp to your project using CMake's FetchContent module. Enable the CppUTest adapter by setting the `ALLURE_ENABLE_CPPUTEST` option before making the content available.

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

# Enable CppUTest support (disabled by default)
set(ALLURE_ENABLE_CPPUTEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(AllureCpp)

# Your test executable
add_executable(MyTests tests/MyTests.cpp)
target_link_libraries(MyTests PRIVATE AllureCpp CppUTest)
```

The library expects you to provide your own CppUTest target. It only fetches CppUTest when building the allure-cpp repository standalone.

## Main File Setup

Create your test runner's main file with the Allure adapter initialized. The adapter must be included after your standard headers to avoid conflicts with CppUTest's memory override macros.

```cpp 
// Include your headers first so they aren't compiled under CppUTest's new/delete overrides
#include <allure-cpp.h>

// CppUTest includes come last
#include <CppUTest/CommandLineTestRunner.h>
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    // Initialize Allure and run tests with the Allure-enabled runner
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    return allure::adapters::cpputest::RunAllureEnabledTests(argc, argv);
}
```

The `AllureCppUTest` constructor configures the Allure output directory and framework metadata. The `RunAllureEnabledTests` function wraps the standard CppUTest runner to capture test results and write them to the Allure format.

## Custom Test Output

The default setup uses CppUTest's standard console output. If you need custom output formatting (TeamCity, JUnit, etc.), you can provide your own output implementation.

```cpp 
#include <allure-cpp.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TeamCityTestOutput.h>
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    auto* output = new TeamCityTestOutput();
    return allure::adapters::cpputest::RunAllureEnabledTests(argc, argv, output);
}
```

The Allure adapter wraps your output implementation to capture failure details while preserving your chosen output format.
