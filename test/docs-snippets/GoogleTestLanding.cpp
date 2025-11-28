// [snippet:landing-googletest-example]
#include <gtest/gtest.h>
#include <allure-cpp.h>

using namespace allure;

class LandingPageTests : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        suite()
            .name("My Test Suite")
            .description("This is what your annotated testcases look like.")
            .epic("library demo")
            .severity("critical");
    }
};

TEST_F(LandingPageTests, testWithSteps) {
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
        EXPECT_EQ(13, result);
    });
}
// [/snippet:landing-googletest-example]
