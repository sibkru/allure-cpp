#include "Framework/Adapters/GoogleTest/AllureGTest.h"

#include "AllureAPI.h"
#include "Framework/Adapters/GoogleTest/GTestAdapter.h"
#include "Model/TestProgram.h"
#include "Services/ServicesFactory.h"

#include <gtest/gtest.h>

namespace allure_cpp { namespace adapters { namespace googletest {

class AllureGTest::Impl
{
public:
	Impl(const std::string& outputFolder)
	{
		AllureAPI::setOutputFolder(outputFolder);
		AllureAPI::setFrameworkName("GoogleTest");

		auto& testProgram = AllureAPI::getTestProgram();
		m_servicesFactory = std::make_unique<service::ServicesFactory>(testProgram);

		auto adapter = std::make_shared<GTestAdapter>(
			m_servicesFactory->buildTestProgramStartEventHandler(),
			m_servicesFactory->buildTestProgramEndEventHandler(),
			m_servicesFactory->buildTestSuiteStartEventHandler(),
			m_servicesFactory->buildTestSuiteEndEventHandler(),
			m_servicesFactory->buildTestCaseStartEventHandler(),
			m_servicesFactory->buildTestCaseEndEventHandler()
		);
		adapter->initialize();
		AllureAPI::setFrameworkAdapter(adapter);
		m_adapter = std::move(adapter);

		// Remove default printer to avoid duplicate output alongside Allure listener
		auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
		delete listeners.Release(listeners.default_result_printer());
	}

private:
	std::shared_ptr<GTestAdapter> m_adapter;
	std::unique_ptr<service::ServicesFactory> m_servicesFactory;
};

AllureGTest::AllureGTest(const std::string& outputFolder)
	: m_impl(std::make_unique<Impl>(outputFolder))
{
}

AllureGTest::~AllureGTest() = default;

}}} // namespace allure_cpp::adapters::googletest
