// [snippet:googletest-environment]
#include <gtest/gtest.h>
#include "allure-cpp.h"

namespace {
    // Initialize Allure before any tests run
    class AllureEnvironment : public ::testing::Environment {
    public:
        void SetUp() override {
            // Allure adapter is initialized as a static member
            // to ensure it lives for the entire test run
            static allure::adapters::googletest::AllureGTest allureHelper;
        }
    };

    // Register the environment
    ::testing::Environment* const allure_env =
        ::testing::AddGlobalTestEnvironment(new AllureEnvironment);
}
// [/snippet:googletest-environment]
