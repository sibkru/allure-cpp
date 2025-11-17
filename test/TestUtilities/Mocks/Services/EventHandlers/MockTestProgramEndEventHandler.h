#pragma once

#include "GTestAllureUtilities/Services/EventHandlers/ITestProgramEndEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestProgramEndEventHandler : public allure_cpp::service::ITestProgramEndEventHandler
	{
	public:
		MockTestProgramEndEventHandler();
		virtual ~MockTestProgramEndEventHandler();

		MOCK_CONST_METHOD0(handleTestProgramEnd, void());
	};

}} // namespace allure_cpp::test_utility

