#pragma once

#include "IGTestStatusChecker.h"


namespace allure { namespace service {

	class GTestStatusChecker : public IGTestStatusChecker
	{
	public:
		GTestStatusChecker();
		virtual ~GTestStatusChecker();

		model::Status getCurrentTestStatus() const override;
	};

}} // namespace allure::service
