#pragma once

#include "Framework/ITestMetadata.h"
#include <string>


namespace allure { namespace service {

	class ITestCaseStartEventHandler
	{
	public:
		virtual ~ITestCaseStartEventHandler() = default;

		virtual void handleTestCaseStart(const std::string& testCaseName) const = 0;
		virtual void handleTestCaseStart(const ITestMetadata& metadata) const = 0;
	};

}} // namespace allure::service
