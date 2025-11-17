#pragma once

#include "ITestCaseEndEventHandler.h"

#include <memory>


namespace allure_cpp { namespace model {
	class Action;
	class TestCase;
	class TestProgram;
	class TestSuite;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITimeService;
	class IUUIDGeneratorService;

	class TestCaseEndEventHandler : public ITestCaseEndEventHandler
	{
	public:
		TestCaseEndEventHandler(model::TestProgram&, std::unique_ptr<ITimeService>);
		virtual ~TestCaseEndEventHandler() = default;

		void handleTestCaseEnd(model::Status) const override;
		void handleTestCaseEnd(model::Status status,
		                       const std::string& statusMessage,
		                       const std::string& statusTrace) const override;

	private:
		model::TestCase& getRunningTestCase() const;
		model::TestSuite& getRunningTestSuite() const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITimeService> m_timeService;
	};

}} // namespace allure_cpp::service
