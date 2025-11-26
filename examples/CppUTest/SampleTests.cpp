#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "allure-cpp.h"
#include "../shared/Calculator.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

using namespace allure;

//==============================================================================
// Basic Test Suite
//==============================================================================

TEST_GROUP(BasicTestSuite)
{
    void setup() override
    {
        suite()
			.name("Basic Test Suite")
			.description("A simple test suite to verify API compatibility")
			.epic("Phase 3 Validation")
			.severity("critical")
			.label("tmsId", "API-COMPAT-001");
    }
};

TEST(BasicTestSuite, testSimplePass)
{
	test().name("Simple passing test");
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK_TRUE(true);
	CHECK_EQUAL(1, 1);
}

TEST(BasicTestSuite, testWithSteps)
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
		CHECK_EQUAL(5, result);
	});

	step("Add 3 to value", [&]() {
		result += 3;
	});

	step("Value should be 8", [&]() {
		CHECK_EQUAL(8, result);
	});
}

TEST(BasicTestSuite, testWithStatusQuery)
{
	test().name("Test demonstrating status provider");

	// Perform some assertions
	CHECK_TRUE(true);

	// Query status (still available via Core)
	auto statusProvider = detail::Core::instance().getStatusProvider();
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
        suite()
			.name("Parametric Test Suite")
			.description("Tests with parameters")
			.epic("Phase 3 Validation")
			.severity("normal");
    }
};

TEST(ParametricTestSuite, testWithParameter)
{
    // CppUTest doesn't have built-in parametric tests, so we simulate it with a loop
    for (int param : {10, 20, 30})
    {
        test().name("Test with parameter: " + std::to_string(param));

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
        suite()
			.name("Complex Test Suite")
			.description("Complex scenarios with nested steps")
			.epic("Phase 3 Validation")
			.severity("high")
			.label("feature", "api-compatibility")
			.label("component", "allure-reporting");
    }
};

TEST(ComplexTestSuite, testNestedSteps)
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
		CHECK_EQUAL(0, numbers.size());
	});

	step("Add three numbers", [&]() {
		numbers.push_back(1);
		numbers.push_back(2);
		numbers.push_back(3);
	});

	step("Vector should have 3 elements", [&]() {
		CHECK_EQUAL(3, numbers.size());
	});

	step("Sum should be 6", [&]() {
		int sum = 0;
		for (int n : numbers) {
			sum += n;
		}
		CHECK_EQUAL(6, sum);
	});
}

TEST(ComplexTestSuite, testMultipleAssertions)
{
	test()
		.name("Test with multiple assertions")
		.feature("String Operations")
		.story("User can validate string properties");

	std::string text = "Hello, Allure!";

	step("Text should not be empty", [&]() {
		CHECK_FALSE(text.empty());
	});

	step("Text should contain 'Allure'", [&]() {
		CHECK_TRUE(text.find("Allure") != std::string::npos);
	});

	step("Text length should be 14", [&]() {
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
        suite()
			.name("Failure Examples")
			.description("Examples of different failure types for category testing")
			.epic("Testing")
			.severity("normal");
    }
};

TEST(FailureExamplesTestSuite, testProductDefect)
{
	test().name("Product defect - calculation error");

	step("Perform calculation", []() {
		// Intentional failure to demonstrate category
	});

	step("Result should be correct", []() {
		CHECK_EQUAL(5, 2 + 2);
	});
}

TEST(FailureExamplesTestSuite, testBrokenTest)
{
	test().name("Broken test - runtime error");

	step("Access invalid pointer", []() {
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
	test().name("Another product failure");

	step("String comparison should work", []() {
		std::string actual = "Hello";
		std::string expected = "World";
		STRCMP_EQUAL(expected.c_str(), actual.c_str());
	});
}

TEST(FailureExamplesTestSuite, testLongsEqualFailure)
{
	test().name("Longs comparison failure");

	step("Long values should match", []() {
		LONGS_EQUAL(123, 321);
	});
}

TEST(FailureExamplesTestSuite, testDoublesEqualFailure)
{
	test().name("Double comparison failure");

	step("Doubles should be within tolerance", []() {
		DOUBLES_EQUAL(1.0, 1.123, 0.01);
	});
}

TEST(FailureExamplesTestSuite, testBytesEqualFailure)
{
	test().name("Byte comparison failure");

	step("Bytes should match", []() {
		BYTES_EQUAL('A', 'B');
	});
}

TEST(FailureExamplesTestSuite, testMemcmpEqualFailure)
{
	test().name("Memory comparison failure");

	step("Buffers should match", []() {
		const char expected[] = "foo";
		const char actual[] = "f0o";
		MEMCMP_EQUAL(expected, actual, sizeof(expected) - 1);
	});
}

TEST(FailureExamplesTestSuite, testCheckFailure)
{
	test().name("Generic check failure");
	CHECK(false);
}

TEST(FailureExamplesTestSuite, testCheckTextFailure)
{
	test().name("Check with message failure");
	CHECK_TEXT(false, "CheckText message should appear");
}

TEST(FailureExamplesTestSuite, testCheckFalseFailure)
{
	test().name("Check false failure");
	CHECK_FALSE(true);
}

TEST(FailureExamplesTestSuite, testCheckEqualFailure)
{
	test().name("Check equal failure");
	CHECK_EQUAL(10, 20);
}

TEST(FailureExamplesTestSuite, testCheckCompareFailure)
{
	test().name("Check compare failure");
	int a = 5;
	int b = 1;
	CHECK_COMPARE(a, <, b);
}

TEST(FailureExamplesTestSuite, testCheckThrowsFailure)
{
	test().name("Check throws failure");
	CHECK_THROWS(std::runtime_error, []() { /* no throw */ }());
}

TEST(FailureExamplesTestSuite, testStrnCmpEqualFailure)
{
	test().name("STRNCMP failure");
	STRNCMP_EQUAL("Hello", "Help", 4);
}

TEST(FailureExamplesTestSuite, testStrCmpNoCaseFailure)
{
	test().name("STRCMP no-case failure");
	STRCMP_NOCASE_EQUAL("Allure", "aLLuRe?");
}

TEST(FailureExamplesTestSuite, testStrCmpContainsFailure)
{
	test().name("STRCMP contains failure");
	STRCMP_CONTAINS("needle", "haystack");
}

TEST(FailureExamplesTestSuite, testUnsignedLongsEqualFailure)
{
	test().name("Unsigned longs failure");
	UNSIGNED_LONGS_EQUAL(static_cast<unsigned long>(1), static_cast<unsigned long>(2));
}

TEST(FailureExamplesTestSuite, testPointersEqualFailure)
{
	test().name("Pointer comparison failure");
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
	test().name("Function pointer comparison failure");
	FUNCTIONPOINTERS_EQUAL(&expectedFunction, &actualFunction);
}

TEST(FailureExamplesTestSuite, testBitsEqualFailure)
{
	test().name("Bits comparison failure");
	uint8_t expected = 0b11110000;
	uint8_t actual = 0b11000011;
	uint8_t mask = 0xFF;
	BITS_EQUAL(expected, actual, mask);
}

TEST(FailureExamplesTestSuite, testExplicitFail)
{
	test().name("Explicit fail");
	FAIL("Forced failure");
}

//==============================================================================
// New Features Test Suite
//==============================================================================

TEST_GROUP(NewFeaturesTestSuite)
{
    void setup() override
    {
        suite()
			.name("New Features Demo")
			.description("Demonstrating RAII steps and test marking features")
			.epic("Phase 3 Features")
			.severity("high");
    }
};

TEST(NewFeaturesTestSuite, testWithNestedSteps)
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
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testWithDeeplyNestedSteps)
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
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsFlaky)
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
		CHECK_TRUE(true);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsKnown)
{
	test()
		.name("Known issue test")
		.known();

	// This test demonstrates a known issue that has been documented
	step("Known issue: will fail until bug #123 is fixed", []() {
		CHECK_EQUAL(5, 2 + 2);
	});
}

TEST(NewFeaturesTestSuite, testMarkedAsMuted)
{
	test()
		.name("Muted test example")
		.muted();

	// This test is muted - failures are expected and ignored
	step("This failure is muted", []() {
		CHECK_TRUE(false);
	});
}
