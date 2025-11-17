#pragma once


namespace allure_cpp { namespace model {

	enum class Stage
	{
		PENDING = 0,
		SCHEDULED = 1,
		RUNNING = 2,
		FINISHED = 3,
		INTERRUPTED = 4
	};

}} // namespace allure_cpp::model
