#pragma once

#include <string>


namespace allure_cpp { namespace service {

	class ITestSuiteStartEventHandler
	{
	public:
		virtual ~ITestSuiteStartEventHandler() = default;

		virtual void handleTestSuiteStart(const std::string& testSuiteName) const = 0;
	};

}} // namespace allure_cpp::service
