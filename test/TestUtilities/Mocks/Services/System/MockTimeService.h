#pragma once

#include "Services/System/ITimeService.h"


namespace allure { namespace test_utility {

	class MockTimeService:  public allure::service::ITimeService
	{
	public:
		MockTimeService();
		virtual ~MockTimeService();

		MOCK_CONST_METHOD0(getCurrentTime, time_t());
	};

}} // namespace allure::test_utility

