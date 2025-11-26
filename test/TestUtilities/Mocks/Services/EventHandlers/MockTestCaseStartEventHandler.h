#pragma once

#include "Services/EventHandlers/ITestCaseStartEventHandler.h"


namespace allure { namespace test_utility {

	class MockTestCaseStartEventHandler : public allure::service::ITestCaseStartEventHandler
	{
	public:
		MockTestCaseStartEventHandler();
		virtual ~MockTestCaseStartEventHandler();

		MOCK_CONST_METHOD1(handleTestCaseStart, void(const std::string&));
		MOCK_CONST_METHOD1(handleTestCaseStart, void(const allure::ITestMetadata&));
	};

}} // namespace allure::test_utility

