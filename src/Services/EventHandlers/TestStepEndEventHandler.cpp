#include "TestStepEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/System/ITimeService.h"


namespace allure_cpp { namespace service {

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
		auto& testSuite = getRunningTestSuite();
		for (model::TestCase& testCase : testSuite.getTestCases())
		{
			if (testCase.getStage() == model::Stage::RUNNING)
			{
				return testCase;
			}
		}

		throw NoRunningTestCaseException();
	}

	model::TestSuite& TestStepEndEventHandler::getRunningTestSuite() const
	{
		unsigned int nTestSuites = (unsigned int) m_testProgram.getTestSuitesCount();
		for (unsigned int i = 0; i < nTestSuites; i++)
		{
			model::TestSuite& testSuite = m_testProgram.getTestSuite(i);
			if (testSuite.getStage() == model::Stage::RUNNING)
			{
				return testSuite;
			}
		}

		throw NoRunningTestSuiteException();
	}

}} // namespace allure_cpp::service
