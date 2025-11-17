#pragma once

#include "Model/Status.h"

#include <string>
#include <stdexcept>


namespace allure_cpp { namespace service {

	class ITestCaseEndEventHandler
	{
	public:
		virtual ~ITestCaseEndEventHandler() = default;

		virtual void handleTestCaseEnd(model::Status) const = 0;

		// Overload for capturing failure details
		virtual void handleTestCaseEnd(model::Status status,
		                                const std::string& statusMessage,
		                                const std::string& statusTrace) const
		{
			// Default implementation ignores message/trace for backward compatibility
			handleTestCaseEnd(status);
		}

	public:
		struct Exception : std::runtime_error
		{
			Exception(const std::string& message)
				:std::runtime_error(message)
			{}
		};

		struct NoRunningTestSuiteException : Exception
		{
			NoRunningTestSuiteException()
				:Exception("No running test suite found when handling event for test case end")
			{}
		};

		struct NoRunningTestCaseException : Exception
		{
			NoRunningTestCaseException()
				:Exception("No running test case found when handling event for test case end")
			{}
		};
	};

}} // namespace allure_cpp::service
