#pragma once

#include "Model/TestProgram.h"
#include "Services/Report/ITestProgramJSONBuilder.h"


namespace allure_cpp { namespace test_utility {

	class MockTestProgramJSONBuilder : public allure_cpp::service::ITestProgramJSONBuilder
	{
	public:
		MockTestProgramJSONBuilder();
		virtual ~MockTestProgramJSONBuilder();

		MOCK_CONST_METHOD1(buildJSONFiles, void(const allure_cpp::model::TestProgram&));
	};

}} // namespace allure_cpp::test_utility

