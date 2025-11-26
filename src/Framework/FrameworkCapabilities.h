#pragma once

#include <string>


namespace allure {

	/**
	 * @brief Describes the capabilities and features of a test framework.
	 *
	 * This structure allows the reporting system to adapt its behavior based on
	 * what the underlying test framework supports. Different frameworks have
	 * varying levels of support for features like test suites, runtime status
	 * queries, skipped tests, and parametric tests.
	 *
	 * The capabilities are determined by the framework adapter and queried
	 * via ITestFrameworkAdapter::getCapabilities().
	 */
	struct FrameworkCapabilities
	{
		/**
		 * @brief Does the framework have an explicit concept of test suites?
		 *
		 * - true: Framework has test suites (e.g., GoogleTest TEST_F, CppUTest TEST_GROUP)
		 * - false: Tests are flat or grouped by other means (synthesize suites as needed)
		 */
		bool supportsTestSuites;

		/**
		 * @brief Can the framework query test status during test execution?
		 *
		 * - true: Framework provides APIs to check if current test has failed
		 *         (e.g., GoogleTest's ::testing::Test::HasFailure())
		 * - false: Status is only known after test completes
		 */
		bool supportsRuntimeStatus;

		/**
		 * @brief Does the framework support skipped/disabled tests?
		 *
		 * - true: Framework can skip tests and report them as skipped
		 *         (e.g., GoogleTest DISABLED_ prefix, GTEST_SKIP())
		 * - false: Tests are either run or not compiled in
		 */
		bool supportsSkippedTests;

		/**
		 * @brief Does the framework support parametric/data-driven tests?
		 *
		 * - true: Framework has built-in parametric test support
		 *         (e.g., GoogleTest INSTANTIATE_TEST_SUITE_P, Catch2 TEMPLATE_TEST_CASE)
		 * - false: Each test case is independent
		 */
		bool supportsParametricTests;

		/**
		 * @brief Name of the test framework.
		 *
		 * Examples: "GoogleTest", "CppUTest", "Catch2", "Boost.Test"
		 */
		std::string frameworkName;

		/**
		 * @brief Version of the test framework.
		 *
		 * Format is framework-dependent (e.g., "1.14.0" for GoogleTest 1.14.0)
		 */
		std::string frameworkVersion;


		/**
		 * @brief Default constructor - creates an "unknown" framework.
		 */
		FrameworkCapabilities()
			: supportsTestSuites(false)
			, supportsRuntimeStatus(false)
			, supportsSkippedTests(false)
			, supportsParametricTests(false)
			, frameworkName("Unknown")
			, frameworkVersion("0.0.0")
		{
		}

		/**
		 * @brief Construct with all capabilities specified.
		 */
		FrameworkCapabilities(bool testSuites,
		                      bool runtimeStatus,
		                      bool skippedTests,
		                      bool parametricTests,
		                      std::string name,
		                      std::string version)
			: supportsTestSuites(testSuites)
			, supportsRuntimeStatus(runtimeStatus)
			, supportsSkippedTests(skippedTests)
			, supportsParametricTests(parametricTests)
			, frameworkName(std::move(name))
			, frameworkVersion(std::move(version))
		{
		}
	};

}
