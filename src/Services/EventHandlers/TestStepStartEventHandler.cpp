#include "TestStepStartEventHandler.h"

#include "Model/Action.h"
#include "Model/ExpectedResult.h"
#include "Model/TestProgram.h"
#include "Services/System/ITimeService.h"


namespace allure_cpp { namespace service {

	TestStepStartEventHandler::TestStepStartEventHandler(model::TestProgram& testProgram,
														 std::unique_ptr<ITimeService> timeService)
		:m_testProgram(testProgram)
		,m_timeService(std::move(timeService))
	{
	}

	void TestStepStartEventHandler::handleTestStepStart(const std::string& testStepName, bool isAction) const
	{
		auto step = buildStep(isAction);
		step->setName(testStepName);
		step->setStart(m_timeService->getCurrentTime());
		step->setStage(model::Stage::RUNNING);
		step->setStatus(model::Status::UNKNOWN);

		auto& testCase = getRunningTestCase();

		// Check if there's a running step to nest within
		model::Step* runningStep = testCase.getRunningStep();
		if (runningStep != nullptr)
		{
			// Add as nested step to the currently running step
			runningStep->addStep(std::move(step));
		}
		else
		{
			// No running step, add as top-level step to test case
			testCase.addStep(std::move(step));
		}
	}

	std::unique_ptr<model::Step> TestStepStartEventHandler::buildStep(bool isAction) const
	{
		if (isAction)
		{
			return std::make_unique<model::Action>();
		}
		else
		{
			return std::make_unique<model::ExpectedResult>();
		}
	}

	model::TestCase& TestStepStartEventHandler::getRunningTestCase() const
	{
		model::TestCase* testCase = m_testProgram.getRunningTestCase();
		if (!testCase)
		{
			throw NoRunningTestCaseException();
		}
		return *testCase;
	}

	model::TestSuite& TestStepStartEventHandler::getRunningTestSuite() const
	{
		model::TestSuite* testSuite = m_testProgram.getRunningTestSuite();
		if (!testSuite)
		{
			throw NoRunningTestSuiteException();
		}
		return *testSuite;
	}

}} // namespace allure_cpp::service
