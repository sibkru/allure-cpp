#include "TestProgramJSONBuilder.h"

#include "Model/Container.h"
#include "Model/TestCase.h"
#include "Model/TestProgram.h"
#include "Model/TestSuite.h"
#include "Services/Report/ITestCaseJSONSerializer.h"
#include "Services/Report/IContainerJSONSerializer.h"
#include "Services/System/IFileService.h"

#include <chrono>

#ifdef _WIN32
	#define PATH_SEPARATOR "\\"
#else
	#define PATH_SEPARATOR "/"
#endif


namespace allure_cpp { namespace service {

	TestProgramJSONBuilder::TestProgramJSONBuilder(std::unique_ptr<ITestCaseJSONSerializer> testCaseJSONSerializer,
												   std::unique_ptr<IContainerJSONSerializer> containerJSONSerializer,
												   std::unique_ptr<IFileService> fileService)
		:m_testCaseJSONSerializer(std::move(testCaseJSONSerializer))
		,m_containerJSONSerializer(std::move(containerJSONSerializer))
		,m_fileService(std::move(fileService))
	{
	}

	void TestProgramJSONBuilder::buildJSONFiles(const model::TestProgram& testProgram) const
	{
		// NOTE: This method is kept for backwards compatibility but is no longer the recommended approach.
		// For better memory management (especially with CppUTest's leak detection), use the new approach where:
		// - TestCaseEndEventHandler writes test case JSON immediately after each test
		// - TestSuiteEndEventHandler writes container JSON immediately after each suite
		// - TestProgramEndEventHandler writes only metadata files

		unsigned int nTestSuites = (unsigned int) testProgram.getTestSuitesCount();
		for (unsigned int i = 0; i < nTestSuites; i++)
		{
			const model::TestSuite& testSuite = testProgram.getTestSuite(i);

			// Create container for this test suite
			model::Container container = createContainerFromTestSuite(testSuite);

			// Generate result file for each test case in the suite
			const auto& testCases = testSuite.getTestCases();
			for (const auto& testCase : testCases)
			{
				// Generate test case result file: {uuid}-result.json
				std::string testCaseFilepath = testProgram.getOutputFolder() + PATH_SEPARATOR + testCase.getUUID() + "-result.json";
				std::string testCaseContent = m_testCaseJSONSerializer->serialize(testCase);
				m_fileService->saveFile(testCaseFilepath, testCaseContent);
			}

			// Generate container file for test suite: {uuid}-container.json
			std::string containerFilepath = testProgram.getOutputFolder() + PATH_SEPARATOR + container.getUUID() + "-container.json";
			std::string containerContent = m_containerJSONSerializer->serialize(container);
			m_fileService->saveFile(containerFilepath, containerContent);
		}

		// Generate metadata files for Allure 2 compatibility
		buildMetadataFiles(testProgram);
	}

	void TestProgramJSONBuilder::buildMetadataFiles(const model::TestProgram& testProgram) const
	{
		const std::string outputFolder = testProgram.getOutputFolder();

		// Generate environment.properties
		generateEnvironmentProperties(outputFolder, testProgram);

		// Generate executor.json
		generateExecutorJson(outputFolder, testProgram);

		// Generate categories.json
		generateCategoriesJson(outputFolder);
	}

	model::Container TestProgramJSONBuilder::createContainerFromTestSuite(const model::TestSuite& testSuite) const
	{
		model::Container container;
		container.setUUID(testSuite.getUUID());
		container.setName(testSuite.getName());
		container.setStart(testSuite.getStart());
		container.setStop(testSuite.getStop());

		// Add each test case UUID as a child
		const auto& testCases = testSuite.getTestCases();
		for (const auto& testCase : testCases)
		{
			container.addChild(testCase.getUUID());
		}

		// Note: befores/afters (SetUp/TearDown) are currently not tracked in TestSuite
		// This could be enhanced in the future to track fixture setup/teardown

		return container;
	}

	void TestProgramJSONBuilder::generateEnvironmentProperties(const std::string& outputFolder, const model::TestProgram& testProgram) const
	{
		std::string filepath = outputFolder + PATH_SEPARATOR + "environment.properties";
		std::string content;

		// Add platform information
		#ifdef _WIN32
			content += "OS=Windows\n";
		#elif __APPLE__
			content += "OS=macOS\n";
		#elif __linux__
			content += "OS=Linux\n";
		#else
			content += "OS=Unknown\n";
		#endif

		// Add architecture
		#if defined(__x86_64__) || defined(_M_X64)
			content += "Architecture=x86_64\n";
		#elif defined(__i386__) || defined(_M_IX86)
			content += "Architecture=x86\n";
		#elif defined(__aarch64__) || defined(_M_ARM64)
			content += "Architecture=ARM64\n";
		#else
			content += "Architecture=Unknown\n";
		#endif

		content += "Framework=" + testProgram.getFrameworkName() + "\n";
		content += "Language=C++\n";

		m_fileService->saveFile(filepath, content);
	}

	void TestProgramJSONBuilder::generateExecutorJson(const std::string& outputFolder, const model::TestProgram& testProgram) const
	{
		std::string filepath = outputFolder + PATH_SEPARATOR + "executor.json";

		// Use current timestamp as buildOrder for trend tracking
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		std::string content = "{\n"
		                      "  \"name\": \"" + testProgram.getFrameworkName() + "\",\n"
		                      "  \"type\": \"local\",\n"
		                      "  \"buildName\": \"Manual Run\",\n"
		                      "  \"buildOrder\": " + std::to_string(millis) + "\n"
		                      "}";

		m_fileService->saveFile(filepath, content);
	}

	void TestProgramJSONBuilder::generateCategoriesJson(const std::string& outputFolder) const
	{
		std::string filepath = outputFolder + PATH_SEPARATOR + "categories.json";
		std::string content = "[\n"
		                      "  {\n"
		                      "    \"name\": \"Product defects\",\n"
		                      "    \"matchedStatuses\": [\"failed\"]\n"
		                      "  },\n"
		                      "  {\n"
		                      "    \"name\": \"Test defects\",\n"
		                      "    \"matchedStatuses\": [\"broken\"]\n"
		                      "  }\n"
		                      "]";

		m_fileService->saveFile(filepath, content);
	}

}} // namespace allure_cpp::service
