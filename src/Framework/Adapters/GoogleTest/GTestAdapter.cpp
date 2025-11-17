#include "Framework/Adapters/GoogleTest/GTestAdapter.h"
#include "Framework/Adapters/GoogleTest/GTestEventListener.h"
#include "Framework/Adapters/GoogleTest/GTestStatusProvider.h"
#include <gtest/gtest.h>


namespace allure_cpp {
namespace adapters {
namespace googletest {

	GTestAdapter::GTestAdapter(
		allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
		allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
		allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
		allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
		allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
		allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler)
		: m_programStartHandler(programStartHandler)
		, m_programEndHandler(programEndHandler)
		, m_suiteStartHandler(suiteStartHandler)
		, m_suiteEndHandler(suiteEndHandler)
		, m_caseStartHandler(caseStartHandler)
		, m_caseEndHandler(caseEndHandler)
	{
	}

	void GTestAdapter::initialize()
	{
		// Create the GoogleTest event listener
		auto listener = new GTestEventListener(
			m_programStartHandler,
			m_programEndHandler,
			m_suiteStartHandler,
			m_suiteEndHandler,
			m_caseStartHandler,
			m_caseEndHandler
		);

		// Register with GoogleTest
		// Note: GoogleTest takes ownership of the listener, so we use new without unique_ptr
		::testing::UnitTest::GetInstance()->listeners().Append(listener);
	}

	std::unique_ptr<ITestStatusProvider> GTestAdapter::createStatusProvider() const
	{
		// Create a new GoogleTest status provider
		return std::make_unique<GTestStatusProvider>();
	}

	FrameworkCapabilities GTestAdapter::getCapabilities() const
	{
		// GoogleTest capabilities
		return FrameworkCapabilities(
			true,  // supportsTestSuites - GoogleTest has test fixtures and suites
			true,  // supportsRuntimeStatus - Can query via ::testing::Test::HasFailure()
			true,  // supportsSkippedTests - Supports DISABLED_ prefix and GTEST_SKIP()
			true,  // supportsParametricTests - Has INSTANTIATE_TEST_SUITE_P
			"GoogleTest",
			// Use GTEST_VERSION if available, otherwise use a default version
#ifdef GTEST_VERSION
			GTEST_VERSION
#else
			"1.14.0"
#endif
		);
	}

}}} // namespace allure_cpp::adapters::googletest
