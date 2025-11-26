#pragma once

#include "ITestCaseEndEventHandler.h"

#include <memory>


namespace allure { namespace model {
	class Action;
	class TestCase;
	class TestProgram;
	class TestSuite;
}} // namespace allure::service

namespace allure { namespace service {

	class ITimeService;
	class IUUIDGeneratorService;
	class ITestCaseJSONSerializer;
	class IFileService;

	class TestCaseEndEventHandler : public ITestCaseEndEventHandler
	{
	public:
		TestCaseEndEventHandler(model::TestProgram&,
		                        std::unique_ptr<ITimeService>,
		                        std::unique_ptr<ITestCaseJSONSerializer>,
		                        std::unique_ptr<IFileService>);
		virtual ~TestCaseEndEventHandler() = default;

		void handleTestCaseEnd(model::Status) const override;
		void handleTestCaseEnd(model::Status status,
		                       const std::string& statusMessage,
		                       const std::string& statusTrace) const override;

	private:
		model::TestCase& getRunningTestCase() const;
		model::TestSuite& getRunningTestSuite() const;
		void writeTestCaseJSON(const model::TestCase& testCase) const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITimeService> m_timeService;
		std::unique_ptr<ITestCaseJSONSerializer> m_testCaseJSONSerializer;
		std::unique_ptr<IFileService> m_fileService;
	};

}} // namespace allure::service
