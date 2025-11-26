#pragma once


namespace allure { namespace service {

	class ITestProgramStartEventHandler
	{
	public:
		virtual ~ITestProgramStartEventHandler() = default;

		virtual void handleTestProgramStart() const = 0;
	};

}} // namespace allure::service
