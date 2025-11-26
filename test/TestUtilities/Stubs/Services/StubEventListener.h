#pragma once

#include "Model/Status.h"
#include "Services/IServicesFactory.h"
#include "Services/EventHandlers/ITestCaseEndEventHandler.h"
#include "Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"


namespace allure { namespace test_utility {

	class StubEventListener
	{
	public:
		StubEventListener(allure::service::IServicesFactory&);
		virtual ~StubEventListener() = default;

		void onProgramStart() const;
		void onTestSuiteStart(const std::string& testSuiteName) const;
		void onTestStart(const std::string& testCaseName) const;
		void onTestEnd(allure::model::Status) const;
		void onTestSuiteEnd(allure::model::Status) const;
		void onProgramEnd() const;

	private:
		allure::service::IServicesFactory& m_servicesFactory;
		std::unique_ptr<allure::service::ITestProgramStartEventHandler> m_testProgramStartEventHandler;
		std::unique_ptr<allure::service::ITestSuiteStartEventHandler> m_testSuiteStartEventHandler;
		std::unique_ptr<allure::service::ITestCaseStartEventHandler> m_testCaseStartEventHandler;
		std::unique_ptr<allure::service::ITestCaseEndEventHandler> m_testCaseEndEventHandler;
		std::unique_ptr<allure::service::ITestSuiteEndEventHandler> m_testSuiteEndEventHandler;
		std::unique_ptr<allure::service::ITestProgramEndEventHandler> m_testProgramEndEventHandler;
	};

}} // namespace allure::test_utility

