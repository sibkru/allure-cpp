#include "stdafx.h"
#include "StubServicesFactory.h"

#include "Model/TestProgram.h"
#include "Services/EventHandlers/TestCaseEndEventHandler.h"
#include "Services/EventHandlers/TestCaseStartEventHandler.h"
#include "Services/EventHandlers/TestProgramEndEventHandler.h"
#include "Services/EventHandlers/TestProgramStartEventHandler.h"
#include "Services/EventHandlers/TestStepEndEventHandler.h"
#include "Services/EventHandlers/TestStepStartEventHandler.h"
#include "Services/EventHandlers/TestSuiteEndEventHandler.h"
#include "Services/EventHandlers/TestSuiteStartEventHandler.h"
#include "Services/GoogleTest/GTestEventListener.h"
#include "Services/GoogleTest/GTestStatusChecker.h"
#include "Services/Property/TestCasePropertySetter.h"
#include "Services/Property/TestSuitePropertySetter.h"
#include "Services/Report/TestCaseJSONSerializer.h"
#include "Services/Report/ContainerJSONSerializer.h"
#include "Services/Report/TestSuiteJSONSerializer.h"
#include "Services/Report/TestProgramJSONBuilder.h"
#include "Services/System/FileService.h"
#include "Services/System/TimeService.h"
#include "Services/System/UUIDGeneratorService.h"


using namespace testing;

namespace allure { namespace test_utility {

	StubServicesFactory::StubServicesFactory(allure::model::TestProgram& testProgram)
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

		return new allure::service::GTestEventListener
						(std::move(testProgramStartEventHandler), std::move(testSuiteStartEventHandler),
						 std::move(testCaseStartEventHandler), std::move(testCaseEndEventHandler),
						 std::move(testSuiteEndEventHandler), std::move(testProgramEndEventHandler));
	}

	allure::service::IGTestStatusChecker* StubServicesFactory::buildGTestStatusCheckerStub() const
	{
		return new allure::service::GTestStatusChecker();
	}

	// Lifecycle events handling services
	allure::service::ITestProgramStartEventHandler* StubServicesFactory::buildTestProgramStartEventHandlerStub() const
	{
		return new allure::service::TestProgramStartEventHandler(m_testProgram);
	}

	allure::service::ITestSuiteStartEventHandler* StubServicesFactory::buildTestSuiteStartEventHandlerStub() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return new allure::service::TestSuiteStartEventHandler(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	allure::service::ITestCaseStartEventHandler* StubServicesFactory::buildTestCaseStartEventHandlerStub() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return new allure::service::TestCaseStartEventHandler(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	allure::service::ITestStepStartEventHandler* StubServicesFactory::buildTestStepStartEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure::service::TestStepStartEventHandler(m_testProgram, std::move(timeService));
	}

	allure::service::ITestStepEndEventHandler* StubServicesFactory::buildTestStepEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		return new allure::service::TestStepEndEventHandler(m_testProgram, std::move(timeService));
	}

	allure::service::ITestCaseEndEventHandler* StubServicesFactory::buildTestCaseEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		auto testCaseJSONSerializer = std::unique_ptr<allure::service::ITestCaseJSONSerializer>(buildTestCaseJSONSerializerStub());
		auto fileService = buildFileService();
		return new allure::service::TestCaseEndEventHandler(m_testProgram, std::move(timeService), std::move(testCaseJSONSerializer), std::move(fileService));
	}

	allure::service::ITestSuiteEndEventHandler* StubServicesFactory::buildTestSuiteEndEventHandlerStub() const
	{
		auto timeService = buildTimeService();
		auto containerJSONSerializer = std::unique_ptr<allure::service::IContainerJSONSerializer>(buildContainerJSONSerializerStub());
		auto fileService = buildFileService();
		return new allure::service::TestSuiteEndEventHandler(m_testProgram, std::move(timeService), std::move(containerJSONSerializer), std::move(fileService));
	}

	allure::service::ITestProgramEndEventHandler* StubServicesFactory::buildTestProgramEndEventHandlerStub() const
	{
		auto testProgramJSONBuilder = buildTestProgramJSONBuilder();
		return new allure::service::TestProgramEndEventHandler(m_testProgram, std::move(testProgramJSONBuilder));
	}


	// Property services
	allure::service::ITestSuitePropertySetter* StubServicesFactory::buildTestSuitePropertySetterStub() const
	{
		return new allure::service::TestSuitePropertySetter(m_testProgram);
	}

	allure::service::ITestCasePropertySetter* StubServicesFactory::buildTestCasePropertySetterStub() const
	{
		return new allure::service::TestCasePropertySetter(m_testProgram);
	}


	// Report services
	allure::service::ITestProgramJSONBuilder* StubServicesFactory::buildTestProgramJSONBuilderStub() const
	{
		std::unique_ptr<allure::service::ITestCaseJSONSerializer> testCaseJSONSerializer =
			std::make_unique<allure::service::TestCaseJSONSerializer>();

		std::unique_ptr<allure::service::IContainerJSONSerializer> containerJSONSerializer =
			std::make_unique<allure::service::ContainerJSONSerializer>();

		auto fileService = buildFileService();

		return new allure::service::TestProgramJSONBuilder(std::move(testCaseJSONSerializer),
		                                                        std::move(containerJSONSerializer),
		                                                        std::move(fileService));
	}

	allure::service::ITestCaseJSONSerializer* StubServicesFactory::buildTestCaseJSONSerializerStub() const
	{
		return new allure::service::TestCaseJSONSerializer();
	}

	allure::service::IContainerJSONSerializer* StubServicesFactory::buildContainerJSONSerializerStub() const
	{
		return new allure::service::ContainerJSONSerializer();
	}

	allure::service::ITestSuiteJSONSerializer* StubServicesFactory::buildTestSuiteJSONSerializerStub() const
	{
		return new allure::service::TestSuiteJSONSerializer();
	}


	// System services
	allure::service::IUUIDGeneratorService* StubServicesFactory::buildUUIDGeneratorServiceStub() const
	{
		return new allure::service::UUIDGeneratorService();
	}

	allure::service::IFileService* StubServicesFactory::buildFileServiceStub() const
	{
		return new allure::service::FileService();
	}

	allure::service::ITimeService* StubServicesFactory::buildTimeServiceStub() const
	{
		return new allure::service::TimeService();
	}

}} // namespace allure::test_utility

