#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "AllureAPI.h"
#include "../shared/Calculator.h"
#include "Framework/Adapters/CppUTest/AllureCppUTest.h"
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

//==============================================================================
// Basic Test Suite
//==============================================================================

TEST_GROUP(BasicTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("Basic Test Suite");
		allure_cpp::AllureAPI::setTestSuiteDescription("A simple test suite to verify API compatibility");
		allure_cpp::AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		allure_cpp::AllureAPI::setTestSuiteSeverity("critical");
		allure_cpp::AllureAPI::setTMSId("API-COMPAT-001");
    }
};

TEST(BasicTestSuite, testSimplePass)
{
	allure_cpp::AllureAPI::setTestCaseName("Simple passing test");
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK_TRUE(true);
	CHECK_EQUAL(1, 1);
}

TEST(BasicTestSuite, testWithSteps)
{
	allure_cpp::AllureAPI::setTestCaseName("Test with multiple steps");
	allure_cpp::AllureAPI::addFeature("Step-by-Step Execution");
	allure_cpp::AllureAPI::addStory("User can track test execution with detailed steps");

	int result = 0;

	allure_cpp::AllureAPI::addAction("Initialize value to 5", [&result]() {
		result = 5;
	});

	allure_cpp::AllureAPI::addExpectedResult("Value should be 5", [result]() {
		CHECK_EQUAL(5, result);
	});

	allure_cpp::AllureAPI::addAction("Add 3 to value", [&result]() {
		result += 3;
	});

	allure_cpp::AllureAPI::addExpectedResult("Value should be 8", [result]() {
		CHECK_EQUAL(8, result);
	});
}

TEST(BasicTestSuite, testWithStatusQuery)
{
	allure_cpp::AllureAPI::setTestCaseName("Test demonstrating status provider");

	// Perform some assertions
	CHECK_TRUE(true);

	// Query status (new API feature)
	auto statusProvider = allure_cpp::AllureAPI::getStatusProvider();
	CHECK_FALSE(statusProvider->isCurrentTestFailed());
	CHECK_FALSE(statusProvider->isCurrentTestSkipped());
}

//==============================================================================
// Parametric Test Suite
//==============================================================================

TEST_GROUP(ParametricTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("Parametric Test Suite");
		allure_cpp::AllureAPI::setTestSuiteDescription("Tests with parameters");
		allure_cpp::AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		allure_cpp::AllureAPI::setTestSuiteSeverity("normal");
    }
};

TEST(ParametricTestSuite, testWithParameter)
{
    // CppUTest doesn't have built-in parametric tests, so we simulate it with a loop
    for (int param : {10, 20, 30})
    {
        allure_cpp::AllureAPI::setTestCaseName("Test with parameter: " + std::to_string(param));

        // Keep a tiny pause to demonstrate timing without slowing the suite
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        CHECK_TRUE(param >= 0);
        CHECK_TRUE(param < 100);
    }
}

//==============================================================================
// Complex Test Suite
//==============================================================================

TEST_GROUP(ComplexTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("Complex Test Suite");
		allure_cpp::AllureAPI::setTestSuiteDescription("Complex scenarios with nested steps");
		allure_cpp::AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		allure_cpp::AllureAPI::setTestSuiteSeverity("high");
		allure_cpp::AllureAPI::setTestSuiteLabel("feature", "api-compatibility");
		allure_cpp::AllureAPI::setTestSuiteLabel("component", "allure-reporting");
    }
};

TEST(ComplexTestSuite, testNestedSteps)
{
	allure_cpp::AllureAPI::setTestCaseName("Test with nested operations");
	allure_cpp::AllureAPI::addFeature("Data Structures");
	allure_cpp::AllureAPI::addStory("User can manipulate collections with validation");

	std::vector<int> numbers;

	allure_cpp::AllureAPI::addAction("Initialize empty vector", [&numbers]() {
		numbers.clear();
	});

	allure_cpp::AllureAPI::addExpectedResult("Vector should be empty", [&numbers]() {
		CHECK_EQUAL(0, numbers.size());
	});

	allure_cpp::AllureAPI::addAction("Add three numbers", [&numbers]() {
		numbers.push_back(1);
		numbers.push_back(2);
		numbers.push_back(3);
	});

	allure_cpp::AllureAPI::addExpectedResult("Vector should have 3 elements", [&numbers]() {
		CHECK_EQUAL(3, numbers.size());
	});

	allure_cpp::AllureAPI::addExpectedResult("Sum should be 6", [&numbers]() {
		int sum = 0;
		for (int n : numbers) {
			sum += n;
		}
		CHECK_EQUAL(6, sum);
	});
}

TEST(ComplexTestSuite, testMultipleAssertions)
{
	allure_cpp::AllureAPI::setTestCaseName("Test with multiple assertions");
	allure_cpp::AllureAPI::addFeature("String Operations");
	allure_cpp::AllureAPI::addStory("User can validate string properties");

	std::string text = "Hello, Allure!";

	allure_cpp::AllureAPI::addExpectedResult("Text should not be empty", [&text]() {
		CHECK_FALSE(text.empty());
	});

	allure_cpp::AllureAPI::addExpectedResult("Text should contain 'Allure'", [&text]() {
		CHECK_TRUE(text.find("Allure") != std::string::npos);
	});

	allure_cpp::AllureAPI::addExpectedResult("Text length should be 14", [&text]() {
		CHECK_EQUAL(14, text.length());
	});
}

//==============================================================================
// Failure Examples Test Suite
//==============================================================================

TEST_GROUP(FailureExamplesTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("Failure Examples");
		allure_cpp::AllureAPI::setTestSuiteDescription("Examples of different failure types for category testing");
		allure_cpp::AllureAPI::setTestSuiteEpic("Testing");
		allure_cpp::AllureAPI::setTestSuiteSeverity("normal");
    }
};

TEST(FailureExamplesTestSuite, testProductDefect)
{
	allure_cpp::AllureAPI::setTestCaseName("Product defect - calculation error");

	allure_cpp::AllureAPI::addAction("Perform calculation", []() {
		// Intentional failure to demonstrate category
	});

	allure_cpp::AllureAPI::addExpectedResult("Result should be correct", []() {
		CHECK_EQUAL(5, 2 + 2);
	});
}

TEST(FailureExamplesTestSuite, testBrokenTest)
{
	allure_cpp::AllureAPI::setTestCaseName("Broken test - runtime error");

		allure_cpp::AllureAPI::addAction("Access invalid pointer", []() {
			// Simulate a test defect (broken test infrastructure)
	        try {
	            std::string* ptr = nullptr;
	            if (ptr == nullptr) {
	                throw std::runtime_error("RuntimeException: Test infrastructure is broken");
	            }
	        } catch (const std::runtime_error& e) {
	            FAIL("Runtime exception thrown");
	        }
		});
	}

TEST(FailureExamplesTestSuite, testAnotherFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Another product failure");

	allure_cpp::AllureAPI::addExpectedResult("String comparison should work", []() {
		std::string actual = "Hello";
		std::string expected = "World";
		STRCMP_EQUAL(expected.c_str(), actual.c_str());
	});
}

TEST(FailureExamplesTestSuite, testLongsEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Longs comparison failure");

	allure_cpp::AllureAPI::addExpectedResult("Long values should match", []() {
		LONGS_EQUAL(123, 321);
	});
}

TEST(FailureExamplesTestSuite, testDoublesEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Double comparison failure");

	allure_cpp::AllureAPI::addExpectedResult("Doubles should be within tolerance", []() {
		DOUBLES_EQUAL(1.0, 1.123, 0.01);
	});
}

TEST(FailureExamplesTestSuite, testBytesEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Byte comparison failure");

	allure_cpp::AllureAPI::addExpectedResult("Bytes should match", []() {
		BYTES_EQUAL('A', 'B');
	});
}

TEST(FailureExamplesTestSuite, testMemcmpEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Memory comparison failure");

	allure_cpp::AllureAPI::addExpectedResult("Buffers should match", []() {
		const char expected[] = "foo";
		const char actual[] = "f0o";
		MEMCMP_EQUAL(expected, actual, sizeof(expected) - 1);
	});
}

TEST(FailureExamplesTestSuite, testCheckFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Generic check failure");
	CHECK(false);
}

TEST(FailureExamplesTestSuite, testCheckTextFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Check with message failure");
	CHECK_TEXT(false, "CheckText message should appear");
}

TEST(FailureExamplesTestSuite, testCheckFalseFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Check false failure");
	CHECK_FALSE(true);
}

TEST(FailureExamplesTestSuite, testCheckEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Check equal failure");
	CHECK_EQUAL(10, 20);
}

TEST(FailureExamplesTestSuite, testCheckCompareFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Check compare failure");
	int a = 5;
	int b = 1;
	CHECK_COMPARE(a, <, b);
}

TEST(FailureExamplesTestSuite, testCheckThrowsFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Check throws failure");
	CHECK_THROWS(std::runtime_error, []() { /* no throw */ }());
}

TEST(FailureExamplesTestSuite, testStrnCmpEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("STRNCMP failure");
	STRNCMP_EQUAL("Hello", "Help", 4);
}

TEST(FailureExamplesTestSuite, testStrCmpNoCaseFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("STRCMP no-case failure");
	STRCMP_NOCASE_EQUAL("Allure", "aLLuRe?");
}

TEST(FailureExamplesTestSuite, testStrCmpContainsFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("STRCMP contains failure");
	STRCMP_CONTAINS("needle", "haystack");
}

TEST(FailureExamplesTestSuite, testUnsignedLongsEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Unsigned longs failure");
	UNSIGNED_LONGS_EQUAL(static_cast<unsigned long>(1), static_cast<unsigned long>(2));
}

TEST(FailureExamplesTestSuite, testPointersEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Pointer comparison failure");
	int x = 0;
	int y = 0;
	int* px = &x;
	int* py = &y;
	POINTERS_EQUAL(px, py);
}

void expectedFunction() {}
void actualFunction() {}

TEST(FailureExamplesTestSuite, testFunctionPointersEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Function pointer comparison failure");
	FUNCTIONPOINTERS_EQUAL(&expectedFunction, &actualFunction);
}

TEST(FailureExamplesTestSuite, testBitsEqualFailure)
{
	allure_cpp::AllureAPI::setTestCaseName("Bits comparison failure");
	uint8_t expected = 0b11110000;
	uint8_t actual = 0b11000011;
	uint8_t mask = 0xFF;
	BITS_EQUAL(expected, actual, mask);
}

TEST(FailureExamplesTestSuite, testExplicitFail)
{
	allure_cpp::AllureAPI::setTestCaseName("Explicit fail");
	FAIL("Forced failure");
}

//==============================================================================
// New Features Test Suite
//==============================================================================

TEST_GROUP(NewFeaturesTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("New Features Demo");
		allure_cpp::AllureAPI::setTestSuiteDescription("Demonstrating substeps and test marking features");
		allure_cpp::AllureAPI::setTestSuiteEpic("Phase 3 Features");
		allure_cpp::AllureAPI::setTestSuiteSeverity("high");
    }
};

TEST(NewFeaturesTestSuite, testWithManualSubsteps)
{
	allure_cpp::AllureAPI::setTestCaseName("Test with manual substeps");

	allure_cpp::AllureAPI::addAction("Process user registration", []() {
		allure_cpp::AllureAPI::beginSubStep("Validate user input");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Check username availability");
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Hash password");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Save to database");
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
		allure_cpp::AllureAPI::endSubStep();
	});

	allure_cpp::AllureAPI::addExpectedResult("User should be registered", []() {
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testWithNestedSubsteps)
{
	allure_cpp::AllureAPI::setTestCaseName("Test with nested substeps");

	allure_cpp::AllureAPI::addAction("Execute complex workflow", []() {
		allure_cpp::AllureAPI::beginSubStep("Phase 1: Initialize");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		allure_cpp::AllureAPI::beginSubStep("Load configuration");
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Connect to services");
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Phase 2: Process data");
		std::this_thread::sleep_for(std::chrono::milliseconds(15));

		allure_cpp::AllureAPI::beginSubStep("Fetch data");
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::beginSubStep("Transform data");
		std::this_thread::sleep_for(std::chrono::milliseconds(12));
		allure_cpp::AllureAPI::endSubStep();

		allure_cpp::AllureAPI::endSubStep();
	});

	allure_cpp::AllureAPI::addExpectedResult("Workflow should complete successfully", []() {
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsFlaky)
{
	allure_cpp::AllureAPI::setTestCaseName("Flaky test example");
	allure_cpp::AllureAPI::markTestAsFlaky();

	// Simulate a test that sometimes passes, sometimes fails
	// In this example, we'll make it pass, but it's marked as flaky
	allure_cpp::AllureAPI::addAction("Execute unreliable operation", []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	});

	allure_cpp::AllureAPI::addExpectedResult("Operation should succeed (but is unreliable)", []() {
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsKnown)
{
	allure_cpp::AllureAPI::setTestCaseName("Known issue test");
	allure_cpp::AllureAPI::markTestAsKnown();

	// This test demonstrates a known issue that has been documented
	allure_cpp::AllureAPI::addExpectedResult("Known issue: will fail until bug #123 is fixed", []() {
		CHECK_EQUAL(5, 2 + 2);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsMuted)
{
	allure_cpp::AllureAPI::setTestCaseName("Muted test example");
	allure_cpp::AllureAPI::markTestAsMuted();

	// This test is muted - failures are expected and ignored
	allure_cpp::AllureAPI::addExpectedResult("This failure is muted", []() {
		CHECK_TRUE(false);
	});
}
