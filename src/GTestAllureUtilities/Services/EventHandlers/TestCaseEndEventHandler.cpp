#include "TestCaseEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/System/ITimeService.h"


namespace allure_cpp { namespace service {

	TestCaseEndEventHandler::TestCaseEndEventHandler(model::TestProgram& testProgram,
													 std::unique_ptr<ITimeService> timeService)
		:m_testProgram(testProgram)
		,m_timeService(std::move(timeService))
	{
	}

	void TestCaseEndEventHandler::handleTestCaseEnd(model::Status status) const
	{
		model::TestCase& testCase = getRunningTestCase();
		testCase.setStop(m_timeService->getCurrentTime());
		testCase.setStage(model::Stage::FINISHED);
		testCase.setStatus(status);
	}

	void TestCaseEndEventHandler::handleTestCaseEnd(model::Status status,
	                                                 const std::string& statusMessage,
	                                                 const std::string& statusTrace) const
	{
		model::TestCase& testCase = getRunningTestCase();
		testCase.setStop(m_timeService->getCurrentTime());
		testCase.setStage(model::Stage::FINISHED);
		testCase.setStatus(status);

		// Set failure details if provided
		if (!statusMessage.empty())
		{
			testCase.setStatusMessage(statusMessage);
		}
		if (!statusTrace.empty())
		{
			testCase.setStatusTrace(statusTrace);
		}
	}

	model::TestCase& TestCaseEndEventHandler::getRunningTestCase() const
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

	model::TestSuite& TestCaseEndEventHandler::getRunningTestSuite() const
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
