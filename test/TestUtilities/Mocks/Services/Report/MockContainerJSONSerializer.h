#pragma once

#include "Model/Container.h"
#include "Services/Report/IContainerJSONSerializer.h"


namespace allure { namespace test_utility {

	class MockContainerJSONSerializer : public allure::service::IContainerJSONSerializer
	{
	public:
		MockContainerJSONSerializer();
		virtual ~MockContainerJSONSerializer();

		MOCK_CONST_METHOD1(serialize, std::string(const allure::model::Container&));
	};

}} // namespace allure::test_utility
