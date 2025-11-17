#pragma once

#include "Model/TestSuite.h"
#include "Services/Report/ITestSuiteJSONSerializer.h"


namespace allure_cpp { namespace test_utility {

	class MockTestSuiteJSONSerializer : public allure_cpp::service::ITestSuiteJSONSerializer
	{
	public:
		MockTestSuiteJSONSerializer();
		virtual ~MockTestSuiteJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure_cpp::model::TestSuite&));
	};

}} // namespace allure_cpp::test_utility

