#include "TestSuiteStartEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/Report/ITestSuiteJSONSerializer.h"
#include "Services/System/IFileService.h"
#include "Services/System/ITimeService.h"
#include "Services/System/IUUIDGeneratorService.h"

#include <sstream>
#include <iomanip>


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
	}

	TestSuiteStartEventHandler::TestSuiteStartEventHandler(model::TestProgram& testProgram,
														   std::unique_ptr<IUUIDGeneratorService> uuidGeneratorService,
														   std::unique_ptr<ITimeService> timeService)
		:m_testProgram(testProgram)
		,m_uuidGeneratorService(std::move(uuidGeneratorService))
		,m_timeService(std::move(timeService))
	{
	}

	TestSuiteStartEventHandler::~TestSuiteStartEventHandler() = default;

	void TestSuiteStartEventHandler::handleTestSuiteStart(const std::string& testSuiteName) const
	{
		model::TestSuite testSuite;
		std::string uuid = m_uuidGeneratorService->generateUUID();

		testSuite.setUUID(uuid);
		testSuite.setFormat(m_testProgram.getFormat());
		testSuite.setName(testSuiteName);
		testSuite.setTmsId(testSuiteName);

		// Set fullName (for GoogleTest, this is the test suite name)
		testSuite.setFullName(testSuiteName);

		// Generate historyId from the test suite name for consistent test history tracking
		testSuite.setHistoryId(generateSimpleHash(testSuiteName));

		// Set testCaseId to the same as UUID for now
		testSuite.setTestCaseId(uuid);

		testSuite.setStart(m_timeService->getCurrentTime());
		testSuite.setStage(model::Stage::RUNNING);
		testSuite.setStatus(model::Status::UNKNOWN);

		m_testProgram.addTestSuite(testSuite);

		// Update cache pointer to the newly added test suite
		m_testProgram.setRunningTestSuite(&m_testProgram.getTestSuite(m_testProgram.getTestSuitesCount() - 1));
	}

}} // namespace allure::service
