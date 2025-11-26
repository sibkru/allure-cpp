#pragma once

#include "ITimeService.h"


namespace allure { namespace service {

	class TimeService : public ITimeService
	{
	public:
		TimeService();
		virtual ~TimeService() = default;

		time_t getCurrentTime() const;
	};

}} // namespace allure::service
