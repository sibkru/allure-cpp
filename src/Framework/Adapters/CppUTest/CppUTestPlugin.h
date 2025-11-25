#pragma once

#include "Framework/TestLifecycleListenerBase.h"
#include "Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"
#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "Services/EventHandlers/ITestCaseEndEventHandler.h"
#include <CppUTest/TestHarness.h>
#include <CppUTest/TestPlugin.h>


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	/**
	 * @brief CppUTest plugin that bridges CppUTest callbacks to the abstraction layer.
	 *
	 * This class serves as the integration point between CppUTest's plugin system
	 * and the framework-agnostic abstraction layer. It inherits from both:
	 *
	 * 1. TestPlugin - CppUTest's plugin interface
	 *    Receives callbacks from CppUTest when test lifecycle events occur
	 *
	 * 2. TestLifecycleListenerBase - Framework-agnostic bridge class
	 *    Delegates to existing event handlers
	 *
	 * The plugin translates CppUTest-specific events and data structures
	 * (UtestShell, TestResult, etc.) into framework-agnostic
	 * representations (ITestMetadata, Status) and forwards them to the parent
	 * TestLifecycleListenerBase, which then delegates to the event handlers.
	 *
	 * Flow:
	 * CppUTest event → CppUTestPlugin → TestLifecycleListenerBase → Event handlers
	 *
	 * Usage:
	 * @code
	 * // Create event handlers
	 * auto programStartHandler = servicesFactory->buildTestProgramStartEventHandler();
	 * auto suiteStartHandler = servicesFactory->buildTestSuiteStartEventHandler();
	 * // ... create other handlers ...
	 *
	 * // Create plugin that bridges to handlers
	 * auto* plugin = new CppUTestPlugin(
	 *     programStartHandler.get(),
	 *     programEndHandler.get(),
	 *     suiteStartHandler.get(),
	 *     suiteEndHandler.get(),
	 *     caseStartHandler.get(),
	 *     caseEndHandler.get()
	 * );
	 *
	 * // Install with CppUTest (CppUTest takes ownership)
	 * TestRegistry::getCurrentRegistry()->installPlugin(plugin);
	 * @endcode
	 */
	class CppUTestPlugin : public TestPlugin,
	                       public allure_cpp::TestLifecycleListenerBase
	{
	public:
		/**
		 * @brief Construct the plugin with event handlers.
		 *
		 * All event handlers are passed to TestLifecycleListenerBase for delegation.
		 *
		 * @param programStartHandler Handler for program start events (must not be null)
		 * @param programEndHandler Handler for program end events (must not be null)
		 * @param suiteStartHandler Handler for suite start events (must not be null)
		 * @param suiteEndHandler Handler for suite end events (must not be null)
		 * @param caseStartHandler Handler for test case start events (must not be null)
		 * @param caseEndHandler Handler for test case end events (must not be null)
		 */
		CppUTestPlugin(
			allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
			allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
			allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
			allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
			allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
			allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler);

		~CppUTestPlugin() override = default;

		// TestPlugin lifecycle callbacks
		void preTestAction(UtestShell& test, TestResult& result) override;
		void postTestAction(UtestShell& test, TestResult& result) override;

	private:
		std::string m_currentGroup;
		bool m_groupStarted;
		int m_initialFailureCount;
	};

}}} // namespace allure_cpp::adapters::cpputest
