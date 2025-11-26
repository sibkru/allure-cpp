#include <memory>

#include "API/Core.h"
#include "Framework/Adapters/CppUTest/AllureCppUTest.h"
#include "Framework/Adapters/CppUTest/AllureCppUTestOutput.h"
#include "Framework/Adapters/CppUTest/CppUTestAdapter.h"
#include "Framework/Adapters/CppUTest/CppUTestPlugin.h"
#include "Services/ServicesFactory.h"

#include <CppUTest/JUnitTestOutput.h>
#include <CppUTest/TeamCityTestOutput.h>
#include <CppUTest/TestRegistry.h>

#include "Framework/Adapters/CppUTest/AllureCppUTestCommandLineTestRunner.h"

namespace allure { namespace adapters { namespace cpputest {

TestOutput* AllureCppUTestCommandLineTestRunner::userTestOutput = nullptr;
static std::shared_ptr<allure::ITestFrameworkAdapter> s_adapter = nullptr;

AllureCppUTestCommandLineTestRunner::AllureCppUTestCommandLineTestRunner(int ac, const char *const *av, TestRegistry* registry)
	: CommandLineTestRunner(ac, av, registry)
{
}

AllureCppUTestCommandLineTestRunner::~AllureCppUTestCommandLineTestRunner()
{
	if (s_adapter)
	{
		auto* cppuTestAdapter = dynamic_cast<CppUTestAdapter*>(s_adapter.get());
		if (cppuTestAdapter)
		{
			cppuTestAdapter->finalize();
		}
	}
}

int AllureCppUTestCommandLineTestRunner::RunAllTests(int ac, const char *const *av)
{
	AllureCppUTest allureHelper;

	auto& testProgram = allure::detail::Core::instance().getTestProgram();
	auto servicesFactory = std::make_unique<allure::service::ServicesFactory>(testProgram);

	// Create handlers, get raw pointers, then move ownership to the adapter
	auto programStartHandler = servicesFactory->buildTestProgramStartEventHandler();
	auto* programStartHandlerPtr = programStartHandler.get();
	auto programEndHandler = servicesFactory->buildTestProgramEndEventHandler();
	auto* programEndHandlerPtr = programEndHandler.get();
	auto suiteStartHandler = servicesFactory->buildTestSuiteStartEventHandler();
	auto* suiteStartHandlerPtr = suiteStartHandler.get();
	auto suiteEndHandler = servicesFactory->buildTestSuiteEndEventHandler();
	auto* suiteEndHandlerPtr = suiteEndHandler.get();
	auto caseStartHandler = servicesFactory->buildTestCaseStartEventHandler();
	auto* caseStartHandlerPtr = caseStartHandler.get();
	auto caseEndHandler = servicesFactory->buildTestCaseEndEventHandler();
	auto* caseEndHandlerPtr = caseEndHandler.get();

	s_adapter = std::make_shared<CppUTestAdapter>(
		std::move(programStartHandler),
		std::move(programEndHandler),
		std::move(suiteStartHandler),
		std::move(suiteEndHandler),
		std::move(caseStartHandler),
		std::move(caseEndHandler)
	);
	allure::detail::Core::instance().setFrameworkAdapter(s_adapter);

	auto* plugin = new CppUTestPlugin(
		programStartHandlerPtr,
		programEndHandlerPtr,
		suiteStartHandlerPtr,
		suiteEndHandlerPtr,
		caseStartHandlerPtr,
		caseEndHandlerPtr
	);

	TestRegistry::getCurrentRegistry()->installPlugin(plugin);

	AllureCppUTestCommandLineTestRunner runner(ac, av, TestRegistry::getCurrentRegistry());
	return runner.runAllTestsMain();
}

void AllureCppUTestCommandLineTestRunner::setUserTestOutput(TestOutput* output)
{
	userTestOutput = output;
}

TestOutput* AllureCppUTestCommandLineTestRunner::createConsoleOutput()
{
	TestOutput* consoleOutput = userTestOutput ? userTestOutput : CommandLineTestRunner::createConsoleOutput();
	return wrapWithAllureOutput(consoleOutput);
}

TestOutput* AllureCppUTestCommandLineTestRunner::createJUnitOutput(const SimpleString& packageName)
{
	TestOutput* junitOutput = userTestOutput ? userTestOutput : CommandLineTestRunner::createJUnitOutput(packageName);
	return wrapWithAllureOutput(junitOutput);
}

TestOutput* AllureCppUTestCommandLineTestRunner::createTeamCityOutput()
{
	TestOutput* teamCityOutput = userTestOutput ? userTestOutput : CommandLineTestRunner::createTeamCityOutput();
	return wrapWithAllureOutput(teamCityOutput);
}

TestOutput* AllureCppUTestCommandLineTestRunner::wrapWithAllureOutput(TestOutput* innerOutput)
{
	return createCompositeOutput(innerOutput, new AllureCppUTestOutput());
}

TestOutput* WrapAllureOutput(TestOutput* innerOutput)
{
	auto* composite = new CompositeTestOutput();
	composite->setOutputOne(innerOutput);
	composite->setOutputTwo(new AllureCppUTestOutput());
	return composite;
}

int RunAllureEnabledTests(int ac, const char *const *av, TestOutput* preferredOutput)
{
	if (preferredOutput != nullptr)
	{
		AllureCppUTestCommandLineTestRunner::setUserTestOutput(WrapAllureOutput(preferredOutput));
	}
	else
	{
		AllureCppUTestCommandLineTestRunner::setUserTestOutput(nullptr);
	}

	return AllureCppUTestCommandLineTestRunner::RunAllTests(ac, av);
}

}}} // namespace allure::adapters::cpputest
