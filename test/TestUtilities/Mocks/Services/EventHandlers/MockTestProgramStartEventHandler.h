#pragma once

#include "Services/EventHandlers/ITestProgramStartEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestProgramStartEventHandler : public allure_cpp::service::ITestProgramStartEventHandler
	{
	public:
		MockTestProgramStartEventHandler();
		virtual ~MockTestProgramStartEventHandler();

		MOCK_CONST_METHOD0(handleTestProgramStart, void());
	};

}} // namespace allure_cpp::test_utility

