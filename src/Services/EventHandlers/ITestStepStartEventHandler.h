#pragma once

#include <string>


namespace allure_cpp { namespace service {

	class ITestStepStartEventHandler
	{
	public:
		virtual ~ITestStepStartEventHandler() = default;

		virtual void handleTestStepStart(const std::string& testStepDescription, bool isAction) const = 0;
	};

}} // namespace allure_cpp::service
