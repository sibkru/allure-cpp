// [snippet:cpputest-main]
// Include your headers first so they aren't compiled under CppUTest's new/delete overrides
#include <allure-cpp.h>

// CppUTest includes come last
#include <CppUTest/CommandLineTestRunner.h>
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    // Initialize Allure and run tests with the Allure-enabled runner
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    return allure::adapters::cpputest::RunAllureEnabledTests(argc, argv);
}
// [/snippet:cpputest-main]
