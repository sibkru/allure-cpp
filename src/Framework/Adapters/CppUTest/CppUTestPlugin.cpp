#include "Framework/Adapters/CppUTest/AllureCppUTestOutput.h"
#include "Framework/Adapters/CppUTest/CppUTestPlugin.h"
#include "Framework/Adapters/CppUTest/CppUTestMetadata.h"
#include "Model/Status.h"
#include <CppUTest/TestHarness.h>
#include <CppUTest/TestPlugin.h>
#include <CppUTest/MemoryLeakWarningPlugin.h>
#include <CppUTest/MemoryLeakDetector.h>


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	CppUTestPlugin::CppUTestPlugin(
		allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
		allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
		allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
		allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
		allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
		allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler)
		: TestPlugin("AllureReportingPlugin")
		, TestLifecycleListenerBase(
			programStartHandler,
			programEndHandler,
			suiteStartHandler,
			suiteEndHandler,
			caseStartHandler,
			caseEndHandler)
		, m_currentGroup("")
		, m_groupStarted(false)
		, m_initialFailureCount(0)
	{
	}

	void CppUTestPlugin::preTestAction(UtestShell& test, TestResult& result)
	{
		if (!test.willRun())
		{
			// --- Handle Ignored/Skipped Test ---
			// For ignored tests, pre/postTestAction are NOT called, so we must handle
			// the entire lifecycle here.

			// Disable leak detection for Allure operations
			auto* leakDetector = MemoryLeakWarningPlugin::getFirstPlugin();
			if (leakDetector != nullptr)
			{
				leakDetector->getMemoryLeakDetector()->disable();
			}

			// Start program if it's the very first test
			static bool programStarted = false;
			if (!programStarted)
			{
				this->onTestProgramStart();
				programStarted = true;
			}

			// Start test suite if it's a new group
			std::string group = test.getGroup().asCharString();
			if (group != m_currentGroup)
			{
				if (m_groupStarted)
				{
					this->onTestSuiteEnd(allure_cpp::model::Status::PASSED);
				}
				m_currentGroup = group;
				m_groupStarted = true;
				CppUTestMetadata metadata(test);
				this->onTestSuiteStart(metadata);
			}

			// Report the skipped test
			CppUTestMetadata metadata(test);
			this->onTestStart(metadata);
			this->onTestEnd(metadata, allure_cpp::model::Status::SKIPPED, "Test is ignored", "");

			// Re-enable leak detection
			if (leakDetector != nullptr)
			{
				leakDetector->getMemoryLeakDetector()->enable();
			}
			return;
		}

		// Cache the failure count before the test runs
		m_initialFailureCount = result.getFailureCount();

		// Disable leak detection for all Allure library operations
		auto* leakDetector = MemoryLeakWarningPlugin::getFirstPlugin();
		if (leakDetector != nullptr)
		{
			leakDetector->getMemoryLeakDetector()->disable();
		}

		// If this is the very first test, start the test program
		static bool programStarted = false;
		if (!programStarted)
		{
			this->onTestProgramStart();
			programStarted = true;
		}

		// Check if we're starting a new test group
		std::string group = test.getGroup().asCharString();

		if (group != m_currentGroup)
		{
			// End previous group if one was started
			if (m_groupStarted)
			{
				// Determine previous group status (passed if we're here)
				this->onTestSuiteEnd(allure_cpp::model::Status::PASSED);
			}

			// Start new group
			m_currentGroup = group;
			m_groupStarted = true;

			// Create metadata for the first test in this group
			CppUTestMetadata metadata(test);
			this->onTestSuiteStart(metadata);
		}

		// Create metadata wrapper
		CppUTestMetadata metadata(test);

		// Call parent method (TestLifecycleListenerBase)
		this->onTestStart(metadata);

		// Re-enable leak detection for user test code
		if (leakDetector != nullptr)
		{
			leakDetector->getMemoryLeakDetector()->enable();
		}
	}

	void CppUTestPlugin::postTestAction(UtestShell& test, TestResult& result)
	{
		// Disable leak detection for all Allure library operations
		auto* leakDetector = MemoryLeakWarningPlugin::getFirstPlugin();
		if (leakDetector != nullptr)
		{
			leakDetector->getMemoryLeakDetector()->disable();
		}

		// Create metadata wrapper
		CppUTestMetadata metadata(test);

		// Determine status by comparing failure count before and after the test
		allure_cpp::model::Status status;
		std::string statusMessage;
		std::string statusTrace;

		if (result.getFailureCount() > m_initialFailureCount)
		{
			status = allure_cpp::model::Status::FAILED;
            statusMessage = AllureCppUTestOutput::getAndClearFailureMessage();
            if (statusMessage.empty()) {
			    statusMessage = "Test failed";
            }
			statusTrace = test.getFile().asCharString() +
			              std::string(":") +
			              std::to_string(test.getLineNumber());
		}
		else
		{
			status = allure_cpp::model::Status::PASSED;
		}

		// Call parent method
		this->onTestEnd(metadata, status, statusMessage, statusTrace);

		// Re-enable leak detection after Allure operations complete
		if (leakDetector != nullptr)
		{
			leakDetector->getMemoryLeakDetector()->enable();
		}
	}

}}}