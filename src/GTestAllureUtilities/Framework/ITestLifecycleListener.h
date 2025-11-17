#pragma once

#include "GTestAllureUtilities/Framework/ITestMetadata.h"
#include "GTestAllureUtilities/Model/Status.h"


namespace allure_cpp {

	/**
	 * @brief Framework-agnostic interface for receiving test lifecycle events.
	 *
	 * This interface defines callbacks for all major test execution events.
	 * Framework adapters call these methods at appropriate points during test execution,
	 * allowing the Allure reporting system to track test progress without coupling
	 * to any specific test framework.
	 *
	 * Event Flow:
	 * 1. onTestProgramStart()
	 * 2. For each test suite:
	 *    a. onTestSuiteStart()
	 *    b. For each test:
	 *       i.   onTestStart()
	 *       ii.  [Test executes]
	 *       iii. onTestEnd()
	 *    c. onTestSuiteEnd()
	 * 3. onTestProgramEnd()
	 *
	 * @note Frameworks without explicit test suites may synthesize suite events
	 *       or call suite start/end around each test.
	 */
	class ITestLifecycleListener
	{
	public:
		virtual ~ITestLifecycleListener() = default;

		/**
		 * @brief Called when the test program starts.
		 *
		 * This is invoked once at the beginning of test execution,
		 * before any test suites or tests run.
		 *
		 * Use this event to:
		 * - Initialize the test program/session
		 * - Set up global reporting structures
		 * - Record program start time
		 */
		virtual void onTestProgramStart() = 0;

		/**
		 * @brief Called when a test suite starts.
		 *
		 * A test suite is a group of related tests (e.g., TEST_F in GoogleTest,
		 * TEST_GROUP in CppUTest, or file-based grouping in Catch2).
		 *
		 * @param metadata Test metadata for the suite (suite name, file info, etc.)
		 *
		 * @note For the first test in a suite, this provides information about
		 *       that test. The suite name comes from metadata.getSuiteName().
		 */
		virtual void onTestSuiteStart(const ITestMetadata& metadata) = 0;

		/**
		 * @brief Called when an individual test starts.
		 *
		 * This is invoked immediately before a test case begins execution.
		 *
		 * @param metadata Complete test metadata (test name, suite name, location)
		 */
		virtual void onTestStart(const ITestMetadata& metadata) = 0;

		/**
		 * @brief Called when an individual test ends.
		 *
		 * This is invoked after a test case completes, regardless of outcome.
		 *
		 * @param metadata Complete test metadata
		 * @param status Final test status (PASSED, FAILED, BROKEN, SKIPPED)
		 *
		 * @note The status parameter uses the existing allure_cpp::model::Status
		 *       enum which is framework-agnostic.
		 */
		virtual void onTestEnd(const ITestMetadata& metadata,
		                       allure_cpp::model::Status status) = 0;

		/**
		 * @brief Called when a test suite ends.
		 *
		 * This is invoked after all tests in a suite have completed.
		 *
		 * @param status Overall suite status (typically worst status of contained tests)
		 */
		virtual void onTestSuiteEnd(allure_cpp::model::Status status) = 0;

		/**
		 * @brief Called when the test program ends.
		 *
		 * This is invoked once at the end of test execution,
		 * after all test suites and tests have completed.
		 *
		 * Use this event to:
		 * - Finalize and write test reports
		 * - Clean up global resources
		 * - Record program end time
		 */
		virtual void onTestProgramEnd() = 0;

		/**
		 * @brief Optional: Check if current test is a retry attempt.
		 *
		 * Frameworks with retry support can override this to indicate
		 * when a test is being re-run. This affects how Allure groups
		 * test results (same historyId for retries of the same test).
		 *
		 * When a test is retried:
		 * - Multiple result files are generated with different UUIDs
		 * - All retries share the same historyId
		 * - Allure displays them grouped together in the Retries view
		 *
		 * @return true if this is a retry attempt (not the first run)
		 * @note Default implementation returns false (no retry support)
		 *
		 * @example
		 * // Framework with retry support (e.g., pytest with pytest-rerunfailures)
		 * bool isRetryAttempt() const override {
		 *     return m_currentRetryCount > 0;
		 * }
		 */
		virtual bool isRetryAttempt() const
		{
			return false;
		}

		/**
		 * @brief Optional: Get the retry attempt number.
		 *
		 * Provides the ordinal number of the current retry attempt.
		 * Used for informational purposes in test reports.
		 *
		 * @return 0 for first attempt (initial run),
		 *         1 for first retry,
		 *         2 for second retry, etc.
		 * @note Default implementation returns 0 (no retries)
		 *
		 * @example
		 * // Track retry count in framework adapter
		 * int getRetryAttemptNumber() const override {
		 *     return m_currentRetryCount;
		 * }
		 */
		virtual int getRetryAttemptNumber() const
		{
			return 0;
		}
	};

}
