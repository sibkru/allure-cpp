#include "GTestStatusChecker.h"

#include <gtest/gtest.h>


namespace allure_cpp { namespace service {

	GTestStatusChecker::GTestStatusChecker() = default;
	GTestStatusChecker::~GTestStatusChecker() = default;

	model::Status GTestStatusChecker::getCurrentTestStatus() const
	{
		bool isSkipped = ::testing::Test::IsSkipped();
		if (isSkipped)
		{
			return model::Status::SKIPPED;
		}
		
		bool hasFailed = ::testing::Test::HasFailure();
		if (hasFailed)
		{
			return model::Status::FAILED;
		}

		return model::Status::PASSED;
	}

}} // namespace allure_cpp::service
