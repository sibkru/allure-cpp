#include <CppUTest/CommandLineTestRunner.h>
#include "Framework/Adapters/CppUTest/AllureCppUTest.h"
#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

int main(int ac, const char * const *av)
{
    // Minimal path: initializes Allure (sets output folder/framework) and runs the
    // stock CppUTest CLI with Allure reporting enabled.
    allure_cpp::adapters::cpputest::AllureCppUTest allureHelper;
    return allure_cpp::adapters::cpputest::RunAllureEnabledTests(ac, av);

    // --- Bring-your-own-main variant ---
    // If you need a custom output (TeamCity/JUnit/Console/custom), create it and pass it in:
    //
    //   allure_cpp::adapters::cpputest::AllureCppUTest allureHelper;
    //   auto* preferredOutput = new TeamCityTestOutput(); // or ConsoleTestOutput/JUnit/custom
    //   return allure_cpp::adapters::cpputest::RunAllureEnabledTests(ac, av, preferredOutput);
    //
    // For full control (your own runner subclass), call WrapAllureOutput() inside your
    // overridden create*Output methods so Allure can capture failure details:
    //
    //   TestOutput* MyRunner::createConsoleOutput() override {
    //       return allure_cpp::adapters::cpputest::WrapAllureOutput(new ConsoleTestOutput());
    //   }
}
