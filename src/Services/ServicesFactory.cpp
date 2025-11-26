#include "ServicesFactory.h"

#include "Model/TestSuite.h"
#include "Services/EventHandlers/TestCaseEndEventHandler.h"
#include "Services/EventHandlers/TestCaseStartEventHandler.h"
#include "Services/EventHandlers/TestProgramEndEventHandler.h"
#include "Services/EventHandlers/TestProgramStartEventHandler.h"
#include "Services/EventHandlers/TestStepEndEventHandler.h"
#include "Services/EventHandlers/TestStepStartEventHandler.h"
#include "Services/EventHandlers/TestSuiteEndEventHandler.h"
#include "Services/EventHandlers/TestSuiteStartEventHandler.h"
#ifdef ALLURE_GOOGLETEST_ENABLED
#include "Services/GoogleTest/GTestEventListener.h"
#include "Services/GoogleTest/GTestStatusChecker.h"
#endif
#include "Services/Property/TestCasePropertySetter.h"
#include "Services/Property/TestSuitePropertySetter.h"
#include "Services/System/FileService.h"
#include "Services/System/TimeService.h"
#include "Services/System/UUIDGeneratorService.h"
#include "Services/Report/TestCaseJSONSerializer.h"
#include "Services/Report/ContainerJSONSerializer.h"
#include "Services/Report/TestSuiteJSONSerializer.h"
#include "Services/Report/TestProgramJSONBuilder.h"


namespace allure { namespace service {

	ServicesFactory::ServicesFactory(model::TestProgram& testProgram)
		:m_testProgram(testProgram)
	{
	}


	// GTest services
#ifdef ALLURE_GOOGLETEST_ENABLED
	std::unique_ptr<::testing::TestEventListener> ServicesFactory::buildGTestEventListener() const
	{
		auto testProgramStartEventHandler = buildTestProgramStartEventHandler();
		auto testSuiteStartEventHandler = buildTestSuiteStartEventHandler();
		auto testCaseStartEventHandler = buildTestCaseStartEventHandler();
		auto testCaseEndEventHandler = buildTestCaseEndEventHandler();
		auto testSuiteEndEventHandler = buildTestSuiteEndEventHandler();
		auto testProgramEndEventHandler = buildTestProgramEndEventHandler();

		return std::make_unique<GTestEventListener>(std::move(testProgramStartEventHandler), std::move(testSuiteStartEventHandler),
													std::move(testCaseStartEventHandler), std::move(testCaseEndEventHandler),
													std::move(testSuiteEndEventHandler), std::move(testProgramEndEventHandler));
	}

	std::unique_ptr<IGTestStatusChecker> ServicesFactory::buildGTestStatusChecker() const
	{
		return std::make_unique<GTestStatusChecker>();
	}
#endif


	// Lifecycle events handling services
	std::unique_ptr<ITestProgramStartEventHandler> ServicesFactory::buildTestProgramStartEventHandler() const
	{
		return std::make_unique<TestProgramStartEventHandler>(m_testProgram);
	}

	std::unique_ptr<ITestSuiteStartEventHandler> ServicesFactory::buildTestSuiteStartEventHandler() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return std::make_unique<TestSuiteStartEventHandler>(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	std::unique_ptr<ITestCaseStartEventHandler> ServicesFactory::buildTestCaseStartEventHandler() const
	{
		auto uuidGeneratorService = buildUUIDGeneratorService();
		auto timeService = buildTimeService();
		return std::make_unique<TestCaseStartEventHandler>(m_testProgram, std::move(uuidGeneratorService), std::move(timeService));
	}

	std::unique_ptr<ITestStepStartEventHandler> ServicesFactory::buildTestStepStartEventHandler() const
	{
		auto timeService = buildTimeService();
		return std::make_unique<TestStepStartEventHandler>(m_testProgram, std::move(timeService));
	}

	std::unique_ptr<ITestStepEndEventHandler> ServicesFactory::buildTestStepEndEventHandler() const
	{
		auto timeService = buildTimeService();
		return std::make_unique<TestStepEndEventHandler>(m_testProgram, std::move(timeService));
	}

	std::unique_ptr<ITestCaseEndEventHandler> ServicesFactory::buildTestCaseEndEventHandler() const
	{
		auto timeService = buildTimeService();
		auto testCaseJSONSerializer = buildTestCaseJSONSerializer();
		auto fileService = buildFileService();
		return std::make_unique<TestCaseEndEventHandler>(m_testProgram, std::move(timeService), std::move(testCaseJSONSerializer), std::move(fileService));
	}

	std::unique_ptr<ITestSuiteEndEventHandler> ServicesFactory::buildTestSuiteEndEventHandler() const
	{
		auto timeService = buildTimeService();
		auto containerJSONSerializer = buildContainerJSONSerializer();
		auto fileService = buildFileService();
		return std::make_unique<TestSuiteEndEventHandler>(m_testProgram, std::move(timeService), std::move(containerJSONSerializer), std::move(fileService));
	}

	std::unique_ptr<ITestProgramEndEventHandler> ServicesFactory::buildTestProgramEndEventHandler() const
	{
		auto testProgramJSONBuilder = buildTestProgramJSONBuilder();
		return std::make_unique<TestProgramEndEventHandler>(m_testProgram, std::move(testProgramJSONBuilder));
	}


	// Property services
	std::unique_ptr<ITestSuitePropertySetter> ServicesFactory::buildTestSuitePropertySetter() const
	{
		return std::make_unique<TestSuitePropertySetter>(m_testProgram);
	}

	std::unique_ptr<ITestCasePropertySetter> ServicesFactory::buildTestCasePropertySetter() const
	{
		return std::make_unique<TestCasePropertySetter>(m_testProgram);
	}


	// JSON services
	std::unique_ptr<ITestProgramJSONBuilder> ServicesFactory::buildTestProgramJSONBuilder() const
	{
		auto testCaseJSONSerializer = buildTestCaseJSONSerializer();
		auto containerJSONSerializer = buildContainerJSONSerializer();
		auto fileService = buildFileService();
		return std::make_unique<TestProgramJSONBuilder>(std::move(testCaseJSONSerializer),
		                                                  std::move(containerJSONSerializer),
		                                                  std::move(fileService));
	}

	std::unique_ptr<ITestCaseJSONSerializer> ServicesFactory::buildTestCaseJSONSerializer() const
	{
		return std::make_unique<TestCaseJSONSerializer>();
	}

	std::unique_ptr<IContainerJSONSerializer> ServicesFactory::buildContainerJSONSerializer() const
	{
		return std::make_unique<ContainerJSONSerializer>();
	}

	std::unique_ptr<ITestSuiteJSONSerializer> ServicesFactory::buildTestSuiteJSONSerializer() const
	{
		return std::make_unique<TestSuiteJSONSerializer>();
	}


	// System services
	std::unique_ptr<IUUIDGeneratorService> ServicesFactory::buildUUIDGeneratorService() const
	{
		return std::make_unique<UUIDGeneratorService>();
	}

	std::unique_ptr<IFileService> ServicesFactory::buildFileService() const
	{
		return std::make_unique<FileService>();
	}

	std::unique_ptr<ITimeService> ServicesFactory::buildTimeService() const
	{
		return std::make_unique<TimeService>();
	}


	// Unique instance (to be used by integration tests)
	std::unique_ptr<IServicesFactory> ServicesFactory::m_instance = nullptr;

	IServicesFactory* ServicesFactory::getInstance()
	{
		return m_instance.get();
	}

	void ServicesFactory::setInstance(std::unique_ptr<IServicesFactory> instance)
	{
		m_instance = std::move(instance);
	}

}} // namespace allure::service
