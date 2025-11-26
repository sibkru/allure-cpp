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
namespace googletest {

	/**
	 * @brief Main adapter class for GoogleTest framework integration.
	 *
	 * This is the primary integration point between the framework-agnostic
	 * Allure reporting system and GoogleTest. It implements the ITestFrameworkAdapter
	 * interface to provide:
	 *
	 * 1. Framework initialization - Registers GTestEventListener with GoogleTest
	 * 2. Status provider creation - Creates GTestStatusProvider for runtime queries
	 * 3. Capability reporting - Describes GoogleTest's features
	 *
	 * The adapter stores references to event handlers and uses them to create
	 * the GTestEventListener during initialization.
	 *
	 * GoogleTest Capabilities:
	 * - Test Suites: Yes (TEST_F, test fixtures)
	 * - Runtime Status: Yes (::testing::Test::HasFailure(), IsSkipped())
	 * - Skipped Tests: Yes (DISABLED_ prefix, GTEST_SKIP())
	 * - Parametric Tests: Yes (INSTANTIATE_TEST_SUITE_P)
	 *
	 * Usage:
	 * @code
	 * // Create event handlers
	 * auto servicesFactory = std::make_unique<ServicesFactory>();
	 * auto programStartHandler = servicesFactory->buildTestProgramStartEventHandler();
	 * // ... create other handlers ...
	 *
	 * // Create and initialize adapter
	 * auto adapter = std::make_unique<GTestAdapter>(
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
	 * assert(caps.frameworkName == "GoogleTest");
	 *
	 * // Create status provider
	 * auto statusProvider = adapter->createStatusProvider();
	 * if (statusProvider->isCurrentTestFailed()) {
	 *     // Handle failure
	 * }
	 * @endcode
	 */
	class GTestAdapter : public allure::ITestFrameworkAdapter
	{
	public:
		/**
		 * @brief Construct the GoogleTest adapter with event handlers.
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
		GTestAdapter(
			std::unique_ptr<allure::service::ITestProgramStartEventHandler> programStartHandler,
			std::unique_ptr<allure::service::ITestProgramEndEventHandler> programEndHandler,
			std::unique_ptr<allure::service::ITestSuiteStartEventHandler> suiteStartHandler,
			std::unique_ptr<allure::service::ITestSuiteEndEventHandler> suiteEndHandler,
			std::unique_ptr<allure::service::ITestCaseStartEventHandler> caseStartHandler,
			std::unique_ptr<allure::service::ITestCaseEndEventHandler> caseEndHandler);

		~GTestAdapter() override = default;

		// ITestFrameworkAdapter implementation
		void initialize() override;
		std::unique_ptr<ITestStatusProvider> createStatusProvider() const override;
		FrameworkCapabilities getCapabilities() const override;

	private:
		// Event handler pointers (owned by this adapter)
		std::unique_ptr<allure::service::ITestProgramStartEventHandler> m_programStartHandler;
		std::unique_ptr<allure::service::ITestProgramEndEventHandler> m_programEndHandler;
		std::unique_ptr<allure::service::ITestSuiteStartEventHandler> m_suiteStartHandler;
		std::unique_ptr<allure::service::ITestSuiteEndEventHandler> m_suiteEndHandler;
		std::unique_ptr<allure::service::ITestCaseStartEventHandler> m_caseStartHandler;
		std::unique_ptr<allure::service::ITestCaseEndEventHandler> m_caseEndHandler;
	};

}}} // namespace allure::adapters::googletest
