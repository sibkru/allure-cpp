#pragma once

#include "Services/System/IUUIDGeneratorService.h"


namespace allure { namespace test_utility {

	class MockUUIDGeneratorService:  public allure::service::IUUIDGeneratorService
	{
	public:
		MockUUIDGeneratorService();
		virtual ~MockUUIDGeneratorService();

		MOCK_CONST_METHOD0(generateUUID, std::string());
	};

}} // namespace allure::test_utility

