#include "stdafx.h"
#include "Services/Report/TestProgramJSONBuilder.h"

#include "Model/TestCase.h"
#include "Model/TestSuite.h"
#include "Model/TestProgram.h"

#include "TestUtilities/Mocks/Services/System/MockFileService.h"
#include "TestUtilities/Mocks/Services/Report/MockTestCaseJSONSerializer.h"
#include "TestUtilities/Mocks/Services/Report/MockContainerJSONSerializer.h"


using namespace testing;
using namespace allure_cpp;
using namespace allure_cpp::test_utility;

namespace systelab { namespace gtest_allure_utilities { namespace unit_test {

	class TestProgramJSONBuilderTest : public testing::Test
	{
		void SetUp()
		{
			m_testProgram = buildTestProgram();
			auto testCaseJSONSerializer = buildTestCaseJSONSerializer();
			auto containerJSONSerializer = buildContainerJSONSerializer();
			auto fileService = buildFileService();

			m_service = std::make_unique<service::TestProgramJSONBuilder>(
				std::move(testCaseJSONSerializer),
				std::move(containerJSONSerializer),
				std::move(fileService));
		}

		std::unique_ptr<model::TestProgram> buildTestProgram()
		{
			m_testProgramName = "MyTestProgram";
			m_outputFolder = "TestProgramJSONBuilderTest\\Reports";
			m_testSuiteUUIDs = { "SuiteUUID1", "SuiteUUID2" };
			m_testCaseUUIDs = { {"TestUUID1", "TestUUID2"}, {"TestUUID3"} };

			auto testProgram = std::make_unique<model::TestProgram>();
			testProgram->setName(m_testProgramName);
			testProgram->setOutputFolder(m_outputFolder);

			for (size_t i = 0; i < m_testSuiteUUIDs.size(); i++)
			{
				model::TestSuite testSuite;
				testSuite.setUUID(m_testSuiteUUIDs[i]);

				// Add test cases to each suite
				for (const auto& testCaseUUID : m_testCaseUUIDs[i])
				{
					model::TestCase testCase;
					testCase.setUUID(testCaseUUID);
					testCase.setName("Test" + testCaseUUID);
					testSuite.addTestCase(testCase);
				}

				testProgram->addTestSuite(testSuite);
			}

			return testProgram;
		}

		std::unique_ptr<service::ITestCaseJSONSerializer> buildTestCaseJSONSerializer()
		{
			auto testCaseJSONSerializer = std::make_unique<MockTestCaseJSONSerializer>();
			m_testCaseJSONSerializer = testCaseJSONSerializer.get();

			ON_CALL(*m_testCaseJSONSerializer, serialize(_)).WillByDefault(Invoke(
				[](const model::TestCase& testCase) -> std::string
				{
					return std::string("SerializedTestCase") + testCase.getUUID();
				}
			));

			return testCaseJSONSerializer;
		}

		std::unique_ptr<service::IContainerJSONSerializer> buildContainerJSONSerializer()
		{
			auto containerJSONSerializer = std::make_unique<MockContainerJSONSerializer>();
			m_containerJSONSerializer = containerJSONSerializer.get();

			ON_CALL(*m_containerJSONSerializer, serialize(_)).WillByDefault(Invoke(
				[](const model::Container& container) -> std::string
				{
					return std::string("SerializedContainer") + container.getUUID();
				}
			));

			return containerJSONSerializer;
		}

		std::unique_ptr<service::IFileService> buildFileService()
		{
			auto fileService = std::make_unique<MockFileService>();
			m_fileService = fileService.get();
			return fileService;
		}

	protected:
		std::unique_ptr<service::TestProgramJSONBuilder> m_service;
		MockTestCaseJSONSerializer* m_testCaseJSONSerializer;
		MockContainerJSONSerializer* m_containerJSONSerializer;
		MockFileService* m_fileService;

		std::unique_ptr<model::TestProgram> m_testProgram;
		std::string m_testProgramName;
		std::string m_outputFolder;
		std::vector<std::string> m_testSuiteUUIDs;
		std::vector<std::vector<std::string>> m_testCaseUUIDs;
	};


	TEST_F(TestProgramJSONBuilderTest, testBuildJSONFilesSavesAFileForEachTestCase)
	{
#ifdef _WIN32
		const char* PATH_SEP = "\\";
#else
		const char* PATH_SEP = "/";
#endif

		// Expect one result file per test case (3 test cases total)
		for (const auto& testCaseList : m_testCaseUUIDs)
		{
			for (const auto& testCaseUUID : testCaseList)
			{
				std::string expectedFilepath = m_outputFolder + PATH_SEP + testCaseUUID + "-result.json";
				std::string expectedFileContent = "SerializedTestCase" + testCaseUUID;
				EXPECT_CALL(*m_fileService, saveFile(expectedFilepath, expectedFileContent));
			}
		}

		// Expect one container file per test suite (2 suites)
		for (const auto& testSuiteUUID : m_testSuiteUUIDs)
		{
			std::string expectedFilepath = m_outputFolder + PATH_SEP + testSuiteUUID + "-container.json";
			std::string expectedFileContent = "SerializedContainer" + testSuiteUUID;
			EXPECT_CALL(*m_fileService, saveFile(expectedFilepath, expectedFileContent));
		}

		// Expect metadata files (environment.properties, executor.json, categories.json)
		EXPECT_CALL(*m_fileService, saveFile(m_outputFolder + PATH_SEP + "environment.properties", _));
		EXPECT_CALL(*m_fileService, saveFile(m_outputFolder + PATH_SEP + "executor.json", _));
		EXPECT_CALL(*m_fileService, saveFile(m_outputFolder + PATH_SEP + "categories.json", _));

		m_service->buildJSONFiles(*m_testProgram);
	}

	TEST_F(TestProgramJSONBuilderTest, testBuildJSONFilesDoesNotSaveAFileWhenSuiteHasNoTestCases)
	{
		// Even with no test suites, metadata files should still be generated
		EXPECT_CALL(*m_fileService, saveFile(_, _)).Times(3); // environment.properties, executor.json, categories.json

		model::TestProgram emptyTestProgram;
		m_service->buildJSONFiles(emptyTestProgram);
	}

}}}
