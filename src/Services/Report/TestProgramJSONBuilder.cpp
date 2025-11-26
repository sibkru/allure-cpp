#include "TestProgramJSONBuilder.h"

#include "Model/Container.h"
#include "Model/TestCase.h"
#include "Model/TestProgram.h"
#include "Model/TestSuite.h"
#include "Services/Report/ITestCaseJSONSerializer.h"
#include "Services/Report/IContainerJSONSerializer.h"
#include "Services/System/IFileService.h"

#include <chrono>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

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

		if (shouldPreserveExistingExecutor(filepath))
		{
			return;
		}

		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		const auto buildOrder = resolveBuildOrder(testProgram, millis, outputFolder);
		const auto buildName = resolveBuildName(testProgram, now);
		const auto executorName = resolveExecutorName(testProgram);

		std::string content = "{\n"
		                      "  \"name\": \"" + escapeJsonString(executorName) + "\",\n"
		                      "  \"type\": \"local\",\n"
		                      "  \"buildName\": \"" + escapeJsonString(buildName) + "\",\n"
		                      "  \"buildOrder\": " + buildOrder + "\n"
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

	std::string TestProgramJSONBuilder::resolveBuildOrder(const model::TestProgram& testProgram, long long currentMillis, const std::string& outputFolder) const
	{
		const auto explicitOrder = testProgram.getExecutorBuildOrder();
		if (isNumeric(explicitOrder))
		{
			return explicitOrder;
		}

		const auto envOrder = getEnvVar("ALLURE_BUILD_ORDER");
		if (isNumeric(envOrder))
		{
			return envOrder;
		}

		auto nextFromHistory = nextBuildOrderFromHistory(outputFolder);
		if (nextFromHistory > 0)
		{
			return std::to_string(nextFromHistory);
		}

		// Fallback: start at 1 if no history exists. Millis is a last resort for uniqueness.
		if (currentMillis > 0)
		{
			return "1";
		}

		return "1";
	}

	std::string TestProgramJSONBuilder::resolveBuildName(const model::TestProgram& testProgram, const std::chrono::system_clock::time_point& now) const
	{
		const auto explicitName = testProgram.getExecutorBuildName();
		if (!explicitName.empty())
		{
			return explicitName;
		}

		const auto envName = getEnvVar("ALLURE_BUILD_NAME");
		if (!envName.empty())
		{
			return envName;
		}

		const auto ciName = buildCiName();
		if (!ciName.empty())
		{
			return ciName;
		}

		return formatTimestamp(now);
	}

	std::string TestProgramJSONBuilder::resolveExecutorName(const model::TestProgram& testProgram) const
	{
		const auto envName = getEnvVar("ALLURE_EXECUTOR_NAME");
		if (!envName.empty())
		{
			return envName;
		}

		if (!testProgram.getFrameworkName().empty())
		{
			return testProgram.getFrameworkName();
		}

		return "unknown";
	}

	std::string TestProgramJSONBuilder::formatTimestamp(const std::chrono::system_clock::time_point& now) const
	{
		std::time_t timeValue = std::chrono::system_clock::to_time_t(now);
		std::tm localTime;
#if defined(_WIN32)
		localtime_s(&localTime, &timeValue);
#else
		localtime_r(&timeValue, &localTime);
#endif

		std::ostringstream stream;
		stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
		return stream.str();
	}

	std::string TestProgramJSONBuilder::buildCiName() const
	{
		const auto githubRun = getEnvVar("GITHUB_RUN_ID");
		if (!githubRun.empty())
		{
			const auto workflow = getEnvVar("GITHUB_WORKFLOW");
			const auto job = getEnvVar("GITHUB_JOB");
			const auto branch = getEnvVar("GITHUB_REF_NAME");
			auto sha = getEnvVar("GITHUB_SHA");
			if (sha.size() > 7)
			{
				sha = sha.substr(0, 7);
			}

			std::ostringstream stream;
			if (!workflow.empty())
			{
				stream << workflow;
			}
			if (!job.empty())
			{
				if (stream.tellp() > 0) stream << " • ";
				stream << job;
			}
			if (!branch.empty())
			{
				if (stream.tellp() > 0) stream << " • ";
				stream << branch;
			}
			if (!sha.empty())
			{
				if (stream.tellp() > 0) stream << " • ";
				stream << sha;
			}

			return stream.str();
		}

		const auto gitlabJob = getEnvVar("CI_JOB_NAME");
		if (!gitlabJob.empty())
		{
			const auto branch = getEnvVar("CI_COMMIT_BRANCH");
			const auto shortSha = getEnvVar("CI_COMMIT_SHORT_SHA");

			std::ostringstream stream;
			stream << gitlabJob;
			if (!branch.empty())
			{
				stream << " • " << branch;
			}
			if (!shortSha.empty())
			{
				stream << " • " << shortSha;
			}

			return stream.str();
		}

		const auto jenkinsJob = getEnvVar("JOB_NAME");
		if (!jenkinsJob.empty())
		{
			const auto buildNumber = getEnvVar("BUILD_NUMBER");

			std::ostringstream stream;
			stream << jenkinsJob;
			if (!buildNumber.empty())
			{
				stream << " #" << buildNumber;
			}

			return stream.str();
		}

		const auto azureDefinition = getEnvVar("BUILD_DEFINITIONNAME");
		const auto azureBuildNumber = getEnvVar("BUILD_BUILDNUMBER");
		if (!azureDefinition.empty() || !azureBuildNumber.empty())
		{
			std::ostringstream stream;
			if (!azureDefinition.empty())
			{
				stream << azureDefinition;
			}
			if (!azureBuildNumber.empty())
			{
				if (stream.tellp() > 0) stream << " ";
				stream << azureBuildNumber;
			}

			return stream.str();
		}

		return "";
	}

	std::string TestProgramJSONBuilder::escapeJsonString(const std::string& value) const
	{
		std::string escaped;
		escaped.reserve(value.size());

		for (const auto ch : value)
		{
			switch (ch)
			{
				case '\"': escaped += "\\\""; break;
				case '\\': escaped += "\\\\"; break;
				case '\b': escaped += "\\b"; break;
				case '\f': escaped += "\\f"; break;
				case '\n': escaped += "\\n"; break;
				case '\r': escaped += "\\r"; break;
				case '\t': escaped += "\\t"; break;
				default: escaped += ch; break;
			}
		}

		return escaped;
	}

	bool TestProgramJSONBuilder::shouldPreserveExistingExecutor(const std::string& filepath) const
	{
		const auto preserveValue = getEnvVar("ALLURE_EXECUTOR_PRESERVE");
		if (preserveValue.empty())
		{
			return false;
		}

		if (!isTruthy(preserveValue))
		{
			return false;
		}

		return fileExists(filepath);
	}

	bool TestProgramJSONBuilder::fileExists(const std::string& filepath) const
	{
		std::ifstream file(filepath);
		return file.good();
	}

	std::string TestProgramJSONBuilder::getEnvVar(const std::string& name) const
	{
		const char* value = std::getenv(name.c_str());
		if (value)
		{
			return std::string(value);
		}

		return "";
	}

	bool TestProgramJSONBuilder::isTruthy(const std::string& value) const
	{
		std::string lowered = value;
		std::transform(lowered.begin(), lowered.end(), lowered.begin(),
					   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

		return (lowered == "1") || (lowered == "true") || (lowered == "yes") || (lowered == "on");
	}

	bool TestProgramJSONBuilder::isNumeric(const std::string& value) const
	{
		if (value.empty())
		{
			return false;
		}

		return std::all_of(value.begin(), value.end(),
						   [](unsigned char c) { return std::isdigit(c) != 0; });
	}

	long long TestProgramJSONBuilder::nextBuildOrderFromHistory(const std::string& outputFolder) const
	{
		const auto historyPath = historyFilepath(outputFolder);
		if (!fileExists(historyPath))
		{
			return 1;
		}

		try
		{
			std::ifstream file(historyPath);
			if (!file.is_open())
			{
				return 1;
			}

			nlohmann::json jsonData;
			file >> jsonData;

			long long maxOrder = 0;
			for (const auto& item : jsonData)
			{
				if (item.contains("buildOrder"))
				{
					long long order = 0;
					if (item["buildOrder"].is_number())
					{
						order = item["buildOrder"].get<long long>();
					}
					else if (item["buildOrder"].is_string())
					{
						const auto text = item["buildOrder"].get<std::string>();
						if (isNumeric(text))
						{
							order = std::stoll(text);
						}
					}

					if (order > maxOrder)
					{
						maxOrder = order;
					}
				}
			}

			return maxOrder + 1;
		}
		catch (...)
		{
			return 1;
		}
	}

	std::string TestProgramJSONBuilder::historyFilepath(const std::string& outputFolder) const
	{
		return outputFolder + PATH_SEPARATOR + "history" + PATH_SEPARATOR + "history-trend.json";
	}

}} // namespace allure_cpp::service
