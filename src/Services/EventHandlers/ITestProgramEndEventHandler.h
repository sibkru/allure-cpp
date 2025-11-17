#pragma once


namespace allure_cpp { namespace service {

	class ITestProgramEndEventHandler
	{
	public:
		virtual ~ITestProgramEndEventHandler() = default;

		virtual void handleTestProgramEnd() const = 0;
	};

}} // namespace allure_cpp::service
