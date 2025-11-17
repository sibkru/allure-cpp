#include "GTestAllureUtilities/Framework/Adapters/GoogleTest/GTestEventListener.h"
#include "GTestAllureUtilities/Framework/Adapters/GoogleTest/GTestMetadata.h"
#include "GTestAllureUtilities/Model/Status.h"


namespace allure_cpp {
namespace adapters {
namespace googletest {

	GTestEventListener::GTestEventListener(
		allure_cpp::service::ITestProgramStartEventHandler* programStartHandler,
		allure_cpp::service::ITestProgramEndEventHandler* programEndHandler,
		allure_cpp::service::ITestSuiteStartEventHandler* suiteStartHandler,
		allure_cpp::service::ITestSuiteEndEventHandler* suiteEndHandler,
		allure_cpp::service::ITestCaseStartEventHandler* caseStartHandler,
		allure_cpp::service::ITestCaseEndEventHandler* caseEndHandler)
		: TestLifecycleListenerBase(
			programStartHandler,
			programEndHandler,
			suiteStartHandler,
			suiteEndHandler,
			caseStartHandler,
			caseEndHandler)
	{
	}

	void GTestEventListener::OnTestProgramStart(const ::testing::UnitTest& unitTest)
	{
		// Delegate to parent method
		this->onTestProgramStart();
	}

	void GTestEventListener::OnTestSuiteStart(const ::testing::TestSuite& testSuite)
	{
		// GoogleTest 1.14 uses TestSuite (newer API)
		// For suite start, we don't have a specific test, so we create metadata
		// from the first test in the suite if available
		if (testSuite.total_test_count() > 0)
		{
			const ::testing::TestInfo* firstTest = testSuite.GetTestInfo(0);
			if (firstTest)
			{
				GTestMetadata metadata(*firstTest);
				this->onTestSuiteStart(metadata);
			}
		}
	}

	void GTestEventListener::OnTestStart(const ::testing::TestInfo& testInfo)
	{
		// Create metadata wrapper
		GTestMetadata metadata(testInfo);

		// Call parent method (TestLifecycleListenerBase)
		this->onTestStart(metadata);
	}

	void GTestEventListener::OnTestEnd(const ::testing::TestInfo& testInfo)
	{
		// Create metadata wrapper
		GTestMetadata metadata(testInfo);

		// Determine status from GoogleTest's TestResult
		const ::testing::TestResult* result = testInfo.result();

		allure_cpp::model::Status status;
		std::string statusMessage;
		std::string statusTrace;

		if (result->Skipped())
		{
			status = allure_cpp::model::Status::SKIPPED;
		}
		else if (result->Passed())
		{
			status = allure_cpp::model::Status::PASSED;
		}
		else
		{
			status = allure_cpp::model::Status::FAILED;

			// Extract failure message and trace
			for (int i = 0; i < result->total_part_count(); i++)
			{
				const ::testing::TestPartResult& part = result->GetTestPartResult(i);
				if (part.failed())
				{
					if (!statusMessage.empty())
					{
						statusMessage += "\n";
					}
					statusMessage += part.message();

					if (!statusTrace.empty())
					{
						statusTrace += "\n";
					}
					// file_name() can be nullptr when failure is from an exception
					std::string location;
					if (part.file_name() != nullptr)
					{
						location = std::string(part.file_name()) + ":" + std::to_string(part.line_number());
					}
					else
					{
						location = "unknown file";
					}
					statusTrace += location + "\n" + part.message();
				}
			}
		}

		// Call parent method
		this->onTestEnd(metadata, status, statusMessage, statusTrace);
	}

	void GTestEventListener::OnTestSuiteEnd(const ::testing::TestSuite& testSuite)
	{
		// Determine suite status
		allure_cpp::model::Status status =
			testSuite.Passed()
				? allure_cpp::model::Status::PASSED
				: allure_cpp::model::Status::FAILED;

		// Call parent method
		this->onTestSuiteEnd(status);
	}

	void GTestEventListener::OnTestProgramEnd(const ::testing::UnitTest& unitTest)
	{
		// Delegate to parent method
		this->onTestProgramEnd();
	}

}}} // namespace allure_cpp::adapters::googletest
