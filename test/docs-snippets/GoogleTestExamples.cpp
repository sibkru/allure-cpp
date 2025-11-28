#include <gtest/gtest.h>
#include <allure-cpp.h>

using namespace allure;

// [snippet:googletest-suite-metadata]
class CalculatorTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        suite()
            .name("Calculator Test Suite")
            .description("Tests for basic arithmetic operations")
            .epic("Calculator Module")
            .severity("critical");
    }
};
// [/snippet:googletest-suite-metadata]

// [snippet:googletest-test-metadata]
TEST_F(CalculatorTests, AdditionTest) {
    test()
        .name("Verify addition of two numbers")
        .feature("Arithmetic Operations")
        .story("User can add two numbers")
        .severity("critical");

    int result = 2 + 3;
    EXPECT_EQ(5, result);
}
// [/snippet:googletest-test-metadata]

// [snippet:googletest-steps]
TEST_F(CalculatorTests, MultiStepTest) {
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
        EXPECT_EQ(8, result);
    });
}
// [/snippet:googletest-steps]

// [snippet:googletest-raii-steps]
TEST_F(CalculatorTests, RAIIStyleSteps) {
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

    EXPECT_EQ(10, result);
}
// [/snippet:googletest-raii-steps]
