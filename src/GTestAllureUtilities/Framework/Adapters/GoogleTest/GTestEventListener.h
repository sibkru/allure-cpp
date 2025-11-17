#pragma once

#include "GTestAllureUtilities/Framework/TestLifecycleListenerBase.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestSuiteStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/ITestCaseEndEventHandler.h"
#include <gtest/gtest.h>


namespace allure_cpp {
namespace adapters {
namespace googletest {

	/**
	 * @brief GoogleTest event listener that bridges GoogleTest callbacks to the abstraction layer.
	 *
	 * This class serves as the integration point between GoogleTest's event listener system
	 * and the framework-agnostic abstraction layer. It inherits from both:
	 *
	 * 1. ::testing::EmptyTestEventListener - GoogleTest's listener interface
	 *    Receives callbacks from GoogleTest when test lifecycle events occur
	 *
	 * 2. TestLifecycleListenerBase - Framework-agnostic bridge class
	 *    Delegates to existing systelab event handlers
	 *
	 * The listener translates GoogleTest-specific events and data structures
	 * (::testing::TestInfo, ::testing::TestSuite, etc.) into framework-agnostic
	 * representations (ITestMetadata, Status) and forwards them to the parent
	 * TestLifecycleListenerBase, which then delegates to the event handlers.
	 *
	 * Flow:
	 * GoogleTest event → GTestEventListener → TestLifecycleListenerBase → Event handlers
	 *
	 * Usage:
	 * @code
	 * // Create event handlers (existing systelab code)
	 * auto programStartHandler = servicesFactory->buildTestProgramStartEventHandler();
	 * auto suiteStartHandler = servicesFactory->buildTestSuiteStartEventHandler();
	 * // ... create other handlers ...
	 *
	 * // Create listener that bridges to handlers
	 * auto listener = new GTestEventListener(
	 *     programStartHandler.get(),
	 *     programEndHandler.get(),
	 *     suiteStartHandler.get(),
	 *     suiteEndHandler.get(),
	 *     caseStartHandler.get(),
	 *     caseEndHandler.get()
	 * );
	 *
	 * // Register with GoogleTest (GoogleTest takes ownership)
	 * ::testing::UnitTest::GetInstance()->listeners().Append(listener);
	 * @endcode
	 */
	class GTestEventListener : public ::testing::EmptyTestEventListener,
	                           public allure_cpp::TestLifecycleListenerBase
	{
	public:
		/**
		 * @brief Construct the listener with event handlers.
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
		GTestEventListener(
			allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
			allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
			allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
			allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
			allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
			allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler);

		~GTestEventListener() override = default;

		// GoogleTest lifecycle callbacks (::testing::EmptyTestEventListener interface)
		void OnTestProgramStart(const ::testing::UnitTest& unitTest) override;
		void OnTestSuiteStart(const ::testing::TestSuite& testSuite) override;
		void OnTestStart(const ::testing::TestInfo& testInfo) override;
		void OnTestEnd(const ::testing::TestInfo& testInfo) override;
		void OnTestSuiteEnd(const ::testing::TestSuite& testSuite) override;
		void OnTestProgramEnd(const ::testing::UnitTest& unitTest) override;
	};

}}} // namespace allure_cpp::adapters::googletest
