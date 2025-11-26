#pragma once

#include "Services/System/IFileService.h"


namespace allure { namespace test_utility {

	class MockFileService:  public allure::service::IFileService
	{
	public:
		MockFileService();
		virtual ~MockFileService();

		MOCK_CONST_METHOD2(saveFile, void(const std::string&, const std::string&));
	};

}} // namespace allure::test_utility

