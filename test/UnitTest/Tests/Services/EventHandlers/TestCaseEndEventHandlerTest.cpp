#include "stdafx.h"
#include "Services/EventHandlers/TestCaseEndEventHandler.h"

#include "Model/TestProgram.h"

#include "TestUtilities/Mocks/Services/System/MockTimeService.h"
#include "TestUtilities/Mocks/Services/System/MockUUIDGeneratorService.h"
#include "TestUtilities/Mocks/Services/Report/MockTestCaseJSONSerializer.h"
#include "TestUtilities/Mocks/Services/System/MockFileService.h"


using namespace testing;
using namespace allure_cpp;
using namespace allure_cpp::test_utility;

namespace systelab { namespace gtest_allure { namespace unit_test {

	class TestCaseEndEventHandlerTest : public testing::Test
	{
	public:
		void SetUp()
		{
			setUpTestProgram();
			auto timeService = buildTimeService();
			auto testCaseJSONSerializer = buildTestCaseJSONSerializer();
			auto fileService = buildFileService();

			m_service = std::make_unique<service::TestCaseEndEventHandler>(m_testProgram, std::move(timeService), std::move(testCaseJSONSerializer), std::move(fileService));
		}

		void setUpTestProgram()
		{
			model::TestSuite finishedTestSuite;
			finishedTestSuite.setStage(model::Stage::FINISHED);
			finishedTestSuite.addTestCase(buildTestCase("TC-1.1", model::Stage::FINISHED));
			finishedTestSuite.addTestCase(buildTestCase("TC-1.2", model::Stage::FINISHED));
			m_testProgram.addTestSuite(finishedTestSuite);

			model::TestSuite runningTestSuite;
			runningTestSuite.setStage(model::Stage::RUNNING);
			runningTestSuite.addTestCase(buildTestCase("TC-2.1", model::Stage::FINISHED));
			runningTestSuite.addTestCase(buildTestCase("TC-2.2", model::Stage::RUNNING));
			m_testProgram.addTestSuite(runningTestSuite);

			m_runningTestCase = &m_testProgram.getTestSuite(1).getTestCases()[1];
		}

		model::TestCase buildTestCase(const std::string& name, model::Stage stage)
		{
			model::TestCase testCase;
			testCase.setName(name);
			testCase.setStage(stage);
			testCase.setStatus(model::Status::UNKNOWN);

			return testCase;
		}

		std::unique_ptr<service::ITimeService> buildTimeService()
		{
			auto timeService = std::make_unique<MockTimeService>();
			m_timeService = timeService.get();

			m_currentTime = 123456789;
			ON_CALL(*m_timeService, getCurrentTime()).WillByDefault(Return(m_currentTime));

			return timeService;
		}

		std::unique_ptr<service::ITestCaseJSONSerializer> buildTestCaseJSONSerializer()
		{
			auto serializer = std::make_unique<MockTestCaseJSONSerializer>();
			m_testCaseJSONSerializer = serializer.get();

			ON_CALL(*m_testCaseJSONSerializer, serialize(_)).WillByDefault(Return("{}"));

			return serializer;
		}

		std::unique_ptr<service::IFileService> buildFileService()
		{
			auto fileService = std::make_unique<MockFileService>();
			m_fileService = fileService.get();

			return fileService;
		}

	protected:
		std::unique_ptr<service::TestCaseEndEventHandler> m_service;
		model::TestProgram m_testProgram;
		MockTimeService* m_timeService;
		MockTestCaseJSONSerializer* m_testCaseJSONSerializer;
		MockFileService* m_fileService;

		model::TestCase* m_runningTestCase;
		time_t m_currentTime;
	};


	TEST_F(TestCaseEndEventHandlerTest, testHandleTestCaseEndSetsStopTimeOfRunningTestCaseToCurrentTime)
	{
		m_service->handleTestCaseEnd(model::Status::PASSED);
		ASSERT_EQ(m_currentTime, m_runningTestCase->getStop());
	}

	TEST_F(TestCaseEndEventHandlerTest, testHandleTestCaseEndSetsStageOfRunningTestCaseToFinished)
	{
		m_service->handleTestCaseEnd(model::Status::PASSED);
		ASSERT_EQ(model::Stage::FINISHED, m_runningTestCase->getStage());
	}

	TEST_F(TestCaseEndEventHandlerTest, testHandleTestCaseEndThrowsExceptionWhenNoRunningTestCase)
	{
		m_runningTestCase->setStage(model::Stage::FINISHED);
		ASSERT_THROW(m_service->handleTestCaseEnd(model::Status::PASSED),
					 service::ITestCaseEndEventHandler::NoRunningTestCaseException);
	}

	TEST_F(TestCaseEndEventHandlerTest, testHandleTestCaseEndThrowsExceptionWhenNoRunningTestSuite)
	{
		m_testProgram.getTestSuite(1).setStage(model::Stage::FINISHED);
		ASSERT_THROW(m_service->handleTestCaseEnd(model::Status::PASSED),
					 service::ITestCaseEndEventHandler::NoRunningTestSuiteException);
	}

	TEST_F(TestCaseEndEventHandlerTest, testHandleTestCaseEndWritesTestCaseJSONImmediately)
	{
		// Expect JSON serializer to be called with the running test case
		EXPECT_CALL(*m_testCaseJSONSerializer, serialize(_)).Times(1);

		// Expect file service to save the JSON
		EXPECT_CALL(*m_fileService, saveFile(_, _)).Times(1);

		m_service->handleTestCaseEnd(model::Status::PASSED);
	}


	class TestCaseEndEventHandlerStatusTest : public TestCaseEndEventHandlerTest
											, public testing::WithParamInterface<model::Status>
	{
	public:
		void SetUp()
		{
			TestCaseEndEventHandlerTest::SetUp();
		}
	};

	TEST_P(TestCaseEndEventHandlerStatusTest, testHandleTestCaseEndSetsStatusOfRunningTestCaseToGivenValue)
	{
		m_service->handleTestCaseEnd(GetParam());
		ASSERT_EQ(GetParam(), m_runningTestCase->getStatus());
	}

	namespace {
		std::vector<model::Status> testStatusData = { model::Status::BROKEN, model::Status::FAILED, model::Status::PASSED,
													  model::Status::SKIPPED, model::Status::UNKNOWN };
	}

	INSTANTIATE_TEST_SUITE_P(Test, TestCaseEndEventHandlerStatusTest, ValuesIn(testStatusData));

}}}
