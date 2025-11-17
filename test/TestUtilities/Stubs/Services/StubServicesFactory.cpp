#include "stdafx.h"
#include "StubServicesFactory.h"

#include "GTestAllureUtilities/Model/TestProgram.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestCaseEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestCaseStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestProgramEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestProgramStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestStepEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestStepStartEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestSuiteEndEventHandler.h"
#include "GTestAllureUtilities/Services/EventHandlers/TestSuiteStartEventHandler.h"
#include "GTestAllureUtilities/Services/GoogleTest/GTestEventListener.h"
#include "GTestAllureUtilities/Services/GoogleTest/GTestStatusChecker.h"
#include "GTestAllureUtilities/Services/Property/TestCasePropertySetter.h"
#include "GTestAllureUtilities/Services/Property/TestSuitePropertySetter.h"
#include "GTestAllureUtilities/Services/Report/TestCaseJSONSerializer.h"
#include "GTestAllureUtilities/Services/Report/ContainerJSONSerializer.h"
#include "GTestAllureUtilities/Services/Report/TestSuiteJSONSerializer.h"
#include "GTestAllureUtilities/Services/Report/TestProgramJSONBuilder.h"
#include "GTestAllureUtilities/Services/System/FileService.h"
#include "GTestAllureUtilities/Services/System/TimeService.h"
#include "GTestAllureUtilities/Services/System/UUIDGeneratorService.h"

#include "RapidJSONAdapter/JSONAdapter.h"


using namespace testing;

namespace allure_cpp { namespace test_utility {

	StubServicesFactory::StubServicesFactory(allure_cpp::model::TestProgram& testProgram)
		:m_testProgram(testProgram)
	{
		ON_CALL(*this, buildGTestEventListenerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildGTestEventListenerStub));
		ON_CALL(*this, buildGTestStatusCheckerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildGTestStatusCheckerStub));

		ON_CALL(*this, buildTestProgramStartEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestProgramStartEventHandlerStub));
		ON_CALL(*this, buildTestSuiteStartEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestSuiteStartEventHandlerStub));
		ON_CALL(*this, buildTestCaseStartEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestCaseStartEventHandlerStub));
		ON_CALL(*this, buildTestStepStartEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestStepStartEventHandlerStub));
		ON_CALL(*this, buildTestStepEndEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestStepEndEventHandlerStub));
		ON_CALL(*this, buildTestCaseEndEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestCaseEndEventHandlerStub));
		ON_CALL(*this, buildTestSuiteEndEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestSuiteEndEventHandlerStub));
		ON_CALL(*this, buildTestProgramEndEventHandlerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestProgramEndEventHandlerStub));

		ON_CALL(*this, buildTestSuitePropertySetterProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestSuitePropertySetterStub));
		ON_CALL(*this, buildTestCasePropertySetterProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestCasePropertySetterStub));

		ON_CALL(*this, buildTestProgramJSONBuilderProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestProgramJSONBuilderStub));
		ON_CALL(*this, buildTestSuiteJSONSerializerProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTestSuiteJSONSerializerStub));

		ON_CALL(*this, buildUUIDGeneratorServiceProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildUUIDGeneratorServiceStub));
		ON_CALL(*this, buildFileServiceProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildFileServiceStub));
		ON_CALL(*this, buildTimeServiceProxy()).WillByDefault(Invoke(this, &StubServicesFactory::buildTimeServiceStub));
	}

	StubServicesFactory::~StubServicesFactory() = default;


	// GTest services
	::testing::TestEventListener* StubServicesFactory::buildGTestEventListenerStub() const
	{
		auto testProgramStartEventHandler = buildTestProgramStartEventHandler();
		auto testSuiteStartEventHandler = buildTestSuiteStartEventHandler();
		auto testCaseStartEventHandler = buildTestCaseStartEventHandler();
		auto testCaseEndEventHandler = buildTestCaseEndEventHandler();
		auto testSuiteEndEventHandler = buildTestSuiteEndEventHandler();
		auto testProgramEndEventHandler = buildTestProgramEndEventHandler();

		return new allure_cpp::service::GTestEventListener
						(std::move(testProgramStartEventHandler), std::move(testSuiteStartEventHandler),
						 std::move(testCaseStartEventHandler), std::move(testCaseEndEventHandler),
						 std::move(testSuiteEndEventHandler), std::move(testProgramEndEventHandler));
	}

	allure_cpp::service::IGTestStatusChecker* StubServicesFactory::buildGTestStatusCheckerStub() const
	{
		return new allure_cpp::service::GTestStatusChecker();
	}

	// Lifecycle events handling services
	allure_cpp::service::ITestProgramStartEventHandler* StubServicesFactory::buildTestProgramStartEventHandlerStub() const
	{
		return new allure_cpp::service::TestProgramStartEventHandler(m_testProgram);
	}

	allure_cpp::service::ITestSuiteStartEventHandler* StubServicesFactory::buildTestSuiteStartEventHandlerStub() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestSuiteStartEventHandler(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	allure_cpp::service::ITestCaseStartEventHandler* StubServicesFactory::buildTestCaseStartEventHandlerStub() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestCaseStartEventHandler(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	allure_cpp::service::ITestStepStartEventHandler* StubServicesFactory::buildTestStepStartEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestStepStartEventHandler(m_testProgram, std::move(timeService));
	}

	allure_cpp::service::ITestStepEndEventHandler* StubServicesFactory::buildTestStepEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestStepEndEventHandler(m_testProgram, std::move(timeService));
	}

	allure_cpp::service::ITestCaseEndEventHandler* StubServicesFactory::buildTestCaseEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestCaseEndEventHandler(m_testProgram, std::move(timeService));
	}

	allure_cpp::service::ITestSuiteEndEventHandler* StubServicesFactory::buildTestSuiteEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure_cpp::service::TestSuiteEndEventHandler(m_testProgram, std::move(timeService));
	}

	allure_cpp::service::ITestProgramEndEventHandler* StubServicesFactory::buildTestProgramEndEventHandlerStub() const
	{
		auto testProgramJSONBuilder = buildTestProgramJSONBuilder();
		return new allure_cpp::service::TestProgramEndEventHandler(m_testProgram, std::move(testProgramJSONBuilder));
	}


	// Property services
	allure_cpp::service::ITestSuitePropertySetter* StubServicesFactory::buildTestSuitePropertySetterStub() const
	{
		return new allure_cpp::service::TestSuitePropertySetter(m_testProgram);
	}

	allure_cpp::service::ITestCasePropertySetter* StubServicesFactory::buildTestCasePropertySetterStub() const
	{
		return new allure_cpp::service::TestCasePropertySetter(m_testProgram);
	}


	// Report services
	allure_cpp::service::ITestProgramJSONBuilder* StubServicesFactory::buildTestProgramJSONBuilderStub() const
	{
		auto jsonAdapter1 = std::make_unique<systelab::json::rapidjson::JSONAdapter>();
		std::unique_ptr<allure_cpp::service::ITestCaseJSONSerializer> testCaseJSONSerializer =
			std::make_unique<allure_cpp::service::TestCaseJSONSerializer>(std::move(jsonAdapter1));

		auto jsonAdapter2 = std::make_unique<systelab::json::rapidjson::JSONAdapter>();
		std::unique_ptr<allure_cpp::service::IContainerJSONSerializer> containerJSONSerializer =
			std::make_unique<allure_cpp::service::ContainerJSONSerializer>(std::move(jsonAdapter2));

		std::unique_ptr<allure_cpp::service::IFileService> fileService(buildFileServiceStub());

		return new allure_cpp::service::TestProgramJSONBuilder(std::move(testCaseJSONSerializer),
		                                                        std::move(containerJSONSerializer),
		                                                        std::move(fileService));
	}

	allure_cpp::service::ITestCaseJSONSerializer* StubServicesFactory::buildTestCaseJSONSerializerStub() const
	{
		auto jsonAdapter = std::make_unique<systelab::json::rapidjson::JSONAdapter>();
		return new allure_cpp::service::TestCaseJSONSerializer(std::move(jsonAdapter));
	}

	allure_cpp::service::IContainerJSONSerializer* StubServicesFactory::buildContainerJSONSerializerStub() const
	{
		auto jsonAdapter = std::make_unique<systelab::json::rapidjson::JSONAdapter>();
		return new allure_cpp::service::ContainerJSONSerializer(std::move(jsonAdapter));
	}

	allure_cpp::service::ITestSuiteJSONSerializer* StubServicesFactory::buildTestSuiteJSONSerializerStub() const
	{
		auto jsonAdapter = std::make_unique<systelab::json::rapidjson::JSONAdapter>();
		return new allure_cpp::service::TestSuiteJSONSerializer(std::move(jsonAdapter));
	}


	// System services
	allure_cpp::service::IUUIDGeneratorService* StubServicesFactory::buildUUIDGeneratorServiceStub() const
	{
		return new allure_cpp::service::UUIDGeneratorService();
	}

	allure_cpp::service::IFileService* StubServicesFactory::buildFileServiceStub() const
	{
		return new allure_cpp::service::FileService();
	}

	allure_cpp::service::ITimeService* StubServicesFactory::buildTimeServiceStub() const
	{
		return new allure_cpp::service::TimeService();
	}

}} // namespace allure_cpp::test_utility

