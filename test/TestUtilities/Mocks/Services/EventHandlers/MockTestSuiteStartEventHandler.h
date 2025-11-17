#pragma once

#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestSuiteStartEventHandler : public allure_cpp::service::ITestSuiteStartEventHandler
	{
	public:
		MockTestSuiteStartEventHandler();
		virtual ~MockTestSuiteStartEventHandler();

		MOCK_CONST_METHOD1(handleTestSuiteStart, void(const std::string&));
	};

}} // namespace allure_cpp::test_utility

