# allure-cpp

> **DISCLAIMER**: This is **NOT** an official Allure project. This is an independent library for generating Allure reports from C++ Tests. For official Allure framework implementations, please visit [https://github.com/allure-framework](https://github.com/allure-framework).

A framework-agnostic C++ library for generating [Allure 2](https://docs.qameta.io/allure/) C++ test reports.

**Supported Test Frameworks:**
- [GoogleTest](https://github.com/google/googletest)
- [CppUTest](https://cpputest.github.io/)

The library features a generic adapter interface that makes it easy to add support for additional C++ testing frameworks.

## Quick Start

### GoogleTest

Add to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTestProject)

set(CMAKE_CXX_STANDARD 14)  # Minimum C++14 required

# Fetch allure-cpp
include(FetchContent)
FetchContent_Declare(
  AllureCpp
  GIT_REPOSITORY https://github.com/sibkru/allure-cpp.git
  GIT_TAG        main
)
# Enable GoogleTest support (default is OFF, must be opted in)
set(ALLURE_ENABLE_GOOGLETEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(AllureCpp)

# Your test executable
add_executable(MyTests tests/MyTests.cpp)
target_link_libraries(MyTests PRIVATE AllureCpp gtest gtest_main)
```

### CppUTest

Add to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTestProject)

set(CMAKE_CXX_STANDARD 14)  # Minimum C++14 required

# Fetch allure-cpp
include(FetchContent)
FetchContent_Declare(
  AllureCpp
  GIT_REPOSITORY https://github.com/sibkru/allure-cpp.git
  GIT_TAG        main
)
# Enable CppUTest support (adapters are OFF by default)
set(ALLURE_ENABLE_CPPUTEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(AllureCpp)

# Your test executable
add_executable(MyTests tests/MyTests.cpp)
target_link_libraries(MyTests PRIVATE AllureCpp CppUTest)
```

## Basic Usage

### GoogleTest

**Main file** (`main.cpp`):

```cpp
#include <gtest/gtest.h>
#include "Framework/Adapters/GoogleTest/AllureGTest.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    allure_cpp::adapters::googletest::AllureGTest allureHelper;
    return RUN_ALL_TESTS();
}
```

**Test file** (`tests/MyTests.cpp`):

```cpp
#include <gtest/gtest.h>
#include "AllureAPI.h"

class CalculatorTests : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        allure_cpp::AllureAPI::setTestSuiteName("Calculator Test Suite");
        allure_cpp::AllureAPI::setTestSuiteDescription("Tests for basic calculator operations");
    }
};

TEST_F(CalculatorTests, AdditionTest)
{
    allure_cpp::AllureAPI::setTestCaseName("Verify addition of two numbers");
    allure_cpp::AllureAPI::addSeverity("critical");
    allure_cpp::AllureAPI::addFeature("Arithmetic Operations");

    allure_cpp::AllureAPI::addAction("Perform addition", []() {
        int result = 2 + 3;
        EXPECT_EQ(5, result);
    });
}
```

### CppUTest

**Main file** (`main.cpp`):

```cpp
#include <CppUTest/CommandLineTestRunner.h>
#include "Framework/Adapters/CppUTest/AllureCppUTest.h"
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    // Initializes Allure (output folder + framework name) and runs tests with
    // the Allure-enabled runner. No custom output: uses CppUTest defaults.
    allure_cpp::adapters::cpputest::AllureCppUTest allureHelper;
    return allure_cpp::adapters::cpputest::RunAllureEnabledTests(argc, argv);
}
```

**Test file** (`tests/MyTests.cpp`):

**Note:** Include `AllureAPI.h` before `<CppUTest/TestHarness.h>` to avoid conflicts with CppUTest's memory leak detection macros.

```cpp
#include "AllureAPI.h"
#include <CppUTest/TestHarness.h>

TEST_GROUP(CalculatorTests)
{
};

TEST(CalculatorTests, AdditionTest)
{
    allure_cpp::AllureAPI::setTestCaseName("Verify addition of two numbers");
    allure_cpp::AllureAPI::addSeverity("critical");
    allure_cpp::AllureAPI::addFeature("Arithmetic Operations");

    allure_cpp::AllureAPI::addAction("Perform addition", []() {
        int result = 2 + 3;
        CHECK_EQUAL(5, result);
    });
}
```

**Build and run:**

```bash
mkdir build && cd build
cmake ..
cmake --build .
./MyTests

# Generate Allure report
allure serve allure-results
```

## Build from Source

**Requirements:**
- CMake 3.14+
- C++14 compiler or newer
- Git

**Build:**

```bash
git clone https://github.com/sibkru/allure-cpp.git
cd allure-cpp
mkdir build && cd build
cmake ..
cmake --build .
```

See [BUILD.md](BUILD.md) for detailed build options.

## Attribution

This project originated as a fork of [cpp-gtest-allure-utilities](https://github.com/systelab/cpp-gtest-allure-utilities) by Systelab Technologies. It has been substantially refactored to support multiple test frameworks and the Allure 2 format. See [ATTRIBUTION.md](ATTRIBUTION.md) for details.

## License

MIT License - See [LICENSE](LICENSE) for details.
