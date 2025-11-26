#include "TimeService.h"

#include <chrono>


namespace allure { namespace service {

	TimeService::TimeService()
	{
	}

	time_t TimeService::getCurrentTime() const
	{
		// Return time in milliseconds since epoch (Allure 2 requirement)
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return static_cast<time_t>(millis);
	}

}} // namespace allure::service
