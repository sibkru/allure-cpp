#pragma once

#include "Model/TestSuite.h"
#include "Services/Report/ITestSuiteJSONSerializer.h"


namespace allure { namespace test_utility {

	class MockTestSuiteJSONSerializer : public allure::service::ITestSuiteJSONSerializer
	{
	public:
		MockTestSuiteJSONSerializer();
		virtual ~MockTestSuiteJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure::model::TestSuite&));
	};

}} // namespace allure::test_utility

