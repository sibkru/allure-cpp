#pragma once

#include "Framework/ITestMetadata.h"
#include <CppUTest/Utest.h>
#include <string>


namespace allure {
namespace adapters {
namespace cpputest {

	/**
	 * @brief CppUTest-specific implementation of ITestMetadata.
	 *
	 * This class wraps CppUTest's UtestShell to provide test metadata
	 * in a framework-agnostic format.
	 *
	 * CppUTest provides test information through the UtestShell class, which contains:
	 * - Test name: The name of the test (e.g., "shouldCalculateCorrectSum")
	 * - Test group: The name of the test group (e.g., "Calculator")
	 * - File location: Source file and line number where the test is defined
	 *
	 * This adapter translates that information into the ITestMetadata interface,
	 * enabling framework-independent access to test metadata.
	 */
	class CppUTestMetadata : public allure::ITestMetadata
	{
	public:
		/**
		 * @brief Construct metadata wrapper around CppUTest UtestShell.
		 *
		 * @param test Reference to CppUTest's UtestShell object
		 *             Must remain valid for the lifetime of this object
		 */
		explicit CppUTestMetadata(const UtestShell& test);

		~CppUTestMetadata() override = default;

		// ITestMetadata implementation
		std::string getTestName() const override;
		std::string getSuiteName() const override;
		std::string getFullName() const override;
		std::string getFileName() const override;
		int getLineNumber() const override;

	private:
		const UtestShell& m_test;
	};

}}} // namespace allure::adapters::cpputest
