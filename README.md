# allure-cpp

> **DISCLAIMER**: This is **NOT** an official Allure project. This is an independent library for generating Allure reports from C++ Tests. For official Allure framework implementations, please visit [https://github.com/allure-framework](https://github.com/allure-framework).

A framework-agnostic C++ library for generating [Allure 2](https://docs.qameta.io/allure/) C++ test reports.

**Supported Test Frameworks:**
- [GoogleTest](https://github.com/google/googletest) - [View Example Report](https://sibkru.github.io/allure-cpp/googletest-example/)
- [CppUTest](https://cpputest.github.io/) - [View Example Report](https://sibkru.github.io/allure-cpp/cpputest-example/)

The library features a generic adapter interface that makes it easy to add support for additional C++ testing frameworks.

## Quick Start

### GoogleTest

Add to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTestProject)

set(CMAKE_CXX_STANDARD 17)  # Minimum C++17 required

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

set(CMAKE_CXX_STANDARD 17)  # Minimum C++17 required

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
#include "allure-cpp.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    allure::adapters::googletest::AllureGTest allureHelper;
    return RUN_ALL_TESTS();
}
```

**Test file** (`tests/MyTests.cpp`):

```cpp
#include "allure-cpp.h"
#include <gtest/gtest.h>

class CalculatorTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        suite()
            .name("Calculator Test Suite")
            .description("Tests for basic calculator operations");
    }
};

TEST_F(CalculatorTests, AdditionTest) {
    test()
        .name("Verify addition of two numbers")
        .severity("critical")
        .feature("Arithmetic Operations");

    step("Perform addition", []() {
        int result = 2 + 3;
        EXPECT_EQ(5, result);
    });
}
```

### CppUTest

**Main file** (`main.cpp`):

```cpp
#include "allure-cpp.h"
// Include your headers first so they aren't compiled under CppUTest's new/delete overrides.
#include <CppUTest/CommandLineTestRunner.h>
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    // Initializes Allure (output folder + framework name) and runs tests with
    // the Allure-enabled runner. No custom output: uses CppUTest defaults.
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    return allure::adapters::cpputest::RunAllureEnabledTests(argc, argv);
}
```

Keep your own project/standard headers (including `allure-cpp.h`) above the
CppUTest include. Place CppUTest at the end of your include list and include the
Allure adapter immediately after it so nothing else is processed under
CppUTest's macro overrides.

**Test file** (`tests/MyTests.cpp`):

```cpp
#include "allure-cpp.h"
#include <CppUTest/TestHarness.h>

TEST_GROUP(CalculatorTests)
{
};

TEST(CalculatorTests, AdditionTest)
{
    test()
        .name("Verify addition of two numbers")
        .severity("critical")
        .feature("Arithmetic Operations");

    step("Perform addition", []() {
        int result = 2 + 3;
        CHECK_EQUAL(5, result);
    });
}
```

**Build and run:**

```bash
mkdir build && cd build
cmake .. -DALLURE_ENABLE_CPPUTEST=ON
cmake --build .
./MyTests

# Generate Allure report
allure serve allure-results
```

## Executor metadata (history trend)

- The library writes `allure-results/executor.json` on each run so Allure can build history trends. By default, `buildOrder` is a short counter (last history `buildOrder` + 1, starting at 1) and `buildName` is a readable timestamp.
- Override values via API (`AllureAPI::setExecutorBuildName`, `AllureAPI::setExecutorBuildOrder`) or env vars (`ALLURE_BUILD_NAME`, `ALLURE_BUILD_ORDER`, `ALLURE_EXECUTOR_NAME`).
- To keep a pre-generated `executor.json`, set `ALLURE_EXECUTOR_PRESERVE=1`; otherwise the file is overwritten on each run to avoid stale metadata.

## Build from Source

**Requirements:**
- CMake 3.14+
- C++17 compiler or newer
- Git

**Build:**

```bash
git clone https://github.com/sibkru/allure-cpp.git
cd allure-cpp
mkdir build && cd build
cmake .. -DALLURE_ENABLE_GOOGLETEST=ON  # and/or ALLURE_ENABLE_CPPUTEST=ON
cmake --build .
```

Examples require `-DALLURE_BUILD_EXAMPLES=ON`. Tests live under `./build/bin/UnitTest` and `./build/bin/IntegrationTest`. See [BUILD.md](BUILD.md) for detailed options.

## Attribution

This project originated as a fork of [cpp-gtest-allure-utilities](https://github.com/systelab/cpp-gtest-allure-utilities) by Systelab Technologies. It has been substantially refactored to support multiple test frameworks and the Allure 2 format. See [ATTRIBUTION.md](ATTRIBUTION.md) for details.

## License

MIT License - See [LICENSE](LICENSE) for details.
