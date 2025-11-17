#include "stdafx.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestCaseStartEventHandler.h"

#include "GTestAllureUtilities/Model/TestProgram.h"

#include "TestUtilities/Mocks/Services/System/MockTimeService.h"
#include "TestUtilities/Mocks/Services/System/MockUUIDGeneratorService.h"


using namespace testing;
using namespace allure_cpp;
using namespace allure_cpp::test_utility;

namespace systelab { namespace gtest_allure { namespace unit_test {

	class TestCaseStartEventHandlerTest : public testing::Test
	{
		void SetUp()
		{
			setUpTestProgram();
			auto uuidGeneratorService = buildUUIDGeneratorService();
			auto timeService = buildTimeService();

			m_service = std::make_unique<service::TestCaseStartEventHandler>(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
		}

		void setUpTestProgram()
		{
			model::TestSuite finishedTestSuite;
			finishedTestSuite.setStage(model::Stage::FINISHED);
			m_testProgram.addTestSuite(finishedTestSuite);

			model::TestSuite runningTestSuite;
			runningTestSuite.setStage(model::Stage::RUNNING);
			runningTestSuite.setName("TestSuiteName");
			m_testProgram.addTestSuite(runningTestSuite);

			m_runningTestSuite = &m_testProgram.getTestSuite(1);
		}

		std::unique_ptr<service::IUUIDGeneratorService> buildUUIDGeneratorService()
		{
			auto uuidGeneratorService = std::make_unique<MockUUIDGeneratorService>();
			m_uuidGeneratorService = uuidGeneratorService.get();

			m_generatedUUID = "test-uuid-1234";
			ON_CALL(*m_uuidGeneratorService, generateUUID()).WillByDefault(Return(m_generatedUUID));

			return uuidGeneratorService;
		}

		std::unique_ptr<service::ITimeService> buildTimeService()
		{
			auto timeService = std::make_unique<MockTimeService>();
			m_timeService = timeService.get();

			m_currentTime = 987654321;
			ON_CALL(*m_timeService, getCurrentTime()).WillByDefault(Return(m_currentTime));

			return timeService;
		}

	protected:
		std::unique_ptr<service::TestCaseStartEventHandler> m_service;
		model::TestProgram m_testProgram;
		MockUUIDGeneratorService* m_uuidGeneratorService;
		MockTimeService* m_timeService;

		model::TestSuite* m_runningTestSuite;
		std::string m_generatedUUID;
		time_t m_currentTime;
	};


	TEST_F(TestCaseStartEventHandlerTest, testHandleTestCaseStartAddsStartedTestCaseIntoRunningSuite)
	{
		std::string startedTestCaseName = "StartedTestCase";
		m_service->handleTestCaseStart(startedTestCaseName);

		ASSERT_EQ(1, m_runningTestSuite->getTestCases().size());

		model::TestCase& addedTestCase = m_runningTestSuite->getTestCases()[0];
		EXPECT_EQ(m_generatedUUID, addedTestCase.getUUID());
		EXPECT_EQ(startedTestCaseName, addedTestCase.getName());
		EXPECT_EQ("TestSuiteName.StartedTestCase", addedTestCase.getFullName());
		EXPECT_FALSE(addedTestCase.getHistoryId().empty());
		EXPECT_EQ(m_generatedUUID, addedTestCase.getTestCaseId());
		EXPECT_EQ(m_currentTime, addedTestCase.getStart());
		EXPECT_EQ(model::Stage::RUNNING, addedTestCase.getStage());
		EXPECT_EQ(model::Status::UNKNOWN, addedTestCase.getStatus());
	}

	TEST_F(TestCaseStartEventHandlerTest, testHandleTestCaseStartThrowsExceptionWhenNoRunningTestSuite)
	{
		m_testProgram.clearTestSuites();
		ASSERT_THROW(m_service->handleTestCaseStart("StartedTestCase"),
					 service::TestCaseStartEventHandler::NoRunningTestSuiteException);
	}

}}}
