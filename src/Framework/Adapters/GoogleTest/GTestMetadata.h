#pragma once

#include "Framework/ITestMetadata.h"
#include <gtest/gtest.h>
#include <string>


namespace allure {
namespace adapters {
namespace googletest {

	/**
	 * @brief GoogleTest-specific implementation of ITestMetadata.
	 *
	 * This class wraps GoogleTest's ::testing::TestInfo to provide test metadata
	 * in a framework-agnostic format.
	 *
	 * GoogleTest provides test information through the TestInfo class, which contains:
	 * - Test name: The name of the test case (e.g., "shouldCalculateCorrectSum")
	 * - Test suite name: The name of the test fixture/suite (e.g., "CalculatorTest")
	 * - File location: Source file and line number where the test is defined
	 *
	 * This adapter translates that information into the ITestMetadata interface,
	 * enabling framework-independent access to test metadata.
	 */
	class GTestMetadata : public allure::ITestMetadata
	{
	public:
		/**
		 * @brief Construct metadata wrapper around GoogleTest TestInfo.
		 *
		 * @param testInfo Reference to GoogleTest's TestInfo object
		 *                 Must remain valid for the lifetime of this object
		 */
		explicit GTestMetadata(const ::testing::TestInfo& testInfo);

		~GTestMetadata() override = default;

		// ITestMetadata implementation
		std::string getTestName() const override;
		std::string getSuiteName() const override;
		std::string getFullName() const override;
		std::string getFileName() const override;
		int getLineNumber() const override;
		bool hasTypeParameter() const override;
		bool hasValueParameter() const override;
		std::string getTypeParameter() const override;
		std::string getValueParameter() const override;

	private:
		const ::testing::TestInfo& m_testInfo;
	};

}}} // namespace allure::adapters::googletest
