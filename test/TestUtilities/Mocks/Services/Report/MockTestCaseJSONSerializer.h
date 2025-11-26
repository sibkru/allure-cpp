#pragma once

#include "Model/TestCase.h"
#include "Services/Report/ITestCaseJSONSerializer.h"


namespace allure { namespace test_utility {

	class MockTestCaseJSONSerializer : public allure::service::ITestCaseJSONSerializer
	{
	public:
		MockTestCaseJSONSerializer();
		virtual ~MockTestCaseJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure::model::TestCase&));
	};

}} // namespace allure::test_utility
