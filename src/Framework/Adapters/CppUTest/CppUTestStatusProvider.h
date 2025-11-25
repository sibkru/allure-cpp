#pragma once

#include "Framework/ITestStatusProvider.h"


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	/**
	 * @brief CppUTest-specific implementation of ITestStatusProvider.
	 *
	 * This class provides runtime access to test execution status by accessing
	 * CppUTest's global TestResult.
	 *
	 * CppUTest provides access to the current test result through static methods
	 * that can be queried during test execution.
	 *
	 * Usage:
	 * @code
	 * CppUTestStatusProvider statusProvider;
	 * if (statusProvider.isCurrentTestFailed()) {
	 *     // Test has already failed, skip expensive operations
	 * }
	 * @endcode
	 */
	class CppUTestStatusProvider : public allure_cpp::ITestStatusProvider
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		CppUTestStatusProvider() = default;

		~CppUTestStatusProvider() override = default;

		// ITestStatusProvider implementation
		bool isCurrentTestFailed() const override;
		bool isCurrentTestSkipped() const override;
	};

}}} // namespace allure_cpp::adapters::cpputest
