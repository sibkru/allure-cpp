#pragma once

#include "GTestAllureUtilities/Model/TestProgram.h"
#include "TestUtilities/Mocks/Services/MockServicesFactory.h"


namespace allure_cpp { namespace service {
	class ITestCaseJSONSerializer;
	class IContainerJSONSerializer;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace test_utility {

	class StubServicesFactory : public MockServicesFactory
	{
	public:
		StubServicesFactory(allure_cpp::model::TestProgram&);
		virtual ~StubServicesFactory();

		// GTest services
		::testing::TestEventListener* buildGTestEventListenerStub() const;
		allure_cpp::service::IGTestStatusChecker* buildGTestStatusCheckerStub() const;

		// Lifecycle events handling services
		allure_cpp::service::ITestProgramStartEventHandler* buildTestProgramStartEventHandlerStub() const;
		allure_cpp::service::ITestSuiteStartEventHandler* buildTestSuiteStartEventHandlerStub() const;
		allure_cpp::service::ITestCaseStartEventHandler* buildTestCaseStartEventHandlerStub() const;
		allure_cpp::service::ITestStepStartEventHandler* buildTestStepStartEventHandlerStub() const;
		allure_cpp::service::ITestStepEndEventHandler* buildTestStepEndEventHandlerStub() const;
		allure_cpp::service::ITestCaseEndEventHandler* buildTestCaseEndEventHandlerStub() const;
		allure_cpp::service::ITestSuiteEndEventHandler* buildTestSuiteEndEventHandlerStub() const;
		allure_cpp::service::ITestProgramEndEventHandler* buildTestProgramEndEventHandlerStub() const;

		// Property services
		allure_cpp::service::ITestSuitePropertySetter* buildTestSuitePropertySetterStub() const;
		allure_cpp::service::ITestCasePropertySetter* buildTestCasePropertySetterStub() const;

		// Report services
		allure_cpp::service::ITestProgramJSONBuilder* buildTestProgramJSONBuilderStub() const;
		allure_cpp::service::ITestCaseJSONSerializer* buildTestCaseJSONSerializerStub() const;
		allure_cpp::service::IContainerJSONSerializer* buildContainerJSONSerializerStub() const;
		allure_cpp::service::ITestSuiteJSONSerializer* buildTestSuiteJSONSerializerStub() const;

		// System services
		allure_cpp::service::IUUIDGeneratorService* buildUUIDGeneratorServiceStub() const;
		allure_cpp::service::IFileService* buildFileServiceStub() const;
		allure_cpp::service::ITimeService* buildTimeServiceStub() const;

	private:
		allure_cpp::model::TestProgram& m_testProgram;
	};

}} // namespace allure_cpp::test_utility

