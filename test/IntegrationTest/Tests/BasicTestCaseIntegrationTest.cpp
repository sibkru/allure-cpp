#include "stdafx.h"
#include "BaseIntegrationTest.h"

#include "TestUtilities/JSONComparison.h"


using namespace testing;
using namespace systelab::json::test_utility;
using namespace allure;

namespace systelab { namespace gtest_allure { namespace unit_test {

	class BasicTestCaseIntegrationTest : public testing::Test
									   , public BaseIntegrationTest
	{
	public:
		void SetUp()
		{
			BaseIntegrationTest::SetUp();
		}

		void TearDown()
		{
			BaseIntegrationTest::TearDown();
		}
	};


TEST_F(BasicTestCaseIntegrationTest, testProgramWithSingleTestSuite)
{
	auto& testProgram = detail::Core::instance().getTestProgram();
	testProgram.setOutputFolder("IntegrationTest\\OutputFolder");
	testProgram.setName("TestProgramName");
	testProgram.setTMSLinksPattern("https://mytms.webpage.com/{}/refresh");

		auto& listener = getEventListener();
		listener.onProgramStart();

		setCurrentTime(111);
		setNextUUIDToGenerate("suite-uuid-1");
		listener.onTestSuiteStart("SingleTestSuite");

	setCurrentTime(222);
	setNextUUIDToGenerate("test-uuid-1");
	listener.onTestStart("SingleTestCase");
	test().name("SingleTestCase");

		setCurrentTime(333);
		listener.onTestEnd(model::Status::PASSED);

		setCurrentTime(444);
		listener.onTestSuiteEnd(model::Status::PASSED);

		listener.onProgramEnd();

		// In Allure 2 format: 1 test case result + 1 container + 3 metadata files = 5 files
		ASSERT_EQ(5, getSavedFilesCount());

		// Verify test case result file
		bool foundTestCaseFile = false;
		bool foundContainerFile = false;
		bool foundEnvironmentFile = false;
		bool foundExecutorFile = false;
		bool foundCategoriesFile = false;

		for (size_t i = 0; i < getSavedFilesCount(); i++)
		{
			StubFile file = getSavedFile(i);

			if (file.m_path.find("test-uuid-1-result.json") != std::string::npos)
			{
				foundTestCaseFile = true;
				// Parse and check key fields (ignoring auto-generated fields like host, thread, historyId)
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-1", actual["uuid"]);
				ASSERT_EQ("test-uuid-1", actual["testCaseId"]);
				ASSERT_EQ("SingleTestCase", actual["name"]);
				ASSERT_EQ("SingleTestSuite.SingleTestCase", actual["fullName"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ("finished", actual["stage"]);
				ASSERT_EQ(222, actual["start"]);
				ASSERT_EQ(333, actual["stop"]);
				// Verify some expected labels exist
				ASSERT_TRUE(actual.contains("labels"));
				bool foundSuiteLabel = false;
				for (const auto& label : actual["labels"])
				{
					if (label["name"] == "suite" && label["value"] == "SingleTestSuite")
					{
						foundSuiteLabel = true;
						break;
					}
				}
				ASSERT_TRUE(foundSuiteLabel);
			}
			else if (file.m_path.find("suite-uuid-1-container.json") != std::string::npos)
			{
				foundContainerFile = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("suite-uuid-1", actual["uuid"]);
				ASSERT_EQ("SingleTestSuite", actual["name"]);
				ASSERT_EQ(111, actual["start"]);
				ASSERT_EQ(444, actual["stop"]);
				ASSERT_EQ(1, actual["children"].size());
				ASSERT_EQ("test-uuid-1", actual["children"][0]);
				ASSERT_TRUE(actual["befores"].is_array());
				ASSERT_TRUE(actual["afters"].is_array());
			}
			else if (file.m_path.find("environment.properties") != std::string::npos)
			{
				foundEnvironmentFile = true;
				// Just verify it exists and has some content
				ASSERT_FALSE(file.m_content.empty());
			}
			else if (file.m_path.find("executor.json") != std::string::npos)
			{
				foundExecutorFile = true;
				// Just verify it exists and has some content
				ASSERT_FALSE(file.m_content.empty());
			}
			else if (file.m_path.find("categories.json") != std::string::npos)
			{
				foundCategoriesFile = true;
				// Just verify it exists and has some content
				ASSERT_FALSE(file.m_content.empty());
			}
		}

		ASSERT_TRUE(foundTestCaseFile) << "Test case result file not found";
		ASSERT_TRUE(foundContainerFile) << "Container file not found";
		ASSERT_TRUE(foundEnvironmentFile) << "Environment properties file not found";
		ASSERT_TRUE(foundExecutorFile) << "Executor JSON file not found";
		ASSERT_TRUE(foundCategoriesFile) << "Categories JSON file not found";
	}

	TEST_F(BasicTestCaseIntegrationTest, testProgramWithCoupleOfTestSuites)
	{
		auto& testProgram = detail::Core::instance().getTestProgram();
		testProgram.setOutputFolder("IntegrationTest\\OutputFolder");
		testProgram.setName("CoupleOfBasicTestSuites");
		testProgram.setTMSLinksPattern("http://{}");

		auto& listener = getEventListener();
		listener.onProgramStart();

		setCurrentTime(101);
		setNextUUIDToGenerate("suite-uuid-1");
		listener.onTestSuiteStart("TestSuite1");
		setNextUUIDToGenerate("test-uuid-1");
		listener.onTestStart("TestCase1");
		test().name("TestCase1");
		setCurrentTime(201);
		listener.onTestEnd(model::Status::FAILED);
		listener.onTestSuiteEnd(model::Status::FAILED);

		setCurrentTime(301);
		setNextUUIDToGenerate("suite-uuid-2");
		listener.onTestSuiteStart("TestSuite2");
		setNextUUIDToGenerate("test-uuid-2-1");
		listener.onTestStart("TestCase2.1");
		test().name("TestCase2.1");
		setCurrentTime(401);
		listener.onTestEnd(model::Status::PASSED);
		setNextUUIDToGenerate("test-uuid-2-2");
		listener.onTestStart("TestCase2.2");
		test().name("TestCase2.2");
		setCurrentTime(501);
		listener.onTestEnd(model::Status::PASSED);
		setNextUUIDToGenerate("test-uuid-2-3");
		listener.onTestStart("TestCase2.3");
		test().name("TestCase2.3");
		setCurrentTime(601);
		listener.onTestEnd(model::Status::SKIPPED);
		listener.onTestSuiteEnd(model::Status::PASSED);

		listener.onProgramEnd();

		// 4 test case result files + 2 container files + 3 metadata files = 9 files
		ASSERT_EQ(9, getSavedFilesCount());

		// Verify test case result files exist with proper content
		bool foundTest1 = false;
		bool foundTest21 = false;
		bool foundTest22 = false;
		bool foundTest23 = false;
		bool foundContainer1 = false;
		bool foundContainer2 = false;

		for (size_t i = 0; i < getSavedFilesCount(); i++)
		{
			StubFile file = getSavedFile(i);

			if (file.m_path.find("test-uuid-1-result.json") != std::string::npos)
			{
				foundTest1 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-1", actual["uuid"]);
				ASSERT_EQ("TestCase1", actual["name"]);
				ASSERT_EQ("failed", actual["status"]);
				ASSERT_EQ(101, actual["start"]);
				ASSERT_EQ(201, actual["stop"]);
			}
			else if (file.m_path.find("test-uuid-2-1-result.json") != std::string::npos)
			{
				foundTest21 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-2-1", actual["uuid"]);
				ASSERT_EQ("TestCase2.1", actual["name"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ(301, actual["start"]);
				ASSERT_EQ(401, actual["stop"]);
			}
			else if (file.m_path.find("test-uuid-2-2-result.json") != std::string::npos)
			{
				foundTest22 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-2-2", actual["uuid"]);
				ASSERT_EQ("TestCase2.2", actual["name"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ(401, actual["start"]);
				ASSERT_EQ(501, actual["stop"]);
			}
			else if (file.m_path.find("test-uuid-2-3-result.json") != std::string::npos)
			{
				foundTest23 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-2-3", actual["uuid"]);
				ASSERT_EQ("TestCase2.3", actual["name"]);
				ASSERT_EQ("skipped", actual["status"]);
				ASSERT_EQ(501, actual["start"]);
				ASSERT_EQ(601, actual["stop"]);
			}
			else if (file.m_path.find("suite-uuid-1-container.json") != std::string::npos)
			{
				foundContainer1 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("suite-uuid-1", actual["uuid"]);
				ASSERT_EQ("TestSuite1", actual["name"]);
				ASSERT_EQ(1, actual["children"].size());
				ASSERT_EQ("test-uuid-1", actual["children"][0]);
			}
			else if (file.m_path.find("suite-uuid-2-container.json") != std::string::npos)
			{
				foundContainer2 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("suite-uuid-2", actual["uuid"]);
				ASSERT_EQ("TestSuite2", actual["name"]);
				ASSERT_EQ(3, actual["children"].size());
				ASSERT_EQ("test-uuid-2-1", actual["children"][0]);
				ASSERT_EQ("test-uuid-2-2", actual["children"][1]);
				ASSERT_EQ("test-uuid-2-3", actual["children"][2]);
			}
		}

		ASSERT_TRUE(foundTest1) << "Test case 1 result file not found";
		ASSERT_TRUE(foundTest21) << "Test case 2.1 result file not found";
		ASSERT_TRUE(foundTest22) << "Test case 2.2 result file not found";
		ASSERT_TRUE(foundTest23) << "Test case 2.3 result file not found";
		ASSERT_TRUE(foundContainer1) << "Container 1 file not found";
		ASSERT_TRUE(foundContainer2) << "Container 2 file not found";
	}

	TEST_F(BasicTestCaseIntegrationTest, testProgramWithSingleComplexTestSuite)
	{
		auto& testProgram = detail::Core::instance().getTestProgram();
		testProgram.setOutputFolder("IntegrationTest\\OutputFolder");
		testProgram.setName("ComplexTestProgram");
		testProgram.setTMSLinksPattern("https://mytms.webpage.com/{}/refresh");

		auto& listener = getEventListener();
		listener.onProgramStart();

		setCurrentTime(100);
		setNextUUIDToGenerate("UUID-Complex");
		listener.onTestSuiteStart("SingleComplexTestSuite");

		suite()
			.label("tmsId", "UTILS-TC-0001")
			.epic("Complex Epic")
			.name("MyComplexTestSuite")
			.description("Description of complex test suite")
			.severity("high")
			.label("tag", "ThisIsMyTag");

		for (unsigned int i = 1; i <= 3; i++)
		{
			setCurrentTime(100 + (100 * i));
			setNextUUIDToGenerate("test-uuid-" + std::to_string(i));
			listener.onTestStart("ParametricTest/" + std::to_string(i));

			test().name("Description of action for scenario " + std::to_string(i));
			step("Description of expectation for scenario " + std::to_string(i), [](){});

			setCurrentTime(190 + (100 * i));
			listener.onTestEnd(model::Status::PASSED);
		}

		setCurrentTime(500);
		listener.onTestSuiteEnd(model::Status::PASSED);

		listener.onProgramEnd();

		// 3 test case result files + 1 container + 3 metadata files = 7 files
		ASSERT_EQ(7, getSavedFilesCount());

		// Verify test case result files with proper content
		bool foundTest1 = false;
		bool foundTest2 = false;
		bool foundTest3 = false;
		bool foundContainer = false;

		for (size_t i = 0; i < getSavedFilesCount(); i++)
		{
			StubFile file = getSavedFile(i);

			if (file.m_path.find("test-uuid-1-result.json") != std::string::npos)
			{
				foundTest1 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-1", actual["uuid"]);
				ASSERT_EQ("Description of action for scenario 1", actual["name"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ(200, actual["start"]);
				ASSERT_EQ(290, actual["stop"]);
				ASSERT_EQ(1, actual["steps"].size());
				ASSERT_EQ("Action: Description of expectation for scenario 1", actual["steps"][0]["name"]);
			}
			else if (file.m_path.find("test-uuid-2-result.json") != std::string::npos)
			{
				foundTest2 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-2", actual["uuid"]);
				ASSERT_EQ("Description of action for scenario 2", actual["name"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ(300, actual["start"]);
				ASSERT_EQ(390, actual["stop"]);
				ASSERT_EQ(1, actual["steps"].size());
				ASSERT_EQ("Action: Description of expectation for scenario 2", actual["steps"][0]["name"]);
			}
			else if (file.m_path.find("test-uuid-3-result.json") != std::string::npos)
			{
				foundTest3 = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("test-uuid-3", actual["uuid"]);
				ASSERT_EQ("Description of action for scenario 3", actual["name"]);
				ASSERT_EQ("passed", actual["status"]);
				ASSERT_EQ(400, actual["start"]);
				ASSERT_EQ(490, actual["stop"]);
				ASSERT_EQ(1, actual["steps"].size());
				ASSERT_EQ("Action: Description of expectation for scenario 3", actual["steps"][0]["name"]);
			}
			else if (file.m_path.find("UUID-Complex-container.json") != std::string::npos)
			{
				foundContainer = true;
				nlohmann::json actual = nlohmann::json::parse(file.m_content);
				ASSERT_EQ("UUID-Complex", actual["uuid"]);
				ASSERT_EQ("MyComplexTestSuite", actual["name"]);
				ASSERT_EQ(100, actual["start"]);
				ASSERT_EQ(500, actual["stop"]);
				ASSERT_EQ(3, actual["children"].size());
				ASSERT_EQ("test-uuid-1", actual["children"][0]);
				ASSERT_EQ("test-uuid-2", actual["children"][1]);
				ASSERT_EQ("test-uuid-3", actual["children"][2]);
			}
		}

		ASSERT_TRUE(foundTest1) << "Test case 1 result file not found";
		ASSERT_TRUE(foundTest2) << "Test case 2 result file not found";
		ASSERT_TRUE(foundTest3) << "Test case 3 result file not found";
		ASSERT_TRUE(foundContainer) << "Container file not found";
	}


}}}
