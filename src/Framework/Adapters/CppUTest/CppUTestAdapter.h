#pragma once

#include "Framework/ITestFrameworkAdapter.h"
#include "Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"
#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "Services/EventHandlers/ITestCaseEndEventHandler.h"
#include <memory>


namespace allure {
namespace adapters {
namespace cpputest {

	/**
	 * @brief Main adapter class for CppUTest framework integration.
	 *
	 * This is the primary integration point between the framework-agnostic
	 * Allure reporting system and CppUTest. It implements the ITestFrameworkAdapter
	 * interface to provide:
	 *
	 * 1. Framework initialization - Installs CppUTestPlugin with CppUTest
	 * 2. Status provider creation - Creates CppUTestStatusProvider for runtime queries
	 * 3. Capability reporting - Describes CppUTest's features
	 *
	 * The adapter stores references to event handlers and uses them to create
	 * the CppUTestPlugin during initialization.
	 *
	 * CppUTest Capabilities:
	 * - Test Suites: Yes (TEST_GROUP)
	 * - Runtime Status: Limited (CppUTest has global TestResult access)
	 * - Skipped Tests: Yes (IGNORE_TEST)
	 * - Parametric Tests: No (not natively supported in CppUTest)
	 *
	 * Usage:
	 * @code
	 * // Create event handlers
	 * auto servicesFactory = std::make_unique<ServicesFactory>();
	 * auto programStartHandler = servicesFactory->buildTestProgramStartEventHandler();
	 * // ... create other handlers ...
	 *
	 * // Create and initialize adapter
	 * auto adapter = std::make_unique<CppUTestAdapter>(
	 *     programStartHandler.get(),
	 *     programEndHandler.get(),
	 *     suiteStartHandler.get(),
	 *     suiteEndHandler.get(),
	 *     caseStartHandler.get(),
	 *     caseEndHandler.get()
	 * );
	 * adapter->initialize();
	 *
	 * // Query capabilities
	 * auto caps = adapter->getCapabilities();
	 * assert(caps.frameworkName == "CppUTest");
	 *
	 * // Create status provider
	 * auto statusProvider = adapter->createStatusProvider();
	 * if (statusProvider->isCurrentTestFailed()) {
	 *     // Handle failure
	 * }
	 * @endcode
	 */
	class CppUTestAdapter : public allure::ITestFrameworkAdapter
	{
	public:
		/**
		 * @brief Construct the CppUTest adapter with event handlers.
		 *
		 * The adapter takes ownership of the event handlers and manages their lifetime.
		 *
		 * @param programStartHandler Handler for program start events (must not be null)
		 * @param programEndHandler Handler for program end events (must not be null)
		 * @param suiteStartHandler Handler for suite start events (must not be null)
		 * @param suiteEndHandler Handler for suite end events (must not be null)
		 * @param caseStartHandler Handler for test case start events (must not be null)
		 * @param caseEndHandler Handler for test case end events (must not be null)
		 */
		CppUTestAdapter(
			std::unique_ptr<allure::service::ITestProgramStartEventHandler> programStartHandler,
			std::unique_ptr<allure::service::ITestProgramEndEventHandler> programEndHandler,
			std::unique_ptr<allure::service::ITestSuiteStartEventHandler> suiteStartHandler,
			std::unique_ptr<allure::service::ITestSuiteEndEventHandler> suiteEndHandler,
			std::unique_ptr<allure::service::ITestCaseStartEventHandler> caseStartHandler,
			std::unique_ptr<allure::service::ITestCaseEndEventHandler> caseEndHandler);

		~CppUTestAdapter() override = default;

		// ITestFrameworkAdapter implementation
		void initialize() override;
		std::unique_ptr<ITestStatusProvider> createStatusProvider() const override;
		FrameworkCapabilities getCapabilities() const override;

		// CppUTest-specific method to finalize reporting
		// Call this after all tests complete to write JSON files
		void finalize();

	private:
		// Event handler pointers (owned by this adapter)
		std::unique_ptr<allure::service::ITestProgramStartEventHandler> m_programStartHandler;
		std::unique_ptr<allure::service::ITestProgramEndEventHandler> m_programEndHandler;
		std::unique_ptr<allure::service::ITestSuiteStartEventHandler> m_suiteStartHandler;
		std::unique_ptr<allure::service::ITestSuiteEndEventHandler> m_suiteEndHandler;
		std::unique_ptr<allure::service::ITestCaseStartEventHandler> m_caseStartHandler;
		std::unique_ptr<allure::service::ITestCaseEndEventHandler> m_caseEndHandler;
	};

}}} // namespace allure::adapters::cpputest
