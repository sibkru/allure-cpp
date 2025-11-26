#pragma once

#include <string>


namespace allure { namespace service {

	class ITestSuiteStartEventHandler
	{
	public:
		virtual ~ITestSuiteStartEventHandler() = default;

		virtual void handleTestSuiteStart(const std::string& testSuiteName) const = 0;
	};

}} // namespace allure::service
