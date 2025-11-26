#include "TestStepEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/System/ITimeService.h"


namespace allure { namespace service {

	TestStepEndEventHandler::TestStepEndEventHandler(model::TestProgram& testProgram,
													 std::unique_ptr<ITimeService> timeService)
		:m_testProgram(testProgram)
		,m_timeService(std::move(timeService))
	{
	}

	void TestStepEndEventHandler::handleTestStepEnd(model::Status status) const
	{
		model::Step& step = getRunningTestStep();
		step.setStop(m_timeService->getCurrentTime());
		step.setStage(model::Stage::FINISHED);
		step.setStatus(status);
	}

	model::Step& TestStepEndEventHandler::getRunningTestStep() const
	{
		auto& testCase = getRunningTestCase();

		// Use the helper method that recursively finds the deepest running step
		model::Step* runningStep = testCase.getRunningStep();
		if (runningStep != nullptr)
		{
			return *runningStep;
		}

		throw NoRunningTestStepException();
	}

	model::TestCase& TestStepEndEventHandler::getRunningTestCase() const
	{
		model::TestCase* testCase = m_testProgram.getRunningTestCase();
		if (!testCase)
		{
			throw NoRunningTestCaseException();
		}
		return *testCase;
	}

	model::TestSuite& TestStepEndEventHandler::getRunningTestSuite() const
	{
		model::TestSuite* testSuite = m_testProgram.getRunningTestSuite();
		if (!testSuite)
		{
			throw NoRunningTestSuiteException();
		}
		return *testSuite;
	}

}} // namespace allure::service
