#pragma once

#include <string>


namespace allure { namespace service {

	class ITestStepStartEventHandler
	{
	public:
		virtual ~ITestStepStartEventHandler() = default;

		virtual void handleTestStepStart(const std::string& testStepDescription, bool isAction) const = 0;
	};

}} // namespace allure::service
