#pragma once

#include "GTestAllureUtilities/Services/EventHandlers/ITestSuiteEndEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestSuiteEndEventHandler : public allure_cpp::service::ITestSuiteEndEventHandler
	{
	public:
		MockTestSuiteEndEventHandler();
		virtual ~MockTestSuiteEndEventHandler();

		MOCK_CONST_METHOD1(handleTestSuiteEnd, void(allure_cpp::model::Status));
	};

}} // namespace allure_cpp::test_utility

