#pragma once

#include "Services/EventHandlers/ITestProgramEndEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestProgramEndEventHandler : public allure::service::ITestProgramEndEventHandler
	{
	public:
		MockTestProgramEndEventHandler();
		virtual ~MockTestProgramEndEventHandler();

		MOCK_CONST_METHOD0(handleTestProgramEnd, void());
	};

}} // namespace allure::test_utility

