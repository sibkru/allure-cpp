#include "allure-cpp.h"
#include <gtest/gtest.h>
#include <chrono>
#include <cstring>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <strings.h>
#include "../shared/Calculator.h"

using namespace allure;

/**
 * @brief Sample test suite to demonstrate Allure reporting with the new modern API
 *
 * This file showcases all features of the modern allure-cpp API including:
 * - RAII-based step guards (automatic cleanup)
 * - Fluent metadata builders (no .apply() needed)
 * - Simplified attachment API
 * - Modern C++17 style with string formatting
 */

class BasicTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("Basic Test Suite")
			.description("A simple test suite to verify API compatibility")
			.epic("Core Functionality")
			.severity("critical")
			.label("tmsId", "API-COMPAT-001");
	}
};

TEST_F(BasicTestSuite, testSimplePass)
{
	test()
		.name("Simple passing test")
		.feature("Basic Operations")
		.story("User can execute simple tests");

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	EXPECT_TRUE(true);
	EXPECT_EQ(1, 1);
}

TEST_F(BasicTestSuite, testWithSteps)
{
	test()
		.name("Test with multiple steps")
		.feature("Step-by-Step Execution")
		.story("User can track test execution with detailed steps");

	int result = 0;

	step("Initialize value to 5", [&]() {
		result = 5;
	});

	step("Value should be 5", [&]() {
		EXPECT_EQ(5, result);
	});

	step("Add 3 to value", [&]() {
		result += 3;
	});

	step("Value should be 8", [&]() {
		EXPECT_EQ(8, result);
	});
}

TEST_F(BasicTestSuite, testWithStatusQuery)
{
	test().name("Test demonstrating status provider");

	// Perform some assertions
	EXPECT_TRUE(true);

	// Query status (still available via Core)
	auto statusProvider = detail::Core::instance().getStatusProvider();
	EXPECT_FALSE(statusProvider->isCurrentTestFailed());
	EXPECT_FALSE(statusProvider->isCurrentTestSkipped());
}


class ParametricTestSuite : public testing::TestWithParam<int>
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("Parametric Test Suite")
			.description("Tests with parameters")
			.epic("Phase 3 Validation")
			.severity("normal");
	}
};

TEST_P(ParametricTestSuite, testWithParameter)
{
	int param = GetParam();
	test().name("Test with parameter: " + std::to_string(param));

	// Keep a tiny pause to demonstrate timing without slowing the suite
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	EXPECT_GE(param, 0);
	EXPECT_LT(param, 100);
}

INSTANTIATE_TEST_SUITE_P(
	NumberRange,
	ParametricTestSuite,
	testing::Values(10, 20, 30)
);


class ComplexTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("Complex Test Suite")
			.description("Complex scenarios with nested steps")
			.epic("Advanced Features")
			.severity("high")
			.label("feature", "api-compatibility")
			.label("component", "allure-reporting");
	}
};

TEST_F(ComplexTestSuite, testNestedSteps)
{
	test()
		.name("Test with nested operations")
		.feature("Data Structures")
		.story("User can manipulate collections with validation");

	std::vector<int> numbers;

	step("Initialize empty vector", [&]() {
		numbers.clear();
	});

	step("Vector should be empty", [&]() {
		EXPECT_EQ(0, numbers.size());
	});

	step("Add three numbers", [&]() {
		numbers.push_back(1);
		numbers.push_back(2);
		numbers.push_back(3);
	});

	step("Vector should have 3 elements", [&]() {
		EXPECT_EQ(3, numbers.size());
	});

	step("Sum should be 6", [&]() {
		int sum = 0;
		for (int n : numbers) {
			sum += n;
		}
		EXPECT_EQ(6, sum);
	});
}

TEST_F(ComplexTestSuite, testMultipleAssertions)
{
	test()
		.name("Test with multiple assertions")
		.feature("String Operations")
		.story("User can validate string properties");

	std::string text = "Hello, Allure!";

	step("Text should not be empty", [&]() {
		EXPECT_FALSE(text.empty());
	});

	step("Text should contain 'Allure'", [&]() {
		EXPECT_NE(text.find("Allure"), std::string::npos);
	});

	step("Text length should be 14", [&]() {
		EXPECT_EQ(14, text.length());
	});
}


// Test Suite with Failures (for demonstrating categories)
class FailureExamplesTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("Failure Examples")
			.description("Examples of different failure types for category testing")
			.epic("Testing")
			.severity("normal");
	}
};

TEST_F(FailureExamplesTestSuite, testProductDefect)
{
	test().name("Product defect - calculation error");

	step("Perform calculation", []() {
		// Intentional failure to demonstrate category
	});

	step("Result should be correct", []() {
		EXPECT_EQ(2 + 2, 5) << "Calculation failed - this is a product defect";
	});
}

TEST_F(FailureExamplesTestSuite, testBrokenTest)
{
	test().name("Broken test - runtime error");

	step("Access invalid pointer", []() {
		// Simulate a test defect (broken test infrastructure)
		std::string* ptr = nullptr;
		if (ptr == nullptr) {
			throw std::runtime_error("RuntimeException: Test infrastructure is broken");
		}
	});
}

TEST_F(FailureExamplesTestSuite, testAnotherFailure)
{
	test().name("Another product failure");

	step("String comparison should work", []() {
		std::string actual = "Hello";
		std::string expected = "World";
		EXPECT_EQ(actual, expected) << "String mismatch - product defect";
	});
}

TEST_F(FailureExamplesTestSuite, testCheckFailure)
{
	test().name("Generic check failure");
	EXPECT_TRUE(false) << "Expected condition to be true";
}

TEST_F(FailureExamplesTestSuite, testCheckTextFailure)
{
	test().name("Check with message failure");
	EXPECT_TRUE(false) << "CheckText message should appear";
}

TEST_F(FailureExamplesTestSuite, testCheckFalseFailure)
{
	test().name("Check false failure");
	EXPECT_FALSE(true) << "Expected condition to be false";
}

TEST_F(FailureExamplesTestSuite, testCheckEqualFailure)
{
	test().name("Check equal failure");
	EXPECT_EQ(10, 20);
}

TEST_F(FailureExamplesTestSuite, testCheckCompareFailure)
{
	test().name("Check compare failure");
	int a = 5;
	int b = 1;
	EXPECT_LT(a, b);
}

TEST_F(FailureExamplesTestSuite, testCheckThrowsFailure)
{
	test().name("Check throws failure");
	EXPECT_THROW([]() { /* no throw */ }(), std::runtime_error);
}

TEST_F(FailureExamplesTestSuite, testStrnCmpEqualFailure)
{
	test().name("STRNCMP failure");
	EXPECT_EQ(0, strncmp("Hello", "Help", 4));
}

TEST_F(FailureExamplesTestSuite, testStrCmpNoCaseFailure)
{
	test().name("STRCMP no-case failure");
	EXPECT_EQ(0, strcasecmp("Allure", "aLLuRe?"));
}

TEST_F(FailureExamplesTestSuite, testStrCmpContainsFailure)
{
	test().name("STRCMP contains failure");
	std::string actual = "haystack";
	EXPECT_NE(actual.find("needle"), std::string::npos);
}

TEST_F(FailureExamplesTestSuite, testLongsEqualFailure)
{
	test().name("Longs comparison failure");
	EXPECT_EQ(123, 321);
}

TEST_F(FailureExamplesTestSuite, testUnsignedLongsEqualFailure)
{
	test().name("Unsigned longs failure");
	unsigned long a = 1;
	unsigned long b = 2;
	EXPECT_EQ(a, b);
}

TEST_F(FailureExamplesTestSuite, testDoublesEqualFailure)
{
	test().name("Double comparison failure");
	EXPECT_NEAR(1.0, 1.123, 0.01);
}

TEST_F(FailureExamplesTestSuite, testBytesEqualFailure)
{
	test().name("Byte comparison failure");
	uint8_t expected = static_cast<uint8_t>('A');
	uint8_t actual = static_cast<uint8_t>('B');
	EXPECT_EQ(expected, actual);
}

TEST_F(FailureExamplesTestSuite, testMemcmpEqualFailure)
{
	test().name("Memory comparison failure");
	const char expected[] = "foo";
	const char actual[] = "f0o";
	EXPECT_EQ(0, memcmp(expected, actual, sizeof(expected) - 1));
}

TEST_F(FailureExamplesTestSuite, testPointersEqualFailure)
{
	test().name("Pointer comparison failure");
	int x = 0;
	int y = 0;
	int* px = &x;
	int* py = &y;
	EXPECT_EQ(px, py) << "Pointers should match";
}

void expectedFunction() {}
void actualFunction() {}

TEST_F(FailureExamplesTestSuite, testFunctionPointersEqualFailure)
{
	test().name("Function pointer comparison failure");
	EXPECT_EQ(&expectedFunction, &actualFunction);
}

TEST_F(FailureExamplesTestSuite, testBitsEqualFailure)
{
	test().name("Bits comparison failure");
	uint8_t expected = 0b11110000;
	uint8_t actual = 0b11000011;
	uint8_t mask = 0xFF;
	EXPECT_EQ(expected & mask, actual & mask);
}

TEST_F(FailureExamplesTestSuite, testExplicitFail)
{
	test().name("Explicit fail");
	GTEST_FAIL() << "Forced failure";
}


// Test Suite demonstrating new features: nested steps with RAII and test marking
class NewFeaturesTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("New Features Demo")
			.description("Demonstrating RAII steps and test marking features")
			.epic("Phase 3 Features")
			.severity("high");
	}
};

TEST_F(NewFeaturesTestSuite, testWithNestedSteps)
{
	test().name("Test with nested steps (RAII - automatic cleanup)");

	step("Process user registration", []() {
		step("Validate user input", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		});

		step("Check username availability", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		});

		step("Hash password", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		});

		step("Save to database", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		});
	});

	step("User should be registered", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testWithDeeplyNestedSteps)
{
	test().name("Test with deeply nested substeps");

	step("Execute complex workflow", []() {
		step("Phase 1: Initialize", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			step("Load configuration", []() {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			});

			step("Connect to services", []() {
				std::this_thread::sleep_for(std::chrono::milliseconds(8));
			});
		});

		step("Phase 2: Process data", []() {
			std::this_thread::sleep_for(std::chrono::milliseconds(15));

			step("Fetch data", []() {
				std::this_thread::sleep_for(std::chrono::milliseconds(7));
			});

			step("Transform data", []() {
				std::this_thread::sleep_for(std::chrono::milliseconds(12));
			});
		});
	});

	step("Workflow should complete successfully", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsFlaky)
{
	test()
		.name("Flaky test example")
		.flaky();

	// Simulate a test that sometimes passes, sometimes fails
	// In this example, we'll make it pass, but it's marked as flaky
	step("Execute unreliable operation", []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	});

	step("Operation should succeed (but is unreliable)", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsKnown)
{
	test()
		.name("Known issue test")
		.known();

	// This test demonstrates a known issue that has been documented
	step("Known issue: will fail until bug #123 is fixed", []() {
		EXPECT_EQ(2 + 2, 5) << "This is a known issue - see ticket BUG-123";
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsMuted)
{
	test()
		.name("Muted test example")
		.muted();

	// This test is muted - failures are expected and ignored
	step("This failure is muted", []() {
		EXPECT_TRUE(false) << "This test is temporarily disabled";
	});
}

TEST_F(NewFeaturesTestSuite, testWithFormattedSteps)
{
	test()
		.name("Test with formatted step names")
		.feature("Modern C++17 API")
		.story("User can use format strings in step names");

	std::string username = "testuser";
	int userId = 12345;

	// New feature: formatted step names with fmt syntax
	step("Login as user: {}", username, [&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	step("User ID should be: {}", userId, [&]() {
		EXPECT_EQ(userId, 12345);
	});

	step("Process {} items for user {}", 42, username, [&]() {
		EXPECT_TRUE(true);
	});
}
