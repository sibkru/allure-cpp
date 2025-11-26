#pragma once

#include <string>


namespace allure {

	/**
	 * @brief Framework-agnostic interface for accessing test metadata.
	 *
	 * This interface provides a uniform way to access test information
	 * regardless of the underlying test framework (GoogleTest, CppUTest, Catch2, etc.).
	 *
	 * Each framework adapter implements this interface to expose test metadata
	 * in a standardized format.
	 */
	class ITestMetadata
	{
	public:
		virtual ~ITestMetadata() = default;

		/**
		 * @brief Get the name of the current test.
		 * @return Test name (e.g., "shouldCalculateCorrectSum")
		 */
		virtual std::string getTestName() const = 0;

		/**
		 * @brief Get the name of the test suite/group.
		 * @return Suite name (e.g., "CalculatorTest" for GoogleTest, "Calculator" for CppUTest)
		 */
		virtual std::string getSuiteName() const = 0;

		/**
		 * @brief Get the full qualified test name.
		 * @return Full name typically in format "SuiteName.TestName"
		 */
		virtual std::string getFullName() const = 0;

		/**
		 * @brief Get the source file where the test is defined.
		 * @return File path (absolute or relative depending on framework)
		 */
		virtual std::string getFileName() const = 0;

		/**
		 * @brief Get the line number where the test is defined.
		 * @return Line number in the source file, or 0 if not available
		 */
		virtual int getLineNumber() const = 0;

		/**
		 * @brief Check if this test is a parametric test.
		 * @return true if test has parameters (e.g., TEST_P, TYPED_TEST in GoogleTest)
		 */
		virtual bool hasTypeParameter() const { return false; }

		/**
		 * @brief Check if this test has a value parameter.
		 * @return true if test has value parameters (e.g., TEST_P in GoogleTest)
		 */
		virtual bool hasValueParameter() const { return false; }

		/**
		 * @brief Get the type parameter for typed tests.
		 * @return Type parameter string (e.g., "int", "double") or empty if not applicable
		 */
		virtual std::string getTypeParameter() const { return ""; }

		/**
		 * @brief Get the value parameter for parametric tests.
		 * @return Value parameter string (e.g., "42", "test_value") or empty if not applicable
		 */
		virtual std::string getValueParameter() const { return ""; }
	};

}
