#pragma once

#include "Services/System/ITimeService.h"


namespace allure_cpp { namespace test_utility {

	class MockTimeService:  public allure_cpp::service::ITimeService
	{
	public:
		MockTimeService();
		virtual ~MockTimeService();

		MOCK_CONST_METHOD0(getCurrentTime, time_t());
	};

}} // namespace allure_cpp::test_utility

