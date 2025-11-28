// Include allure-cpp.h BEFORE CppUTest headers to avoid conflicts with CppUTest's new/delete overrides
#include "allure-cpp.h"
#include <CppUTest/TestHarness.h>

using namespace allure;

// [snippet:landing-cpputest-example]
TEST_GROUP(LandingPageTests)
{
    void setup() override
    {
        suite()
            .name("My Test Suite")
            .description("This is what your annotated testcases look like.")
            .epic("library demo")
            .severity("critical");
    }
};

TEST(LandingPageTests, TestWithSteps)
{
    test()
        .name("Test with multiple steps")
        .feature("Step-by-Step Execution")
        .story("User can track test execution with detailed steps");

    int result = 0;

    step("Initialize value to 5", [&]() {
        result = 5;
    });

    {
        auto s = step("alternative step syntax");
        result *= 2;
    }

    step("Add 3 to value", [&]() {
        result += 3;
    });

    step("Verify result is 13", [&]() {
        CHECK_EQUAL(13, result);
    });
}
// [/snippet:landing-cpputest-example]

// [snippet:cpputest-suite-metadata]
TEST_GROUP(CalculatorTests)
{
    void setup() override
    {
        suite()
            .name("Calculator Test Suite")
            .description("Tests for basic arithmetic operations")
            .epic("Calculator Module")
            .severity("critical");
    }
};
// [/snippet:cpputest-suite-metadata]

// [snippet:cpputest-test-metadata]
TEST(CalculatorTests, AdditionTest)
{
    test()
        .name("Verify addition of two numbers")
        .feature("Arithmetic Operations")
        .story("User can add two numbers")
        .severity("critical");

    int result = 2 + 3;
    CHECK_EQUAL(5, result);
}
// [/snippet:cpputest-test-metadata]

// [snippet:cpputest-steps]
TEST(CalculatorTests, MultiStepTest)
{
    test()
        .name("Multi-step calculation")
        .feature("Arithmetic Operations");

    int result = 0;

    step("Initialize value to 5", [&]() {
        result = 5;
    });

    step("Add 3 to value", [&]() {
        result += 3;
    });

    step("Verify result is 8", [&]() {
        CHECK_EQUAL(8, result);
    });
}
// [/snippet:cpputest-steps]

// [snippet:cpputest-raii-steps]
TEST(CalculatorTests, RAIIStyleSteps)
{
    test().name("RAII-style steps example");

    int result = 0;

    {
        auto s = step("Initialize value");
        result = 5;
    }

    {
        auto s = step("Multiply by 2");
        result *= 2;
    }

    CHECK_EQUAL(10, result);
}
// [/snippet:cpputest-raii-steps]
