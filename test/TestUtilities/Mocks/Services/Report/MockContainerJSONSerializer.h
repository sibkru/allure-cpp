#pragma once

#include "Model/Container.h"
#include "Services/Report/IContainerJSONSerializer.h"


namespace allure_cpp { namespace test_utility {

	class MockContainerJSONSerializer : public allure_cpp::service::IContainerJSONSerializer
	{
	public:
		MockContainerJSONSerializer();
		virtual ~MockContainerJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure_cpp::model::Container&));
	};

}} // namespace allure_cpp::test_utility
