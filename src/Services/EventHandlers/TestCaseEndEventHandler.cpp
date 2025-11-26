#include "TestCaseEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/System/ITimeService.h"
#include "Services/Report/ITestCaseJSONSerializer.h"
#include "Services/System/IFileService.h"

#ifdef _WIN32
	#define PATH_SEPARATOR "\\"
#else
	#define PATH_SEPARATOR "/"
#endif


namespace allure { namespace service {

	TestCaseEndEventHandler::TestCaseEndEventHandler(model::TestProgram& testProgram,
													 std::unique_ptr<ITimeService> timeService,
													 std::unique_ptr<ITestCaseJSONSerializer> testCaseJSONSerializer,
													 std::unique_ptr<IFileService> fileService)
		:m_testProgram(testProgram)
		,m_timeService(std::move(timeService))
		,m_testCaseJSONSerializer(std::move(testCaseJSONSerializer))
		,m_fileService(std::move(fileService))
	{
	}

	void TestCaseEndEventHandler::handleTestCaseEnd(model::Status status) const
	{
		model::TestCase& testCase = getRunningTestCase();
		model::TestSuite& testSuite = getRunningTestSuite();

		testCase.setStop(m_timeService->getCurrentTime());
		testCase.setStage(model::Stage::FINISHED);
		testCase.setStatus(status);

		// Write JSON immediately after test completes
		writeTestCaseJSON(testCase);

		// Clear the test case cache since it's no longer running
		m_testProgram.setRunningTestCase(nullptr);
	}

	void TestCaseEndEventHandler::handleTestCaseEnd(model::Status status,
	                                                 const std::string& statusMessage,
	                                                 const std::string& statusTrace) const
	{
		model::TestCase& testCase = getRunningTestCase();
		model::TestSuite& testSuite = getRunningTestSuite();

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

		// Write JSON immediately after test completes
		writeTestCaseJSON(testCase);

		// Clear the test case cache since it's no longer running
		m_testProgram.setRunningTestCase(nullptr);
	}

	void TestCaseEndEventHandler::writeTestCaseJSON(const model::TestCase& testCase) const
	{
		// Generate test case result file: {uuid}-result.json
		std::string filepath = m_testProgram.getOutputFolder() + PATH_SEPARATOR + testCase.getUUID() + "-result.json";
		std::string content = m_testCaseJSONSerializer->serialize(testCase);
		m_fileService->saveFile(filepath, content);
	}

	model::TestCase& TestCaseEndEventHandler::getRunningTestCase() const
	{
		model::TestCase* testCase = m_testProgram.getRunningTestCase();
		if (!testCase)
		{
			throw NoRunningTestCaseException();
		}
		return *testCase;
	}

	model::TestSuite& TestCaseEndEventHandler::getRunningTestSuite() const
	{
		model::TestSuite* testSuite = m_testProgram.getRunningTestSuite();
		if (!testSuite)
		{
			throw NoRunningTestSuiteException();
		}
		return *testSuite;
	}

}} // namespace allure::service
