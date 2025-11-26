#pragma once

#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestSuiteEndEventHandler : public allure::service::ITestSuiteEndEventHandler
	{
	public:
		MockTestSuiteEndEventHandler();
		virtual ~MockTestSuiteEndEventHandler();

		MOCK_CONST_METHOD1(handleTestSuiteEnd, void(allure::model::Status));
	};

}} // namespace allure::test_utility

