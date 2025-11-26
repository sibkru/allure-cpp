#include "TestProgramStartEventHandler.h"

#include "Model/TestProgram.h"


namespace allure { namespace service {

	TestProgramStartEventHandler::TestProgramStartEventHandler(model::TestProgram& testProgram)
		:m_testProgram(testProgram)
	{
	}

	TestProgramStartEventHandler::~TestProgramStartEventHandler() = default;

	void TestProgramStartEventHandler::handleTestProgramStart() const
	{
		m_testProgram.clearTestSuites();
	}

}} // namespace allure::service
