#pragma once


namespace allure_cpp { namespace service {

	class ITestProgramStartEventHandler
	{
	public:
		virtual ~ITestProgramStartEventHandler() = default;

		virtual void handleTestProgramStart() const = 0;
	};

}} // namespace allure_cpp::service
