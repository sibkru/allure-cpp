#pragma once

#include "Model/Status.h"

#include <string>
#include <stdexcept>


namespace allure { namespace service {

	class IGTestStatusChecker
	{
	public:
		virtual ~IGTestStatusChecker() = default;

		virtual model::Status getCurrentTestStatus() const = 0;
	};

}} // namespace allure::service
