#include "Framework/TestLifecycleListenerBase.h"

#include <stdexcept>


namespace allure {

	TestLifecycleListenerBase::TestLifecycleListenerBase(
		allure::service::ITestProgramStartEventHandler* programStartHandler,
		allure::service::ITestProgramEndEventHandler* programEndHandler,
		allure::service::ITestSuiteStartEventHandler* suiteStartHandler,
		allure::service::ITestSuiteEndEventHandler* suiteEndHandler,
		allure::service::ITestCaseStartEventHandler* caseStartHandler,
		allure::service::ITestCaseEndEventHandler* caseEndHandler)
		: m_programStartHandler(programStartHandler)
		, m_programEndHandler(programEndHandler)
		, m_suiteStartHandler(suiteStartHandler)
		, m_suiteEndHandler(suiteEndHandler)
		, m_caseStartHandler(caseStartHandler)
		, m_caseEndHandler(caseEndHandler)
	{
		// Validate that all handlers are provided
		if (!m_programStartHandler || !m_programEndHandler ||
		    !m_suiteStartHandler || !m_suiteEndHandler ||
		    !m_caseStartHandler || !m_caseEndHandler)
		{
			throw std::invalid_argument("All event handlers must be non-null");
		}
	}

	void TestLifecycleListenerBase::onTestProgramStart()
	{
		m_programStartHandler->handleTestProgramStart();
	}

	void TestLifecycleListenerBase::onTestSuiteStart(const ITestMetadata& metadata)
	{
		// Extract suite name from metadata and delegate to existing handler
		std::string suiteName = metadata.getSuiteName();
		m_suiteStartHandler->handleTestSuiteStart(suiteName);
	}

	void TestLifecycleListenerBase::onTestStart(const ITestMetadata& metadata)
	{
		// Delegate to handler with full metadata for parametric test support
		m_caseStartHandler->handleTestCaseStart(metadata);
	}

	void TestLifecycleListenerBase::onTestEnd(const ITestMetadata& metadata,
	                                          allure::model::Status status)
	{
		// Delegate to existing handler with status
		// Note: metadata is available but current handler doesn't use it
		m_caseEndHandler->handleTestCaseEnd(status);
	}

	void TestLifecycleListenerBase::onTestSuiteEnd(allure::model::Status status)
	{
		m_suiteEndHandler->handleTestSuiteEnd(status);
	}

	void TestLifecycleListenerBase::onTestProgramEnd()
	{
		m_programEndHandler->handleTestProgramEnd();
	}

	void TestLifecycleListenerBase::onTestEnd(const ITestMetadata& metadata,
	                                          allure::model::Status status,
	                                          const std::string& statusMessage,
	                                          const std::string& statusTrace)
	{
		// Call the overloaded handler with failure details
		m_caseEndHandler->handleTestCaseEnd(status, statusMessage, statusTrace);
	}

}
