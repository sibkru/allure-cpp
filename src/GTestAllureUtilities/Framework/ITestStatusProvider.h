#pragma once


namespace allure_cpp {

	/**
	 * @brief Framework-agnostic interface for querying test execution status.
	 *
	 * This interface allows code to query the current state of a running test
	 * without depending on framework-specific APIs.
	 *
	 * Different frameworks have different capabilities for runtime status queries:
	 * - GoogleTest: Can query via ::testing::Test::HasFailure()
	 * - CppUTest: Can query via UtestShell::hasFailed()
	 * - Some frameworks may not support runtime queries (return cached values)
	 */
	class ITestStatusProvider
	{
	public:
		virtual ~ITestStatusProvider() = default;

		/**
		 * @brief Check if the current test has failed.
		 *
		 * A test is considered failed if:
		 * - Any assertion/expectation failed
		 * - An unexpected exception was thrown
		 * - The test was explicitly marked as failed
		 *
		 * @return true if test has failed, false otherwise
		 *
		 * @note For frameworks that don't support runtime queries, this may
		 *       return the status from the last known state or always return false
		 *       until the test ends.
		 */
		virtual bool isCurrentTestFailed() const = 0;

		/**
		 * @brief Check if the current test has been skipped.
		 *
		 * A test is skipped when:
		 * - Test was disabled (e.g., DISABLED_ prefix in GoogleTest)
		 * - Test was conditionally skipped (e.g., GTEST_SKIP())
		 * - Test dependencies failed (framework-dependent)
		 *
		 * @return true if test is skipped, false otherwise
		 *
		 * @note Not all frameworks support skipped tests. In such cases,
		 *       this method returns false. Check FrameworkCapabilities::supportsSkippedTests
		 *       to determine if the framework supports this feature.
		 */
		virtual bool isCurrentTestSkipped() const = 0;
	};

}
