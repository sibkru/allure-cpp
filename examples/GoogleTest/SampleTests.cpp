#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include <allure-cpp.h>
#include <gtest/gtest.h>
#include "../shared/Calculator.h"

using namespace allure;

//==============================================================================
// Basic Test Suite
//==============================================================================

class BasicTestSuite : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        suite()
            .name("Basic Test Suite")
            .description("A simple test suite to verify API compatibility")
            .epic("Phase 3 Validation")
            .severity("critical")
            .label("layer", "unit")
            .label("tmsId", "API-COMPAT-001");
    }

    void SetUp() override
    {
        test().label("layer", "unit");
        test().epic("Phase 3 Validation");
    }
};

TEST_F(BasicTestSuite, testSimplePass)
{
    test()
        .name("Simple passing test")
        .feature("Basic Operations")
        .story("User can execute simple GoogleTest cases");
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

//==============================================================================
// Complex Test Suite
//==============================================================================

class ComplexTestSuite : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        suite()
            .name("Complex Test Suite")
            .description("Complex scenarios with nested steps")
            .epic("Phase 3 Validation")
            .severity("high")
            .label("layer", "integration")
            .label("feature", "api-compatibility")
            .label("component", "allure-reporting");
    }

    void SetUp() override
    {
        test().label("layer", "integration");
        test().epic("Phase 3 Validation");
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
        EXPECT_EQ(0u, numbers.size());
    });

    step("Add three numbers", [&]() {
        numbers.push_back(1);
        numbers.push_back(2);
        numbers.push_back(3);
    });

    step("Vector should have 3 elements", [&]() {
        EXPECT_EQ(3u, numbers.size());
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
        EXPECT_TRUE(text.find("Allure") != std::string::npos);
    });

    step("Text length should be 14", [&]() {
        EXPECT_EQ(14u, text.length());
    });
}

//==============================================================================
// New Features Test Suite
//==============================================================================

class NewFeaturesTestSuite : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        suite()
            .name("New Features Demo")
            .description("Demonstrating RAII steps and test marking features")
            .epic("Phase 3 Features")
            .severity("high")
            .label("layer", "e2e");
    }

    void SetUp() override
    {
        test().label("layer", "e2e");
        test().epic("Phase 3 Features");
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

TEST_F(NewFeaturesTestSuite, testMarkedAsFlaky)
{
    test()
        .name("Flaky test example")
        .flaky();

    step("Execute unreliable operation", []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    });

    step("Operation should succeed (but is unreliable)", []() {
        EXPECT_TRUE(true);
    });
}
