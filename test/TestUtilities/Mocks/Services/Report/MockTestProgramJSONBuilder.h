#pragma once

#include "Model/TestProgram.h"
#include "Services/Report/ITestProgramJSONBuilder.h"


namespace allure { namespace test_utility {

	class MockTestProgramJSONBuilder : public allure::service::ITestProgramJSONBuilder
	{
	public:
		MockTestProgramJSONBuilder();
		virtual ~MockTestProgramJSONBuilder();

		MOCK_CONST_METHOD1(buildJSONFiles, void(const allure::model::TestProgram&));
		MOCK_CONST_METHOD1(buildMetadataFiles, void(const allure::model::TestProgram&));
	};

}} // namespace allure::test_utility

