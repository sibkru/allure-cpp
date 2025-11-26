#pragma once

#include <string>


namespace allure { namespace service {

	class ITimeService
	{
	public:
		virtual ~ITimeService() = default;

		virtual time_t getCurrentTime() const = 0;
	};

}} // namespace allure::service
