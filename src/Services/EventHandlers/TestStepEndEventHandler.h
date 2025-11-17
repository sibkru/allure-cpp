#pragma once

#include "ITestStepEndEventHandler.h"

#include <memory>


namespace allure_cpp { namespace model {
	class Action;
	class Step;
	class TestCase;
	class TestProgram;
	class TestSuite;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITimeService;
	class IUUIDGeneratorService;

	class TestStepEndEventHandler : public ITestStepEndEventHandler
	{
	public:
		TestStepEndEventHandler(model::TestProgram&, std::unique_ptr<ITimeService>);
		virtual ~TestStepEndEventHandler() = default;

		void handleTestStepEnd(model::Status) const;

	private:
		model::Step& getRunningTestStep() const;
		model::TestCase& getRunningTestCase() const;
		model::TestSuite& getRunningTestSuite() const;

	private:
		model::TestProgram& m_testProgram;
		std::unique_ptr<ITimeService> m_timeService;
	};

}} // namespace allure_cpp::service
