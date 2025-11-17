#pragma once

#include "GTestAllureUtilities/Services/System/IUUIDGeneratorService.h"


namespace allure_cpp { namespace test_utility {

	class MockUUIDGeneratorService:  public allure_cpp::service::IUUIDGeneratorService
	{
	public:
		MockUUIDGeneratorService();
		virtual ~MockUUIDGeneratorService();

		MOCK_CONST_METHOD0(generateUUID, std::string());
	};

}} // namespace allure_cpp::test_utility

