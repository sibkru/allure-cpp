// [snippet:cpputest-main-custom-output]
#include "allure-cpp.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TeamCityTestOutput.h>
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int argc, const char* const* argv)
{
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    auto* output = new TeamCityTestOutput();
    return allure::adapters::cpputest::RunAllureEnabledTests(argc, argv, output);
}
// [/snippet:cpputest-main-custom-output]
