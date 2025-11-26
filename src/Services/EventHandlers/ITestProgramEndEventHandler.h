#pragma once


namespace allure { namespace service {

	class ITestProgramEndEventHandler
	{
	public:
		virtual ~ITestProgramEndEventHandler() = default;

		virtual void handleTestProgramEnd() const = 0;
	};

}} // namespace allure::service
