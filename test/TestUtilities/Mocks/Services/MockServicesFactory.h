#pragma once

#include "Services/IServicesFactory.h"


namespace allure { namespace test_utility {

	class MockServicesFactory : public allure::service::IServicesFactory
	{
	public:
		MockServicesFactory();
		virtual ~MockServicesFactory();

		// GTest services
		std::unique_ptr<::testing::TestEventListener> buildGTestEventListener() const;
		MOCK_CONST_METHOD0(buildGTestEventListenerProxy, ::testing::TestEventListener*());

		std::unique_ptr<allure::service::IGTestStatusChecker> buildGTestStatusChecker() const;
		MOCK_CONST_METHOD0(buildGTestStatusCheckerProxy, allure::service::IGTestStatusChecker* ());


		// Lifecycle events handling services
		std::unique_ptr<allure::service::ITestProgramStartEventHandler> buildTestProgramStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestProgramStartEventHandlerProxy, allure::service::ITestProgramStartEventHandler*());

		std::unique_ptr<allure::service::ITestSuiteStartEventHandler> buildTestSuiteStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestSuiteStartEventHandlerProxy, allure::service::ITestSuiteStartEventHandler* ());

		std::unique_ptr<allure::service::ITestCaseStartEventHandler> buildTestCaseStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestCaseStartEventHandlerProxy, allure::service::ITestCaseStartEventHandler*());

		std::unique_ptr<allure::service::ITestStepStartEventHandler> buildTestStepStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestStepStartEventHandlerProxy, allure::service::ITestStepStartEventHandler* ());

		std::unique_ptr<allure::service::ITestStepEndEventHandler> buildTestStepEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestStepEndEventHandlerProxy, allure::service::ITestStepEndEventHandler* ());

		std::unique_ptr<allure::service::ITestCaseEndEventHandler> buildTestCaseEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestCaseEndEventHandlerProxy, allure::service::ITestCaseEndEventHandler*());

		std::unique_ptr<allure::service::ITestSuiteEndEventHandler> buildTestSuiteEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestSuiteEndEventHandlerProxy, allure::service::ITestSuiteEndEventHandler* ());

		std::unique_ptr<allure::service::ITestProgramEndEventHandler> buildTestProgramEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestProgramEndEventHandlerProxy, allure::service::ITestProgramEndEventHandler*());


		// Property services
		std::unique_ptr<allure::service::ITestSuitePropertySetter> buildTestSuitePropertySetter() const;
		MOCK_CONST_METHOD0(buildTestSuitePropertySetterProxy, allure::service::ITestSuitePropertySetter*());

		std::unique_ptr<allure::service::ITestCasePropertySetter> buildTestCasePropertySetter() const;
		MOCK_CONST_METHOD0(buildTestCasePropertySetterProxy, allure::service::ITestCasePropertySetter* ());


		// JSON services
		std::unique_ptr<allure::service::ITestProgramJSONBuilder> buildTestProgramJSONBuilder() const;
		MOCK_CONST_METHOD0(buildTestProgramJSONBuilderProxy, allure::service::ITestProgramJSONBuilder*());

		std::unique_ptr<allure::service::ITestSuiteJSONSerializer> buildTestSuiteJSONSerializer() const;
		MOCK_CONST_METHOD0(buildTestSuiteJSONSerializerProxy, allure::service::ITestSuiteJSONSerializer*());


		// System services
		std::unique_ptr<allure::service::IUUIDGeneratorService> buildUUIDGeneratorService() const;
		MOCK_CONST_METHOD0(buildUUIDGeneratorServiceProxy, allure::service::IUUIDGeneratorService*());

		std::unique_ptr<allure::service::IFileService> buildFileService() const;
		MOCK_CONST_METHOD0(buildFileServiceProxy, allure::service::IFileService*());

		std::unique_ptr<allure::service::ITimeService> buildTimeService() const;
		MOCK_CONST_METHOD0(buildTimeServiceProxy, allure::service::ITimeService*());
	};

}} // namespace allure::test_utility

