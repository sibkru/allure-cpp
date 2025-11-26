#include "TestCaseStartEventHandler.h"

#include "Model/TestProgram.h"
#include "Model/Label.h"
#include "Services/System/ITimeService.h"
#include "Services/System/IUUIDGeneratorService.h"

#include <sstream>
#include <iomanip>
#include <thread>

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	#include <unistd.h>
#endif


namespace allure { namespace service {

	namespace {
		// Simple hash function for generating historyId
		std::string generateSimpleHash(const std::string& input)
		{
			std::hash<std::string> hasher;
			size_t hashValue = hasher(input);
			std::stringstream ss;
			ss << std::hex << std::setfill('0') << std::setw(16) << hashValue;
			return ss.str();
		}

		std::string getHostname()
		{
			char hostname[256];
			#ifdef _WIN32
				WSADATA wsaData;
				WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (gethostname(hostname, sizeof(hostname)) == 0)
				{
					WSACleanup();
					return std::string(hostname);
				}
				WSACleanup();
			#else
				if (gethostname(hostname, sizeof(hostname)) == 0)
				{
					return std::string(hostname);
				}
			#endif
			return "localhost";
		}

		std::string getThreadId()
		{
			std::stringstream ss;
			ss << std::this_thread::get_id();
			return ss.str();
		}
	}

	TestCaseStartEventHandler::TestCaseStartEventHandler(model::TestProgram& testProgram,
														 std::unique_ptr<IUUIDGeneratorService> uuidGeneratorService,
														 std::unique_ptr<ITimeService> timeService)
		:m_testProgram(testProgram)
		,m_uuidGeneratorService(std::move(uuidGeneratorService))
		,m_timeService(std::move(timeService))
	{
	}

	void TestCaseStartEventHandler::handleTestCaseStart(const std::string& testCaseName) const
	{
		auto& testSuite = getRunningTestSuite();

		model::TestCase testCase;
		std::string uuid = m_uuidGeneratorService->generateUUID();

		testCase.setUUID(uuid);
		testCase.setName(testCaseName);

		// Set fullName (for GoogleTest, this is TestSuiteName.TestCaseName)
		std::string fullName = testSuite.getName() + "." + testCaseName;
		testCase.setFullName(fullName);

		// Generate historyId from the full name for consistent test history tracking
		testCase.setHistoryId(generateSimpleHash(fullName));

		// Set testCaseId to the same as UUID for now
		testCase.setTestCaseId(uuid);

		testCase.setStart(m_timeService->getCurrentTime());
		testCase.setStage(model::Stage::RUNNING);
		testCase.setStatus(model::Status::UNKNOWN);

		// Add common labels
		addCommonLabels(testCase, testSuite.getName());

		testSuite.addTestCase(testCase);

		// Update cache pointer to the newly added test case
		auto& testCases = testSuite.getTestCases();
		m_testProgram.setRunningTestCase(&testCases[testCases.size() - 1]);
	}

	void TestCaseStartEventHandler::handleTestCaseStart(const ITestMetadata& metadata) const
	{
		auto& testSuite = getRunningTestSuite();

		model::TestCase testCase;
		std::string uuid = m_uuidGeneratorService->generateUUID();

		testCase.setUUID(uuid);
		testCase.setName(metadata.getTestName());
		testCase.setFullName(metadata.getFullName());

		// Generate historyId from the full name for consistent test history tracking
		testCase.setHistoryId(generateSimpleHash(metadata.getFullName()));

		// Set testCaseId to the same as UUID for now
		testCase.setTestCaseId(uuid);

		testCase.setStart(m_timeService->getCurrentTime());
		testCase.setStage(model::Stage::RUNNING);
		testCase.setStatus(model::Status::UNKNOWN);

		// Add parameters if this is a parametric test
		if (metadata.hasTypeParameter())
		{
			model::Parameter typeParam;
			typeParam.setName("type");
			typeParam.setValue(metadata.getTypeParameter());
			testCase.addParameter(typeParam);
		}

		if (metadata.hasValueParameter())
		{
			model::Parameter valueParam;
			valueParam.setName("value");
			valueParam.setValue(metadata.getValueParameter());
			testCase.addParameter(valueParam);
		}

		// Add common labels
		addCommonLabels(testCase, metadata.getSuiteName());

		testSuite.addTestCase(testCase);

		// Update cache pointer to the newly added test case
		auto& testCases = testSuite.getTestCases();
		m_testProgram.setRunningTestCase(&testCases[testCases.size() - 1]);
	}

	void TestCaseStartEventHandler::addCommonLabels(model::TestCase& testCase, const std::string& suiteName) const
	{
		// Add critical labels for Allure 2 compatibility
		model::Label suiteLabel;
		suiteLabel.setName("suite");
		suiteLabel.setValue(suiteName);
		testCase.addLabel(suiteLabel);

		model::Label packageLabel;
		packageLabel.setName("package");
		packageLabel.setValue(suiteName);
		testCase.addLabel(packageLabel);

		model::Label frameworkLabel;
		frameworkLabel.setName("framework");
		frameworkLabel.setValue("googletest");
		testCase.addLabel(frameworkLabel);

		model::Label languageLabel;
		languageLabel.setName("language");
		languageLabel.setValue("cpp");
		testCase.addLabel(languageLabel);

		model::Label severityLabel;
		severityLabel.setName("severity");
		severityLabel.setValue("normal");
		testCase.addLabel(severityLabel);

		// Add host and thread labels for Timeline view
		model::Label hostLabel;
		hostLabel.setName("host");
		hostLabel.setValue(getHostname());
		testCase.addLabel(hostLabel);

		model::Label threadLabel;
		threadLabel.setName("thread");
		threadLabel.setValue(getThreadId());
		testCase.addLabel(threadLabel);
	}

	model::TestSuite& TestCaseStartEventHandler::getRunningTestSuite() const
	{
		model::TestSuite* testSuite = m_testProgram.getRunningTestSuite();
		if (!testSuite)
		{
			throw NoRunningTestSuiteException();
		}
		return *testSuite;
	}

}} // namespace allure::service
