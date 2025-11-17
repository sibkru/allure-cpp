#pragma once

#include <string>


namespace allure_cpp { namespace service {

	class ITimeService
	{
	public:
		virtual ~ITimeService() = default;

		virtual time_t getCurrentTime() const = 0;
	};

}} // namespace allure_cpp::service
