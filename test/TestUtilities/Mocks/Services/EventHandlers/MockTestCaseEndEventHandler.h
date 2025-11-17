#pragma once

#include "GTestAllureUtilities/Services/EventHandlers/ITestCaseEndEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestCaseEndEventHandler : public allure_cpp::service::ITestCaseEndEventHandler
	{
	public:
		MockTestCaseEndEventHandler();
		virtual ~MockTestCaseEndEventHandler();

		MOCK_CONST_METHOD1(handleTestCaseEnd, void(allure_cpp::model::Status));
	};

}} // namespace allure_cpp::test_utility

