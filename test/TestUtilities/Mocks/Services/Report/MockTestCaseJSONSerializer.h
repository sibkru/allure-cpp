#pragma once

#include "GTestAllureUtilities/Model/TestCase.h"
#include "GTestAllureUtilities/Services/Report/ITestCaseJSONSerializer.h"


namespace allure_cpp { namespace test_utility {

	class MockTestCaseJSONSerializer : public allure_cpp::service::ITestCaseJSONSerializer
	{
	public:
		MockTestCaseJSONSerializer();
		virtual ~MockTestCaseJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure_cpp::model::TestCase&));
	};

}} // namespace allure_cpp::test_utility
