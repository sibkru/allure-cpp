// Include your headers first so they are not compiled under CppUTest's new/delete
// overrides.
#include <allure-cpp.h>

// Bring in CppUTest yourself; keep these last so their overrides don't leak.
#include <CppUTest/CommandLineTestRunner.h>

// Allure's adapter depends on CppUTest but does not pull it in for you, so this
// stays last as well.
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int ac, const char * const *av)
{
    // Minimal path: initializes Allure (sets output folder/framework) and runs the
    // stock CppUTest CLI with Allure reporting enabled.
    allure::adapters::cpputest::AllureCppUTest allureHelper;
    return allure::adapters::cpputest::RunAllureEnabledTests(ac, av);

    // --- Bring-your-own-main variant ---
    // If you need a custom output (TeamCity/JUnit/Console/custom), create it and pass it in:
    //
    //   allure::adapters::cpputest::AllureCppUTest allureHelper;
    //   auto* preferredOutput = new TeamCityTestOutput(); // or ConsoleTestOutput/JUnit/custom
    //   return allure::adapters::cpputest::RunAllureEnabledTests(ac, av, preferredOutput);
    //
    // For full control (your own runner subclass), call WrapAllureOutput() inside your
    // overridden create*Output methods so Allure can capture failure details:
    //
    //   TestOutput* MyRunner::createConsoleOutput() override {
    //       return allure::adapters::cpputest::WrapAllureOutput(new ConsoleTestOutput());
    //   }
}
