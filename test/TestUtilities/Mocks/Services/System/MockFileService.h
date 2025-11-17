#pragma once

#include "Services/System/IFileService.h"


namespace allure_cpp { namespace test_utility {

	class MockFileService:  public allure_cpp::service::IFileService
	{
	public:
		MockFileService();
		virtual ~MockFileService();

		MOCK_CONST_METHOD2(saveFile, void(const std::string&, const std::string&));
	};

}} // namespace allure_cpp::test_utility

