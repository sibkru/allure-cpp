#pragma once

#include "Model/Status.h"

#include <string>
#include <stdexcept>


namespace allure_cpp { namespace service {

	class ITestSuiteEndEventHandler
	{
	public:
		virtual ~ITestSuiteEndEventHandler() = default;

		virtual void handleTestSuiteEnd(model::Status) const = 0;

	public:
		struct NoRunningTestSuiteException : std::runtime_error
		{
			NoRunningTestSuiteException()
				:std::runtime_error("No running test suite found when handling event for test suite end")
			{}
		};
	};

}} // namespace allure_cpp::service
