#include "stdafx.h"
#include "MockServicesFactory.h"

#include "Services/EventHandlers/ITestCaseEndEventHandler.h"
#include "Services/EventHandlers/ITestCaseStartEventHandler.h"
#include "Services/EventHandlers/ITestProgramEndEventHandler.h"
#include "Services/EventHandlers/ITestProgramStartEventHandler.h"
#include "Services/EventHandlers/ITestStepEndEventHandler.h"
#include "Services/EventHandlers/ITestStepStartEventHandler.h"
#include "Services/EventHandlers/ITestSuiteEndEventHandler.h"
#include "Services/EventHandlers/ITestSuiteStartEventHandler.h"
#include "Services/GoogleTest/IGTestStatusChecker.h"
#include "Services/Property/ITestCasePropertySetter.h"
#include "Services/Property/ITestSuitePropertySetter.h"
#include "Services/Report/ITestSuiteJSONSerializer.h"
#include "Services/Report/ITestProgramJSONBuilder.h"
#include "Services/System/IFileService.h"
#include "Services/System/ITimeService.h"
#include "Services/System/IUUIDGeneratorService.h"


namespace allure_cpp { namespace test_utility {

	MockServicesFactory::MockServicesFactory() = default;
	MockServicesFactory::~MockServicesFactory() = default;


	// GTest services
	std::unique_ptr<::testing::TestEventListener> MockServicesFactory::buildGTestEventListener() const
	{
		return std::unique_ptr<::testing::TestEventListener>(buildGTestEventListenerProxy());
	}

	std::unique_ptr<allure_cpp::service::IGTestStatusChecker> MockServicesFactory::buildGTestStatusChecker() const
	{
		return std::unique_ptr<allure_cpp::service::IGTestStatusChecker>(buildGTestStatusCheckerProxy());
	}


	// Lifecycle events handling services
	std::unique_ptr<allure_cpp::service::ITestProgramStartEventHandler> MockServicesFactory::buildTestProgramStartEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestProgramStartEventHandler>(buildTestProgramStartEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestSuiteStartEventHandler> MockServicesFactory::buildTestSuiteStartEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestSuiteStartEventHandler>(buildTestSuiteStartEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestCaseStartEventHandler> MockServicesFactory::buildTestCaseStartEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestCaseStartEventHandler>(buildTestCaseStartEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestStepStartEventHandler> MockServicesFactory::buildTestStepStartEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestStepStartEventHandler>(buildTestStepStartEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestStepEndEventHandler> MockServicesFactory::buildTestStepEndEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestStepEndEventHandler>(buildTestStepEndEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestCaseEndEventHandler> MockServicesFactory::buildTestCaseEndEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestCaseEndEventHandler>(buildTestCaseEndEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestSuiteEndEventHandler> MockServicesFactory::buildTestSuiteEndEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestSuiteEndEventHandler>(buildTestSuiteEndEventHandlerProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestProgramEndEventHandler> MockServicesFactory::buildTestProgramEndEventHandler() const
	{
		return std::unique_ptr<allure_cpp::service::ITestProgramEndEventHandler>(buildTestProgramEndEventHandlerProxy());
	}


	// Property services
	std::unique_ptr<allure_cpp::service::ITestSuitePropertySetter> MockServicesFactory::buildTestSuitePropertySetter() const
	{
		return std::unique_ptr<allure_cpp::service::ITestSuitePropertySetter>(buildTestSuitePropertySetterProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestCasePropertySetter> MockServicesFactory::buildTestCasePropertySetter() const
	{
		return std::unique_ptr<allure_cpp::service::ITestCasePropertySetter>(buildTestCasePropertySetterProxy());
	}


	// JSON services
	std::unique_ptr<allure_cpp::service::ITestProgramJSONBuilder> MockServicesFactory::buildTestProgramJSONBuilder() const
	{
		return std::unique_ptr<allure_cpp::service::ITestProgramJSONBuilder>(buildTestProgramJSONBuilderProxy());
	}

	std::unique_ptr<allure_cpp::service::ITestSuiteJSONSerializer> MockServicesFactory::buildTestSuiteJSONSerializer() const
	{
		return std::unique_ptr<allure_cpp::service::ITestSuiteJSONSerializer>(buildTestSuiteJSONSerializerProxy());
	}


	// System services
	std::unique_ptr<allure_cpp::service::IUUIDGeneratorService> MockServicesFactory::buildUUIDGeneratorService() const
	{
		return std::unique_ptr<allure_cpp::service::IUUIDGeneratorService>(buildUUIDGeneratorServiceProxy());
	}

	std::unique_ptr<allure_cpp::service::IFileService> MockServicesFactory::buildFileService() const
	{
		return std::unique_ptr<allure_cpp::service::IFileService>(buildFileServiceProxy());
	}

	std::unique_ptr<allure_cpp::service::ITimeService> MockServicesFactory::buildTimeService() const
	{
		return std::unique_ptr<allure_cpp::service::ITimeService>(buildTimeServiceProxy());
	}

}} // namespace allure_cpp::test_utility

