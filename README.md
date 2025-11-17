# allure-cpp

> **DISCLAIMER**: This is **NOT** an official Allure project. This is an independent C++ implementation for generating Allure reports, created and maintained by the community. For official Allure framework implementations, please visit [https://github.com/allure-framework](https://github.com/allure-framework).

A framework-agnostic C++ library for generating [Allure 2](https://docs.qameta.io/allure/) reports from C++ test frameworks.

Currently supports [GoogleTest](https://github.com/google/googletest), with a generic interface designed for extending to other C++ testing frameworks.

## Attribution

This project originated as a fork of [cpp-gtest-allure-utilities](https://github.com/systelab/cpp-gtest-allure-utilities) by Systelab Technologies. It has been substantially refactored to support multiple test frameworks and the Allure 2 format. See [ATTRIBUTION.md](ATTRIBUTION.md) for details.


## Setup

### Build from sources

This library uses **CMake with FetchContent** for dependency management. No package manager required!

```bash
git clone https://github.com/sibkru/allure-cpp.git
cd allure-cpp
mkdir build && cd build
cmake ..
cmake --build .
```

See [BUILD.md](BUILD.md) for detailed build instructions and options.

### Using in your project

To use this library in your CMake project, you can either:

1. **Add as a subdirectory** (if vendored):
```cmake
add_subdirectory(path/to/allure-cpp)
target_link_libraries(your_test_target PRIVATE GTestAllureUtilities)
```

2. **Use CMake FetchContent** (recommended):
```cmake
include(FetchContent)
FetchContent_Declare(
  GTestAllureUtilities
  GIT_REPOSITORY https://github.com/sibkru/allure-cpp.git
  GIT_TAG main
)
FetchContent_MakeAvailable(GTestAllureUtilities)
target_link_libraries(your_test_target PRIVATE GTestAllureUtilities)
```

See [FETCHCONTENT_INTEGRATION.md](FETCHCONTENT_INTEGRATION.md) for detailed integration examples.


## Usage

### Register library as a GoogleTest listener

This library needs to be registered as a listener of your GoogleTest program. There are two ways to do this:

#### New API (Recommended)

The streamlined approach using the framework-agnostic adapter pattern:

```cpp
#include <gtest/gtest.h>
#include "GTestAllureUtilities/AllureAPI.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	// Configure Allure output folder (optional, before initialization)
	systelab::gtest_allure::AllureAPI::setOutputFolder("./allure-results");

	// Initialize Allure with GoogleTest (automatically registers listener)
	systelab::gtest_allure::AllureAPI::initializeGoogleTest();

	return RUN_ALL_TESTS();
}
```

**Benefits:** Simpler, cleaner, automatic memory management, foundation for multi-framework support.

#### Old API (Still Fully Supported)

The traditional approach continues to work for backward compatibility:

```cpp
#include <gtest/gtest.h>
#include "GTestAllureUtilities/AllureAPI.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	::testing::UnitTest::GetInstance()->listeners()
		.Append(systelab::gtest_allure::AllureAPI::buildListener().release());

	return RUN_ALL_TESTS();
}
```

> **Note:** See [MIGRATION_GUIDE.md](MIGRATION_GUIDE.md) for detailed information about migrating to the new API.


### Configure output folder

The folder where JSON Allure reports will be generated can be defined by using the `AllureAPI::setOutputFolder(...)` method:

```cpp
systelab::gtest_allure::AllureAPI::setOutputFolder("Allure/Reports/Folder");
```
> This needs to be configured before executing the `RUN_ALL_TESTS()` macro of GoogleTest.


### Set name, description and TMS identifier of test suite

By default, each test suite will be named as the associated test class. However, this name can be adjusted with the `AllureAPI::setTestSuiteName(...)` method. Similarly, the `AllureAPI::setTestSuiteDescription(...)` and `AllureAPI::setTMSId(...)` methods allow setting the description and the TMS identifier of the test suite respectively.

```cpp
class MyTestSuite : public testing::Test
{
    static void SetUpTestSuite()
    {
        AllureAPI::setTestSuiteName("My test suite name"); // Default test suite is "MyTestSuite"
        AllureAPI::setTestSuiteDescription("This is the description for the MyTestSuite");
        AllureAPI::setTMSId("TC-AT-PROJ-001");
    }
};
```


### Set test macro description

For each test macro (`TEST_F` or `TEST_P`) in a suite, an action step will be added to the generated report. By default, these actions will have the name of the test fixture assigned by GoogleTest as a description. This description can be overwritten by making use of the `AllureAPI::setTestCaseName(...)` method:

```cpp
TEST_F(MyTestSuite, testSomething)
{
   // Default action description is "MyTestSuite.testSomething" (automatically assigned by GoogleTest)
   AllureAPI::setTestCaseName("Human readable description of test action");
}
```


### Define steps for a test macro

When a test macro is quite complex, it might be interesting to decompose it into several steps. Moreover, some these steps will correspond with actions and the rest to expected results. So, in order to facilitate the definition of this kind of test scenarios, the library provides the `AllureAPI::addAction(...)` and `AllureAPI::addExpectedResult(...)` methods. They require 2 arguments: a human readable description of the step and a lambda function with the code included on the step.

The following example illustrates how to define a test macro with multiple steps:

```cpp
TEST_F(MyTestSuite, testSumOfValuesVector)
{
    AllureAPI::setTestCaseName("Compute sum of 5 + 3");
    int result = 5 + 3;
   
    AllureAPI::addExpectedResult("Sum result is equal to 8", [result]() -> void
    {
        ASSERT_EQ(8, result);
    });
    
    AllureAPI::addAction("Add 22 to result of previous sum", [&result]() -> void
    {
        result += 22;
    });
    
    AllureAPI::addExpectedResult("New sum result is equal to 30", [result]() -> void
    {
        ASSERT_EQ(30, result);
    });
}
```


### Query test status at runtime

The new API provides runtime access to test execution status through the `getStatusProvider()` method. This allows you to check if a test has already failed or been skipped during execution:

```cpp
TEST_F(MyTestSuite, testWithExpensiveOperation)
{
    // Perform initial setup
    performInitialSetup();

    // Check if test has already failed
    auto statusProvider = systelab::gtest_allure::AllureAPI::getStatusProvider();
    if (statusProvider->isCurrentTestFailed())
    {
        // Skip expensive operations if test already failed
        return;
    }

    // Continue with expensive operations only if test hasn't failed
    performExpensiveOperation();
}
```

**Use cases:**
- Skip expensive cleanup operations if test has already failed
- Implement conditional test logic based on current test state
- Add custom logging based on test status
- Optimize test execution by detecting early failures

**Available methods:**
- `isCurrentTestFailed()` - Returns true if any assertion has failed
- `isCurrentTestSkipped()` - Returns true if test was skipped (e.g., via GTEST_SKIP())

> **Note:** The status provider works with both the old and new initialization APIs.


### Add labels to a test suite

Labels allow complementing the general information defined for each test suite. They can be recorded through the `AllureAPI::setTestSuiteLabel(...)` method. Additionally, the library provides built-in methods to include the most common labels. 

```cpp
class MyTestSuite : public testing::Test
{
    static void SetUpTestSuite()
    {
    	// ...
	
	// Add built-in labels
        AllureAPI::setTestSuiteEpic("Epic name of test suite");
        AllureAPI::setTestSuiteSeverity("high");
	
	// Add custom labels
        AllureAPI::setTestSuiteLabel("CustomLabel1", "Value for custom label 1");
        AllureAPI::setTestSuiteLabel("CustomLabel2", "Value for custom label 2");
    }
};
```

> Labels are designed to be unique per test suite. Thus, if a label is set twice on the same suite, then only the latest value provided will be included on the report.

### Additional test program options

#### Program name

The name of the test program (included in filename of generated report files) can be adjusted as follows:

```cpp
systelab::gtest_allure::AllureAPI::setTestProgramName("MyTestProjectName");
```

#### TMS Link patterns

The pattern of the links between each test suite and the test management system (TMS) needs to be configured before starting the test program:

```cpp
systelab::gtest_allure::AllureAPI::setTMSLinksPattern("https://mycompany.com/tms/{}");
```
> The `{}` will be replaced by the TMS identifier on the report of each test suite


### Examples

See [Sample Test project](test/SampleTestProject) for more complete usage examples.
