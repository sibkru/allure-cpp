#pragma once

#include "Services/EventHandlers/ITestCaseEndEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestCaseEndEventHandler : public allure::service::ITestCaseEndEventHandler
	{
	public:
		MockTestCaseEndEventHandler();
		virtual ~MockTestCaseEndEventHandler();

		MOCK_CONST_METHOD1(handleTestCaseEnd, void(allure::model::Status));
	};

}} // namespace allure::test_utility

