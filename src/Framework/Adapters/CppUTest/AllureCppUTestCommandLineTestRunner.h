#pragma once

#include <CppUTest/CommandLineTestRunner.h>

namespace allure { namespace adapters { namespace cpputest {

class AllureCppUTestCommandLineTestRunner : public CommandLineTestRunner
{
public:
	explicit AllureCppUTestCommandLineTestRunner(int ac, const char *const *av, TestRegistry* registry = nullptr);
	~AllureCppUTestCommandLineTestRunner() override;

	static int RunAllTests(int ac, const char *const *av);
	static void setUserTestOutput(TestOutput* output);

protected:
	TestOutput* createConsoleOutput() override;
	TestOutput* createJUnitOutput(const SimpleString& packageName) override;
	TestOutput* createTeamCityOutput() override;

private:
	TestOutput* wrapWithAllureOutput(TestOutput* innerOutput);

	static TestOutput* userTestOutput;
};

// Minimal helper APIs for bring-your-own-main flows.
// 1) Wrap a preferred output (Console/TeamCity/JUnit/custom) with Allure capture.
TestOutput* WrapAllureOutput(TestOutput* innerOutput);
// 2) Run all tests with Allure enabled while preserving the stock CppUTest CLI.
int RunAllureEnabledTests(int ac, const char *const *av, TestOutput* preferredOutput = nullptr);

}}} // namespace allure::adapters::cpputest

namespace allure {
using AllureCppUTestCommandLineTestRunner = allure::adapters::cpputest::AllureCppUTestCommandLineTestRunner;
}
