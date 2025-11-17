#pragma once

#include "Framework/ITestLifecycleListener.h"
#include "Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"
#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "Services/EventHandlers/ITestCaseEndEventHandler.h"

#include <memory>


namespace allure_cpp {

	/**
	 * @brief Base implementation of ITestLifecycleListener that bridges to existing event handlers.
	 *
	 * This class provides a default implementation of ITestLifecycleListener that delegates
	 * to the existing systelab event handler interfaces. This allows the new framework-agnostic
	 * abstraction to work with the existing event handling architecture without requiring
	 * immediate refactoring of all dependent code.
	 *
	 * The class acts as an adapter between:
	 * - NEW: Framework-agnostic ITestLifecycleListener interface
	 * - EXISTING: systelab event handler interfaces (ITestProgramStartEventHandler, etc.)
	 *
	 * Design:
	 * - Event handlers are injected via constructor (dependency injection)
	 * - Implements ITestLifecycleListener by delegating to the handlers
	 * - Extracts necessary information from ITestMetadata to pass to handlers
	 * - Preserves existing behavior while enabling framework independence
	 *
	 * Usage:
	 * @code
	 * // Create event handlers (existing code)
	 * auto programStartHandler = std::make_unique<TestProgramStartEventHandler>(...);
	 * auto suiteStartHandler = std::make_unique<TestSuiteStartEventHandler>(...);
	 * // ... other handlers ...
	 *
	 * // Create the base listener that bridges to handlers
	 * auto listener = std::make_unique<TestLifecycleListenerBase>(
	 *     programStartHandler.get(),
	 *     suiteStartHandler.get(),
	 *     // ... pass all handlers ...
	 * );
	 *
	 * // Use with framework adapter
	 * auto adapter = std::make_unique<GTestAdapter>(listener.get());
	 * adapter->initialize();
	 * @endcode
	 */
	class TestLifecycleListenerBase : public ITestLifecycleListener
	{
	public:
		/**
		 * @brief Construct the listener with event handlers.
		 *
		 * All event handlers are injected as dependencies. The listener does not
		 * take ownership - the caller must ensure handlers remain valid for the
		 * lifetime of this listener.
		 *
		 * @param programStartHandler Handler for program start events (must not be null)
		 * @param programEndHandler Handler for program end events (must not be null)
		 * @param suiteStartHandler Handler for suite start events (must not be null)
		 * @param suiteEndHandler Handler for suite end events (must not be null)
		 * @param caseStartHandler Handler for test case start events (must not be null)
		 * @param caseEndHandler Handler for test case end events (must not be null)
		 */
		TestLifecycleListenerBase(
			allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
			allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
			allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
			allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
			allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
			allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler);

		virtual ~TestLifecycleListenerBase() = default;

		// ITestLifecycleListener implementation
		void onTestProgramStart() override;
		void onTestSuiteStart(const ITestMetadata& metadata) override;
		void onTestStart(const ITestMetadata& metadata) override;
		void onTestEnd(const ITestMetadata& metadata, allure_cpp::model::Status status) override;
		void onTestSuiteEnd(allure_cpp::model::Status status) override;
		void onTestProgramEnd() override;

		// Overload for capturing error details
		void onTestEnd(const ITestMetadata& metadata, allure_cpp::model::Status status,
		               const std::string& statusMessage, const std::string& statusTrace);

	private:
		allure_cpp::service::ITestProgramStartEventHandler* m_programStartHandler;
		allure_cpp::service::ITestProgramEndEventHandler* m_programEndHandler;
		allure_cpp::service::ITestSuiteStartEventHandler* m_suiteStartHandler;
		allure_cpp::service::ITestSuiteEndEventHandler* m_suiteEndHandler;
		allure_cpp::service::ITestCaseStartEventHandler* m_caseStartHandler;
		allure_cpp::service::ITestCaseEndEventHandler* m_caseEndHandler;
	};

}
