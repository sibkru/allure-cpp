#pragma once

#include "Model/TestProgram.h"
#include "TestUtilities/Mocks/Services/MockServicesFactory.h"


namespace allure { namespace service {
	class ITestCaseJSONSerializer;
	class IContainerJSONSerializer;
}} // namespace allure::service

namespace allure { namespace test_utility {

	class StubServicesFactory : public MockServicesFactory
	{
	public:
		StubServicesFactory(allure::model::TestProgram&);
		virtual ~StubServicesFactory();

		// GTest services
		::testing::TestEventListener* buildGTestEventListenerStub() const;
		allure::service::IGTestStatusChecker* buildGTestStatusCheckerStub() const;

		// Lifecycle events handling services
		allure::service::ITestProgramStartEventHandler* buildTestProgramStartEventHandlerStub() const;
		allure::service::ITestSuiteStartEventHandler* buildTestSuiteStartEventHandlerStub() const;
		allure::service::ITestCaseStartEventHandler* buildTestCaseStartEventHandlerStub() const;
		allure::service::ITestStepStartEventHandler* buildTestStepStartEventHandlerStub() const;
		allure::service::ITestStepEndEventHandler* buildTestStepEndEventHandlerStub() const;
		allure::service::ITestCaseEndEventHandler* buildTestCaseEndEventHandlerStub() const;
		allure::service::ITestSuiteEndEventHandler* buildTestSuiteEndEventHandlerStub() const;
		allure::service::ITestProgramEndEventHandler* buildTestProgramEndEventHandlerStub() const;

		// Property services
		allure::service::ITestSuitePropertySetter* buildTestSuitePropertySetterStub() const;
		allure::service::ITestCasePropertySetter* buildTestCasePropertySetterStub() const;

		// Report services
		allure::service::ITestProgramJSONBuilder* buildTestProgramJSONBuilderStub() const;
		allure::service::ITestCaseJSONSerializer* buildTestCaseJSONSerializerStub() const;
		allure::service::IContainerJSONSerializer* buildContainerJSONSerializerStub() const;
		allure::service::ITestSuiteJSONSerializer* buildTestSuiteJSONSerializerStub() const;

		// System services
		allure::service::IUUIDGeneratorService* buildUUIDGeneratorServiceStub() const;
		allure::service::IFileService* buildFileServiceStub() const;
		allure::service::ITimeService* buildTimeServiceStub() const;

	private:
		allure::model::TestProgram& m_testProgram;
	};

}} // namespace allure::test_utility

