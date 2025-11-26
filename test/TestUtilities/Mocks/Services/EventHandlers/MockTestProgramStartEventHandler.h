#pragma once

#include "Services/EventHandlers/ITestProgramStartEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestProgramStartEventHandler : public allure::service::ITestProgramStartEventHandler
	{
	public:
		MockTestProgramStartEventHandler();
		virtual ~MockTestProgramStartEventHandler();

		MOCK_CONST_METHOD0(handleTestProgramStart, void());
	};

}} // namespace allure::test_utility

