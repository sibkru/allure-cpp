#pragma once

#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestSuiteStartEventHandler : public allure::service::ITestSuiteStartEventHandler
	{
	public:
		MockTestSuiteStartEventHandler();
		virtual ~MockTestSuiteStartEventHandler();

		MOCK_CONST_METHOD1(handleTestSuiteStart, void(const std::string&));
	};

}} // namespace allure::test_utility

