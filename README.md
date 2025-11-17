# allure-cpp

> **DISCLAIMER**: This is **NOT** an official Allure project. This is an independent library for generating Allure reports from C++ Tests. For official Allure framework implementations, please visit [https://github.com/allure-framework](https://github.com/allure-framework).

A framework-agnostic C++ library for generating [Allure 2](https://docs.qameta.io/allure/) reports from C++ test frameworks.

Currently supports [GoogleTest](https://github.com/google/googletest), with a generic interface designed for extending to other C++ testing frameworks.

## Quick Start

Add to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTestProject)

set(CMAKE_CXX_STANDARD 14)  # Minimum C++14 required

# Fetch allure-cpp
include(FetchContent)
FetchContent_Declare(
  GTestAllureUtilities
  GIT_REPOSITORY https://github.com/sibkru/allure-cpp.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(GTestAllureUtilities)

# Your test executable
add_executable(MyTests tests/MyTests.cpp)
target_link_libraries(MyTests PRIVATE GTestAllureUtilities gtest gtest_main)
```

## Basic Usage

**Main file** (`main.cpp`):

```cpp
#include <gtest/gtest.h>
#include "GTestAllureUtilities/AllureAPI.h"

using namespace allure_cpp;

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    // Initialize Allure reporting
    AllureAPI::initializeGoogleTest();
    AllureAPI::setOutputFolder("./allure-results");

    return RUN_ALL_TESTS();
}
```

**Test file** (`tests/MyTests.cpp`):

```cpp
#include <gtest/gtest.h>
#include "GTestAllureUtilities/AllureAPI.h"

using namespace allure_cpp;

class CalculatorTests : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        AllureAPI::setTestSuiteName("Calculator Test Suite");
        AllureAPI::setTestSuiteDescription("Tests for basic calculator operations");
    }
};

TEST_F(CalculatorTests, AdditionTest)
{
    AllureAPI::setTestCaseName("Verify addition of two numbers");
    AllureAPI::addSeverity("critical");
    AllureAPI::addFeature("Arithmetic Operations");

    AllureAPI::addAction("Perform addition", []() {
        int result = 2 + 3;
        EXPECT_EQ(5, result);
    });
}

TEST_F(CalculatorTests, MultiStepTest)
{
    AllureAPI::setTestCaseName("Complex calculation with multiple steps");

    int value = 10;

    AllureAPI::addAction("Initialize value", [&value]() {
        EXPECT_EQ(10, value);
    });

    AllureAPI::addAction("Multiply by 2", [&value]() {
        value *= 2;
    });

    AllureAPI::addExpectedResult("Value should be 20", [value]() {
        EXPECT_EQ(20, value);
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

# Run tests
ctest
```

See [BUILD.md](BUILD.md) for detailed build options.

## Attribution

This project originated as a fork of [cpp-gtest-allure-utilities](https://github.com/systelab/cpp-gtest-allure-utilities) by Systelab Technologies. It has been substantially refactored to support multiple test frameworks and the Allure 2 format. See [ATTRIBUTION.md](ATTRIBUTION.md) for details.

## License

MIT License - See [LICENSE](LICENSE) for details.
