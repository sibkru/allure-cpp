#include "TestSuiteEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Model/Container.h"
#include "Services/System/ITimeService.h"
#include "Services/Report/IContainerJSONSerializer.h"
#include "Services/System/IFileService.h"

#include <regex>

#ifdef _WIN32
	#define PATH_SEPARATOR "\\"
#else
	#define PATH_SEPARATOR "/"
#endif


namespace allure { namespace service {

	TestSuiteEndEventHandler::TestSuiteEndEventHandler(model::TestProgram& testProgram,
													   std::unique_ptr<ITimeService> timeService,
													   std::unique_ptr<IContainerJSONSerializer> containerJSONSerializer,
													   std::unique_ptr<IFileService> fileService)
		:m_testProgram(testProgram)
		,m_timeService(std::move(timeService))
		,m_containerJSONSerializer(std::move(containerJSONSerializer))
		,m_fileService(std::move(fileService))
	{
	}

	void TestSuiteEndEventHandler::handleTestSuiteEnd(model::Status status) const
	{
		model::TestSuite& testSuite = getRunningTestSuite();
		testSuite.setStop(m_timeService->getCurrentTime());
		testSuite.setStage(model::Stage::FINISHED);
		testSuite.setStatus(status);
		addTMSLink(testSuite);

		// Write container JSON immediately after suite completes
		writeContainerJSON(testSuite);

		// Clear the cache since the test suite is no longer running
		m_testProgram.setRunningTestSuite(nullptr);
		m_testProgram.setRunningTestCase(nullptr);
	}

	void TestSuiteEndEventHandler::writeContainerJSON(const model::TestSuite& testSuite) const
	{
		// Create container from test suite
		model::Container container;
		container.setUUID(testSuite.getUUID());
		container.setName(testSuite.getName());
		container.setStart(testSuite.getStart());
		container.setStop(testSuite.getStop());

		// Add test case UUIDs as children by iterating through them
		for (const auto& testCase : testSuite.getTestCases())
		{
			container.addChild(testCase.getUUID());
		}

		// Write container file: {uuid}-container.json
		std::string filepath = m_testProgram.getOutputFolder() + PATH_SEPARATOR + container.getUUID() + "-container.json";
		std::string content = m_containerJSONSerializer->serialize(container);
		m_fileService->saveFile(filepath, content);
	}

	model::TestSuite& TestSuiteEndEventHandler::getRunningTestSuite() const
	{
		model::TestSuite* testSuite = m_testProgram.getRunningTestSuite();
		if (!testSuite)
		{
			throw NoRunningTestSuiteException();
		}
		return *testSuite;
	}

	void TestSuiteEndEventHandler::addTMSLink(model::TestSuite& testSuite) const
	{
		auto tmsId = testSuite.getTmsId();
		auto tmsLinksPattern = m_testProgram.getTMSLinksPattern();
		auto tmsURL = tmsLinksPattern.replace(tmsLinksPattern.find("{}"), 2, tmsId);

		model::Link tmsLink;
		tmsLink.setName(tmsId);
		tmsLink.setURL(tmsURL);
		tmsLink.setType("tms");
		testSuite.addLink(tmsLink);
	}

}} // namespace allure::service
