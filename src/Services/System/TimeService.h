#pragma once

#include "ITimeService.h"


namespace allure_cpp { namespace service {

	class TimeService : public ITimeService
	{
	public:
		TimeService();
		virtual ~TimeService() = default;

		time_t getCurrentTime() const;
	};

}} // namespace allure_cpp::service
