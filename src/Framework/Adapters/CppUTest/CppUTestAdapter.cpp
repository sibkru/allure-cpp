#include "Framework/Adapters/CppUTest/CppUTestAdapter.h"
#include "Framework/Adapters/CppUTest/CppUTestPlugin.h"
#include "Framework/Adapters/CppUTest/CppUTestStatusProvider.h"
#include <CppUTest/TestRegistry.h>


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	CppUTestAdapter::CppUTestAdapter(
		std::unique_ptr<allure_cpp::service::ITestProgramStartEventHandler> programStartHandler,
		std::unique_ptr<allure_cpp::service::ITestProgramEndEventHandler> programEndHandler,
		std::unique_ptr<allure_cpp::service::ITestSuiteStartEventHandler> suiteStartHandler,
		std::unique_ptr<allure_cpp::service::ITestSuiteEndEventHandler> suiteEndHandler,
		std::unique_ptr<allure_cpp::service::ITestCaseStartEventHandler> caseStartHandler,
		std::unique_ptr<allure_cpp::service::ITestCaseEndEventHandler> caseEndHandler)
		: m_programStartHandler(std::move(programStartHandler))
		, m_programEndHandler(std::move(programEndHandler))
		, m_suiteStartHandler(std::move(suiteStartHandler))
		, m_suiteEndHandler(std::move(suiteEndHandler))
		, m_caseStartHandler(std::move(caseStartHandler))
		, m_caseEndHandler(std::move(caseEndHandler))
	{
	}

	void CppUTestAdapter::initialize()
	{
		// Create the CppUTest plugin
		auto* plugin = new CppUTestPlugin(
			m_programStartHandler.get(),
			m_programEndHandler.get(),
			m_suiteStartHandler.get(),
			m_suiteEndHandler.get(),
			m_caseStartHandler.get(),
			m_caseEndHandler.get()
		);

		// Install the plugin
		// Note: CppUTest's TestRegistry takes ownership of the plugin
		TestRegistry::getCurrentRegistry()->installPlugin(plugin);
	}

	std::unique_ptr<ITestStatusProvider> CppUTestAdapter::createStatusProvider() const
	{
		// Create a new CppUTest status provider
		return std::make_unique<CppUTestStatusProvider>();
	}

	void CppUTestAdapter::finalize()
	{
		// Trigger program end event to write JSON files
		if (m_programEndHandler)
		{
			m_programEndHandler->handleTestProgramEnd();
		}
	}

	FrameworkCapabilities CppUTestAdapter::getCapabilities() const
	{
		// CppUTest capabilities
		return FrameworkCapabilities(
			true,   // supportsTestSuites - CppUTest has TEST_GROUP
			true,   // supportsRuntimeStatus - Can query via TestResult
			true,   // supportsSkippedTests - Supports IGNORE_TEST
			false,  // supportsParametricTests - Not natively supported
			"CppUTest",
			"4.0"   // CppUTest version
		);
	}

}}} // namespace allure_cpp::adapters::cpputest
