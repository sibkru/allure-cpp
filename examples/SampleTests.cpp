#include "AllureAPI.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

using namespace allure_cpp;

/**
 * @brief Sample test suite to demonstrate Allure reporting
 *
 * This file contains the same tests that will be run with both the old and new API
 * to verify they produce identical Allure reports.
 */

class BasicTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Basic Test Suite");
		AllureAPI::setTestSuiteDescription("A simple test suite to verify API compatibility");
		AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		AllureAPI::setTestSuiteSeverity("critical");
		AllureAPI::setTMSId("API-COMPAT-001");
	}
};

TEST_F(BasicTestSuite, testSimplePass)
{
	AllureAPI::setTestCaseName("Simple passing test");
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	EXPECT_TRUE(true);
	EXPECT_EQ(1, 1);
}

TEST_F(BasicTestSuite, testWithSteps)
{
	AllureAPI::setTestCaseName("Test with multiple steps");

	int result = 0;

	AllureAPI::addAction("Initialize value to 5", [&result]() {
		result = 5;
	});

	AllureAPI::addExpectedResult("Value should be 5", [result]() {
		EXPECT_EQ(5, result);
	});

	AllureAPI::addAction("Add 3 to value", [&result]() {
		result += 3;
	});

	AllureAPI::addExpectedResult("Value should be 8", [result]() {
		EXPECT_EQ(8, result);
	});
}

TEST_F(BasicTestSuite, testWithStatusQuery)
{
	AllureAPI::setTestCaseName("Test demonstrating status provider");

	// Perform some assertions
	EXPECT_TRUE(true);

	// Query status (new API feature)
	auto statusProvider = AllureAPI::getStatusProvider();
	EXPECT_FALSE(statusProvider->isCurrentTestFailed());
	EXPECT_FALSE(statusProvider->isCurrentTestSkipped());
}


class ParametricTestSuite : public testing::TestWithParam<int>
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Parametric Test Suite");
		AllureAPI::setTestSuiteDescription("Tests with parameters");
		AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		AllureAPI::setTestSuiteSeverity("normal");
	}
};

TEST_P(ParametricTestSuite, testWithParameter)
{
	int param = GetParam();
	AllureAPI::setTestCaseName("Test with parameter: " + std::to_string(param));

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
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
		AllureAPI::setTestSuiteName("Complex Test Suite");
		AllureAPI::setTestSuiteDescription("Complex scenarios with nested steps");
		AllureAPI::setTestSuiteEpic("Phase 3 Validation");
		AllureAPI::setTestSuiteSeverity("high");
		AllureAPI::setTestSuiteLabel("feature", "api-compatibility");
		AllureAPI::setTestSuiteLabel("component", "allure-reporting");
	}
};

TEST_F(ComplexTestSuite, testNestedSteps)
{
	AllureAPI::setTestCaseName("Test with nested operations");

	std::vector<int> numbers;

	AllureAPI::addAction("Initialize empty vector", [&numbers]() {
		numbers.clear();
	});

	AllureAPI::addExpectedResult("Vector should be empty", [&numbers]() {
		EXPECT_EQ(0, numbers.size());
	});

	AllureAPI::addAction("Add three numbers", [&numbers]() {
		numbers.push_back(1);
		numbers.push_back(2);
		numbers.push_back(3);
	});

	AllureAPI::addExpectedResult("Vector should have 3 elements", [&numbers]() {
		EXPECT_EQ(3, numbers.size());
	});

	AllureAPI::addExpectedResult("Sum should be 6", [&numbers]() {
		int sum = 0;
		for (int n : numbers) {
			sum += n;
		}
		EXPECT_EQ(6, sum);
	});
}

TEST_F(ComplexTestSuite, testMultipleAssertions)
{
	AllureAPI::setTestCaseName("Test with multiple assertions");

	std::string text = "Hello, Allure!";

	AllureAPI::addExpectedResult("Text should not be empty", [&text]() {
		EXPECT_FALSE(text.empty());
	});

	AllureAPI::addExpectedResult("Text should contain 'Allure'", [&text]() {
		EXPECT_NE(text.find("Allure"), std::string::npos);
	});

	AllureAPI::addExpectedResult("Text length should be 14", [&text]() {
		EXPECT_EQ(14, text.length());
	});
}


// Test Suite with Failures (for demonstrating categories)
class FailureExamplesTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Failure Examples");
		AllureAPI::setTestSuiteDescription("Examples of different failure types for category testing");
		AllureAPI::setTestSuiteEpic("Testing");
		AllureAPI::setTestSuiteSeverity("normal");
	}
};

TEST_F(FailureExamplesTestSuite, testProductDefect)
{
	AllureAPI::setTestCaseName("Product defect - calculation error");

	AllureAPI::addAction("Perform calculation", []() {
		// Intentional failure to demonstrate category
	});

	AllureAPI::addExpectedResult("Result should be correct", []() {
		EXPECT_EQ(2 + 2, 5) << "Calculation failed - this is a product defect";
	});
}

TEST_F(FailureExamplesTestSuite, testBrokenTest)
{
	AllureAPI::setTestCaseName("Broken test - runtime error");

	AllureAPI::addAction("Access invalid pointer", []() {
		// Simulate a test defect (broken test infrastructure)
		std::string* ptr = nullptr;
		if (ptr == nullptr) {
			throw std::runtime_error("RuntimeException: Test infrastructure is broken");
		}
	});
}

TEST_F(FailureExamplesTestSuite, testAnotherFailure)
{
	AllureAPI::setTestCaseName("Another product failure");

	AllureAPI::addExpectedResult("String comparison should work", []() {
		std::string actual = "Hello";
		std::string expected = "World";
		EXPECT_EQ(actual, expected) << "String mismatch - product defect";
	});
}


// Test Suite demonstrating new features: substeps and test marking
class NewFeaturesTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("New Features Demo");
		AllureAPI::setTestSuiteDescription("Demonstrating substeps and test marking features");
		AllureAPI::setTestSuiteEpic("Phase 3 Features");
		AllureAPI::setTestSuiteSeverity("high");
	}
};

TEST_F(NewFeaturesTestSuite, testWithManualSubsteps)
{
	AllureAPI::setTestCaseName("Test with manual substeps");

	AllureAPI::addAction("Process user registration", []() {
		AllureAPI::beginSubStep("Validate user input");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Check username availability");
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Hash password");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Save to database");
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
		AllureAPI::endSubStep();
	});

	AllureAPI::addExpectedResult("User should be registered", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testWithNestedSubsteps)
{
	AllureAPI::setTestCaseName("Test with nested substeps");

	AllureAPI::addAction("Execute complex workflow", []() {
		AllureAPI::beginSubStep("Phase 1: Initialize");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		AllureAPI::beginSubStep("Load configuration");
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Connect to services");
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
		AllureAPI::endSubStep();

		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Phase 2: Process data");
		std::this_thread::sleep_for(std::chrono::milliseconds(15));

		AllureAPI::beginSubStep("Fetch data");
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Transform data");
		std::this_thread::sleep_for(std::chrono::milliseconds(12));
		AllureAPI::endSubStep();

		AllureAPI::endSubStep();
	});

	AllureAPI::addExpectedResult("Workflow should complete successfully", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsFlaky)
{
	AllureAPI::setTestCaseName("Flaky test example");
	AllureAPI::markTestAsFlaky();

	// Simulate a test that sometimes passes, sometimes fails
	// In this example, we'll make it pass, but it's marked as flaky
	AllureAPI::addAction("Execute unreliable operation", []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	});

	AllureAPI::addExpectedResult("Operation should succeed (but is unreliable)", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsKnown)
{
	AllureAPI::setTestCaseName("Known issue test");
	AllureAPI::markTestAsKnown();

	// This test demonstrates a known issue that has been documented
	AllureAPI::addExpectedResult("Known issue: will fail until bug #123 is fixed", []() {
		EXPECT_EQ(2 + 2, 5) << "This is a known issue - see ticket BUG-123";
	});
}

TEST_F(NewFeaturesTestSuite, testMarkedAsMuted)
{
	AllureAPI::setTestCaseName("Muted test example");
	AllureAPI::markTestAsMuted();

	// This test is muted - failures are expected and ignored
	AllureAPI::addExpectedResult("This failure is muted", []() {
		EXPECT_TRUE(false) << "This test is temporarily disabled";
	});
}
