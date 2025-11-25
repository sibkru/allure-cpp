#include "stdafx.h"
#include "Services/EventHandlers/TestSuiteEndEventHandler.h"

#include "Model/TestProgram.h"

#include "TestUtilities/Mocks/Services/System/MockTimeService.h"
#include "TestUtilities/Mocks/Services/System/MockUUIDGeneratorService.h"
#include "TestUtilities/Mocks/Services/Report/MockContainerJSONSerializer.h"
#include "TestUtilities/Mocks/Services/System/MockFileService.h"


using namespace testing;
using namespace allure_cpp;
using namespace allure_cpp::test_utility;

namespace systelab { namespace gtest_allure { namespace unit_test {

	class TestSuiteEndEventHandlerTest : public testing::Test
	{
	public:
		void SetUp()
		{
			setUpTestProgram();
			auto timeService = buildTimeService();
			auto containerJSONSerializer = buildContainerJSONSerializer();
			auto fileService = buildFileService();

			m_service = std::make_unique<service::TestSuiteEndEventHandler>(m_testProgram, std::move(timeService), std::move(containerJSONSerializer), std::move(fileService));
		}

		void setUpTestProgram()
		{
			model::TestSuite finishedTestSuite;
			finishedTestSuite.setStage(model::Stage::FINISHED);
			m_testProgram.addTestSuite(finishedTestSuite);

			model::TestSuite runningTestSuite;
			runningTestSuite.setStage(model::Stage::RUNNING);
			m_testProgram.addTestSuite(runningTestSuite);

			m_runningTestSuite = &m_testProgram.getTestSuite(1);
		}

		std::unique_ptr<service::ITimeService> buildTimeService()
		{
			auto timeService = std::make_unique<MockTimeService>();
			m_timeService = timeService.get();

			m_currentTime = 123456789;
			ON_CALL(*m_timeService, getCurrentTime()).WillByDefault(Return(m_currentTime));

			return timeService;
		}

		std::unique_ptr<service::IContainerJSONSerializer> buildContainerJSONSerializer()
		{
			auto serializer = std::make_unique<MockContainerJSONSerializer>();
			m_containerJSONSerializer = serializer.get();

			ON_CALL(*m_containerJSONSerializer, serialize(_)).WillByDefault(Return("{}"));

			return serializer;
		}

		std::unique_ptr<service::IFileService> buildFileService()
		{
			auto fileService = std::make_unique<MockFileService>();
			m_fileService = fileService.get();

			return fileService;
		}

	protected:
		std::unique_ptr<service::TestSuiteEndEventHandler> m_service;
		model::TestProgram m_testProgram;
		MockTimeService* m_timeService;
		MockContainerJSONSerializer* m_containerJSONSerializer;
		MockFileService* m_fileService;

		model::TestSuite* m_runningTestSuite;
		time_t m_currentTime;
	};


	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndSetsStopTimeOfRunningTestSuiteToCurrentTime)
	{
		m_service->handleTestSuiteEnd(model::Status::PASSED);
		ASSERT_EQ(m_currentTime, m_runningTestSuite->getStop());
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndSetsStageOfRunningTestSuiteToFinished)
	{
		m_service->handleTestSuiteEnd(model::Status::PASSED);
		ASSERT_EQ(model::Stage::FINISHED, m_runningTestSuite->getStage());
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndAddsTMSLinkWithSuiteTmsIdAndDefaultPattern)
	{
		m_runningTestSuite->setTmsId("MyID-123");

		m_service->handleTestSuiteEnd(model::Status::PASSED);

		auto testSuiteLinks = m_runningTestSuite->getLinks();
		ASSERT_EQ(1, testSuiteLinks.size());
		ASSERT_EQ("tms", testSuiteLinks[0].getType());
		ASSERT_EQ("MyID-123", testSuiteLinks[0].getName());
		ASSERT_EQ("http://MyID-123", testSuiteLinks[0].getURL());
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndAddsTMSLinkWithSuiteTmsIdAndCustomPattern)
	{
		m_testProgram.setTMSLinksPattern("https://www.mycompany.com/{}/page");
		m_runningTestSuite->setTmsId("9876");

		m_service->handleTestSuiteEnd(model::Status::PASSED);

		auto testSuiteLinks = m_runningTestSuite->getLinks();
		ASSERT_EQ(1, testSuiteLinks.size());
		ASSERT_EQ("tms", testSuiteLinks[0].getType());
		ASSERT_EQ("9876", testSuiteLinks[0].getName());
		ASSERT_EQ("https://www.mycompany.com/9876/page", testSuiteLinks[0].getURL());
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndThrowsExceptionWhenNoRunningTestSuite)
	{
		m_runningTestSuite->setStage(model::Stage::FINISHED);
		ASSERT_THROW(m_service->handleTestSuiteEnd(model::Status::PASSED), service::ITestSuiteEndEventHandler::NoRunningTestSuiteException);
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndWritesContainerJSONImmediately)
	{
		// Expect JSON serializer to be called
		EXPECT_CALL(*m_containerJSONSerializer, serialize(_)).Times(1);

		// Expect file service to save the JSON
		EXPECT_CALL(*m_fileService, saveFile(_, _)).Times(1);

		m_service->handleTestSuiteEnd(model::Status::PASSED);
	}

	TEST_F(TestSuiteEndEventHandlerTest, testHandleTestSuiteEndUsesTestCasesFromSuiteInContainer)
	{
		// Add test cases to the running suite
		model::TestCase tc1;
		tc1.setUUID("uuid-1");
		m_runningTestSuite->addTestCase(tc1);

		model::TestCase tc2;
		tc2.setUUID("uuid-2");
		m_runningTestSuite->addTestCase(tc2);

		// Capture the container that gets serialized
		EXPECT_CALL(*m_containerJSONSerializer, serialize(_))
			.WillOnce(Invoke([](const model::Container& container) {
				// Verify the container has the correct UUIDs as children
				const auto& children = container.getChildren();
				EXPECT_EQ(2u, children.size());
				EXPECT_EQ("uuid-1", children[0]);
				EXPECT_EQ("uuid-2", children[1]);
				return "{}";
			}));

		m_service->handleTestSuiteEnd(model::Status::PASSED);
	}


	class TestSuiteEndEventHandlerStatusTest : public TestSuiteEndEventHandlerTest
											 , public testing::WithParamInterface<model::Status>
	{
	public:
		void SetUp()
		{
			TestSuiteEndEventHandlerTest::SetUp();
		}
	};

	TEST_P(TestSuiteEndEventHandlerStatusTest, testHandleTestSuiteEndSetsStatusOfRunningTestSuiteToGivenValue)
	{
		m_service->handleTestSuiteEnd(GetParam());
		ASSERT_EQ(GetParam(), m_runningTestSuite->getStatus());
	}

	namespace {
		std::vector<model::Status> testStatusData = { model::Status::BROKEN, model::Status::FAILED, model::Status::PASSED,
													  model::Status::SKIPPED, model::Status::UNKNOWN };
	}

	INSTANTIATE_TEST_SUITE_P(Test, TestSuiteEndEventHandlerStatusTest, ValuesIn(testStatusData));

}}}
