#pragma once

#include "GTestAllureUtilities/Services/EventHandlers/ITestCaseStartEventHandler.h"


namespace allure_cpp { namespace test_utility {

	class MockTestCaseStartEventHandler : public allure_cpp::service::ITestCaseStartEventHandler
	{
	public:
		MockTestCaseStartEventHandler();
		virtual ~MockTestCaseStartEventHandler();

		MOCK_CONST_METHOD1(handleTestCaseStart, void(const std::string&));
		MOCK_CONST_METHOD1(handleTestCaseStart, void(const allure_cpp::ITestMetadata&));
	};

}} // namespace allure_cpp::test_utility

