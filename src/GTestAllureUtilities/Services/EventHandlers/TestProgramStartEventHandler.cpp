#include "TestProgramStartEventHandler.h"

#include "Model/TestProgram.h"


namespace allure_cpp { namespace service {

	TestProgramStartEventHandler::TestProgramStartEventHandler(model::TestProgram& testProgram)
		:m_testProgram(testProgram)
	{
	}

	TestProgramStartEventHandler::~TestProgramStartEventHandler() = default;

	void TestProgramStartEventHandler::handleTestProgramStart() const
	{
		m_testProgram.clearTestSuites();
	}

}} // namespace allure_cpp::service
