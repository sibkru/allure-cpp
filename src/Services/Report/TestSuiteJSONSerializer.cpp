#include "TestSuiteJSONSerializer.h"

#include "Model/StepType.h"
#include "Model/TestSuite.h"


namespace allure { namespace service {

	std::string TestSuiteJSONSerializer::serialize(const model::TestSuite& testSuite) const
	{
		json j = json::object();
		addTestSuiteToJSON(testSuite, j);
		return j.dump();
	}

	void TestSuiteJSONSerializer::addTestSuiteToJSON(const model::TestSuite& testSuite, json& j) const
	{
		if (model::Format::ALLURE_FOR_JENKINS != testSuite.getFormat())
			j["uuid"] = testSuite.getUUID();

		j["name"] = testSuite.getName();

		// Add fullName field (required for Allure 2)
		if (!testSuite.getFullName().empty())
		{
			j["fullName"] = testSuite.getFullName();
		}

		// Add historyId field (required for test history tracking)
		if (!testSuite.getHistoryId().empty())
		{
			j["historyId"] = testSuite.getHistoryId();
		}

		// Add testCaseId field (for Allure TestOps)
		if (!testSuite.getTestCaseId().empty())
		{
			j["testCaseId"] = testSuite.getTestCaseId();
		}

		j["status"] = translateStatusToString(testSuite.getStatus());
		j["stage"] = translateStageToString(testSuite.getStage());

		// Convert timestamps from seconds to milliseconds for Allure 2 compatibility
		j["start"] = testSuite.getStart() * 1000LL;
		j["stop"] = testSuite.getStop() * 1000LL;

		addLabelsToJSON(testSuite, j);
		addLinksToJSON(testSuite.getLinks(), j);
		addTestCasesToJSON(testSuite.getTestCases(), j);
	}

	void TestSuiteJSONSerializer::addLabelsToJSON(const model::TestSuite& testSuite, json& j) const
	{
		json labelsArray = json::array();

		json suiteLabel = {
			{"name", "suite"},
			{"value", testSuite.getName()}
		};
		labelsArray.push_back(suiteLabel);

		const std::vector<model::Label>& labels = testSuite.getLabels();
		if (labels.size() > 0)
		{
			for (const auto& label : labels)
			{
				json labelObj = {
					{"name", label.getName()},
					{"value", label.getValue()}
				};
				labelsArray.push_back(labelObj);
			}
		}

		j["labels"] = labelsArray;
	}

	void TestSuiteJSONSerializer::addLinksToJSON(const std::vector<model::Link>& links, json& j) const
	{
		if (links.size() > 0)
		{
			json linksArray = json::array();
			for (const auto& link : links)
			{
				json linkObj = {
					{"name", link.getName()},
					{"url", link.getURL()},
					{"type", link.getType()}
				};
				linksArray.push_back(linkObj);
			}

			j["links"] = linksArray;
		}
	}

	void TestSuiteJSONSerializer::addTestCasesToJSON(const std::vector<model::TestCase>& testCases, json& j) const
	{
		if (testCases.size() > 0)
		{
			json testCasesArray = json::array();
			for (const auto& testCase : testCases)
			{
				json testCaseObj = json::object();
				testCaseObj["name"] = "Action: " + testCase.getName();
				testCaseObj["status"] = translateStatusToString(testCase.getStatus());
				testCaseObj["stage"] = translateStageToString(testCase.getStage());
				// Convert timestamps from seconds to milliseconds
				testCaseObj["start"] = testCase.getStart() * 1000LL;
				testCaseObj["stop"] = testCase.getStop() * 1000LL;

				addTestCaseStepsToJSON(testCase, testCaseObj);

				testCasesArray.push_back(testCaseObj);
			}

			j["steps"] = testCasesArray;
		}
	}

	void TestSuiteJSONSerializer::addTestCaseStepsToJSON(const model::TestCase& testCase, json& j) const
	{
		unsigned int nSteps = testCase.getStepCount();
		if (nSteps > 0)
		{
			json stepsArray = json::array();
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto step = testCase.getStep(i);
				json stepObj = json::object();
				auto actionPrefix = (step->getStepType() == model::StepType::ACTION_STEP) ? "Action: " : "";

				stepObj["name"] = actionPrefix + step->getName();
				stepObj["status"] = translateStatusToString(step->getStatus());
				stepObj["stage"] = translateStageToString(step->getStage());
				// Convert timestamps from seconds to milliseconds
				stepObj["start"] = step->getStart() * 1000LL;
				stepObj["stop"] = step->getStop() * 1000LL;

				stepsArray.push_back(stepObj);
			}

			j["steps"] = stepsArray;
		}
	}

	std::string TestSuiteJSONSerializer::translateStatusToString(model::Status status) const
	{
		if (status == model::Status::SKIPPED)
		{
			return "skipped";
		}
		else if (status == model::Status::BROKEN)
		{
			return "broken";
		}
		else if (status == model::Status::FAILED)
		{
			return "failed";
		}
		else if (status == model::Status::PASSED)
		{
			return "passed";
		}
		else // (status == model::Status::UNKNOWN)
		{
			return "unknown";
		}
	}

	std::string TestSuiteJSONSerializer::translateStageToString(model::Stage stage) const
	{
		if (stage == model::Stage::FINISHED)
		{
			return "finished";
		}
		else if (stage == model::Stage::SCHEDULED)
		{
			return "scheduled";
		}
		else if (stage == model::Stage::INTERRUPTED)
		{
			return "interrupted";
		}
		else if (stage == model::Stage::RUNNING)
		{
			return "running";
		}
		else // (stage == model::Stage::PENDING)
		{
			return "pending";
		}
	}

}} // namespace allure::service
