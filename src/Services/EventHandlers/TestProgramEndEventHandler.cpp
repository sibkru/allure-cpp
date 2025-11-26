#include "TestProgramEndEventHandler.h"

#include "Model/TestProgram.h"
#include "Services/Report/ITestProgramJSONBuilder.h"


namespace allure { namespace service {

	TestProgramEndEventHandler::TestProgramEndEventHandler(model::TestProgram& testProgram,
														   std::unique_ptr<ITestProgramJSONBuilder> testProgramJSONBuilderService)
		:m_testProgram(testProgram)
		,m_testProgramJSONBuilderService(std::move(testProgramJSONBuilderService))
	{
	}

	void TestProgramEndEventHandler::handleTestProgramEnd() const
	{
		// Note: Test case and container JSON files are now written immediately
		// after each test/suite completes (by TestCaseEndEventHandler and TestSuiteEndEventHandler).
		// Here we only need to write the metadata files.
		m_testProgramJSONBuilderService->buildMetadataFiles(m_testProgram);
	}

}} // namespace allure::service
