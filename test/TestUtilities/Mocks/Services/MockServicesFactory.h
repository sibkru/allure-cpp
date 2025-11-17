#pragma once

#include "Services/IServicesFactory.h"


namespace allure_cpp { namespace test_utility {

	class MockServicesFactory : public allure_cpp::service::IServicesFactory
	{
	public:
		MockServicesFactory();
		virtual ~MockServicesFactory();

		// GTest services
		std::unique_ptr<::testing::TestEventListener> buildGTestEventListener() const;
		MOCK_CONST_METHOD0(buildGTestEventListenerProxy, ::testing::TestEventListener*());

		std::unique_ptr<allure_cpp::service::IGTestStatusChecker> buildGTestStatusChecker() const;
		MOCK_CONST_METHOD0(buildGTestStatusCheckerProxy, allure_cpp::service::IGTestStatusChecker* ());


		// Lifecycle events handling services
		std::unique_ptr<allure_cpp::service::ITestProgramStartEventHandler> buildTestProgramStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestProgramStartEventHandlerProxy, allure_cpp::service::ITestProgramStartEventHandler*());

		std::unique_ptr<allure_cpp::service::ITestSuiteStartEventHandler> buildTestSuiteStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestSuiteStartEventHandlerProxy, allure_cpp::service::ITestSuiteStartEventHandler* ());

		std::unique_ptr<allure_cpp::service::ITestCaseStartEventHandler> buildTestCaseStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestCaseStartEventHandlerProxy, allure_cpp::service::ITestCaseStartEventHandler*());

		std::unique_ptr<allure_cpp::service::ITestStepStartEventHandler> buildTestStepStartEventHandler() const;
		MOCK_CONST_METHOD0(buildTestStepStartEventHandlerProxy, allure_cpp::service::ITestStepStartEventHandler* ());

		std::unique_ptr<allure_cpp::service::ITestStepEndEventHandler> buildTestStepEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestStepEndEventHandlerProxy, allure_cpp::service::ITestStepEndEventHandler* ());

		std::unique_ptr<allure_cpp::service::ITestCaseEndEventHandler> buildTestCaseEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestCaseEndEventHandlerProxy, allure_cpp::service::ITestCaseEndEventHandler*());

		std::unique_ptr<allure_cpp::service::ITestSuiteEndEventHandler> buildTestSuiteEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestSuiteEndEventHandlerProxy, allure_cpp::service::ITestSuiteEndEventHandler* ());

		std::unique_ptr<allure_cpp::service::ITestProgramEndEventHandler> buildTestProgramEndEventHandler() const;
		MOCK_CONST_METHOD0(buildTestProgramEndEventHandlerProxy, allure_cpp::service::ITestProgramEndEventHandler*());


		// Property services
		std::unique_ptr<allure_cpp::service::ITestSuitePropertySetter> buildTestSuitePropertySetter() const;
		MOCK_CONST_METHOD0(buildTestSuitePropertySetterProxy, allure_cpp::service::ITestSuitePropertySetter*());

		std::unique_ptr<allure_cpp::service::ITestCasePropertySetter> buildTestCasePropertySetter() const;
		MOCK_CONST_METHOD0(buildTestCasePropertySetterProxy, allure_cpp::service::ITestCasePropertySetter* ());


		// JSON services
		std::unique_ptr<allure_cpp::service::ITestProgramJSONBuilder> buildTestProgramJSONBuilder() const;
		MOCK_CONST_METHOD0(buildTestProgramJSONBuilderProxy, allure_cpp::service::ITestProgramJSONBuilder*());

		std::unique_ptr<allure_cpp::service::ITestSuiteJSONSerializer> buildTestSuiteJSONSerializer() const;
		MOCK_CONST_METHOD0(buildTestSuiteJSONSerializerProxy, allure_cpp::service::ITestSuiteJSONSerializer*());


		// System services
		std::unique_ptr<allure_cpp::service::IUUIDGeneratorService> buildUUIDGeneratorService() const;
		MOCK_CONST_METHOD0(buildUUIDGeneratorServiceProxy, allure_cpp::service::IUUIDGeneratorService*());

		std::unique_ptr<allure_cpp::service::IFileService> buildFileService() const;
		MOCK_CONST_METHOD0(buildFileServiceProxy, allure_cpp::service::IFileService*());

		std::unique_ptr<allure_cpp::service::ITimeService> buildTimeService() const;
		MOCK_CONST_METHOD0(buildTimeServiceProxy, allure_cpp::service::ITimeService*());
	};

}} // namespace allure_cpp::test_utility

