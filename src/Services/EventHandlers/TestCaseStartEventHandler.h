#pragma once

#include "ITestCaseStartEventHandler.h"

#include <memory>
#include <stdexcept>


namespace allure_cpp { namespace model {
	class TestProgram;
	class TestSuite;
	class TestCase;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITimeService;
	class IUUIDGeneratorService;

	class TestCaseStartEventHandler : public ITestCaseStartEventHandler
	{
	public:
		TestCaseStartEventHandler(model::TestProgram&,
								  std::unique_ptr<IUUIDGeneratorService>,
								  std::unique_ptr<ITimeService>);
		virtual ~TestCaseStartEventHandler() = default;

		void handleTestCaseStart(const std::string& testCaseName) const override;
		void handleTestCaseStart(const ITestMetadata& metadata) const override;

	public:
		struct NoRunningTestSuiteException : std::runtime_error
		{
			NoRunningTestSuiteException()
				:std::runtime_error("No running test suite found when handling event for test case start")
			{}
		};

	private:
		model::TestSuite& getRunningTestSuite() const;
		void addCommonLabels(model::TestCase& testCase, const std::string& suiteName) const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<IUUIDGeneratorService> m_uuidGeneratorService;
		std::unique_ptr<ITimeService> m_timeService;
	};

}} // namespace allure_cpp::service
