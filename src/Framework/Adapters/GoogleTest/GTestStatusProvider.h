#pragma once

#include "Framework/ITestStatusProvider.h"
#include <gtest/gtest.h>


namespace allure_cpp {
namespace adapters {
namespace googletest {

	/**
	 * @brief GoogleTest-specific implementation of ITestStatusProvider.
	 *
	 * This class provides runtime access to test execution status by wrapping
	 * GoogleTest's static status query methods.
	 *
	 * GoogleTest provides the following static methods for runtime status queries:
	 * - ::testing::Test::HasFailure() - Returns true if any assertion has failed
	 * - ::testing::Test::IsSkipped() - Returns true if test was skipped (GoogleTest 1.10+)
	 *
	 * These methods can be called from anywhere during test execution to check
	 * the current test's status.
	 *
	 * Usage:
	 * @code
	 * GTestStatusProvider statusProvider;
	 * if (statusProvider.isCurrentTestFailed()) {
	 *     // Test has already failed, skip expensive operations
	 * }
	 * @endcode
	 */
	class GTestStatusProvider : public allure_cpp::ITestStatusProvider
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * No state is needed - GoogleTest provides static methods for status queries.
		 */
		GTestStatusProvider() = default;

		~GTestStatusProvider() override = default;

		// ITestStatusProvider implementation
		bool isCurrentTestFailed() const override;
		bool isCurrentTestSkipped() const override;
	};

}}} // namespace allure_cpp::adapters::googletest
