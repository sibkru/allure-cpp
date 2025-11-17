#pragma once


namespace allure_cpp { namespace model {

	enum class Status
	{
		UNKNOWN = 0,
		PASSED = 1,
		FAILED = 2,
		BROKEN = 3,
		SKIPPED = 4
	};

}} // namespace allure_cpp::model
