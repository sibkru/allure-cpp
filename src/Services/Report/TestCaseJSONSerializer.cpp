#include "TestCaseJSONSerializer.h"

#include "Model/Attachment.h"
#include "Model/Label.h"
#include "Model/Link.h"
#include "Model/Parameter.h"
#include "Model/Step.h"
#include "Model/StepType.h"
#include "Model/TestCase.h"


namespace allure_cpp { namespace service {

	std::string TestCaseJSONSerializer::serialize(const model::TestCase& testCase) const
	{
		json j = json::object();
		addTestCaseToJSON(testCase, j);
		return j.dump();
	}

	void TestCaseJSONSerializer::addTestCaseToJSON(const model::TestCase& testCase, json& j) const
	{
		// Required fields
		j["uuid"] = testCase.getUUID();
		j["historyId"] = testCase.getHistoryId();
		j["name"] = testCase.getName();
		j["fullName"] = testCase.getFullName();
		j["status"] = translateStatusToString(testCase.getStatus());
		j["stage"] = translateStageToString(testCase.getStage());

		// Timestamps are already in milliseconds (from TimeService)
		j["start"] = testCase.getStart();
		j["stop"] = testCase.getStop();

		// Optional fields
		if (!testCase.getTestCaseId().empty())
		{
			j["testCaseId"] = testCase.getTestCaseId();
		}

		if (!testCase.getDescription().empty())
		{
			j["description"] = testCase.getDescription();
		}

		if (!testCase.getDescriptionHtml().empty())
		{
			j["descriptionHtml"] = testCase.getDescriptionHtml();
		}

		// Add statusDetails if any status detail fields are present
		if (!testCase.getStatusMessage().empty() || !testCase.getStatusTrace().empty() ||
		    testCase.getStatusKnown() || testCase.getStatusMuted() || testCase.getStatusFlaky())
		{
			json statusDetails = json::object();

			if (testCase.getStatusKnown())
			{
				statusDetails["known"] = true;
			}

			if (testCase.getStatusMuted())
			{
				statusDetails["muted"] = true;
			}

			if (testCase.getStatusFlaky())
			{
				statusDetails["flaky"] = true;
			}

			if (!testCase.getStatusMessage().empty())
			{
				statusDetails["message"] = testCase.getStatusMessage();
			}

			if (!testCase.getStatusTrace().empty())
			{
				statusDetails["trace"] = testCase.getStatusTrace();
			}

			j["statusDetails"] = statusDetails;
		}

		// Add arrays
		addLabelsToJSON(testCase.getLabels(), j);
		addParametersToJSON(testCase.getParameters(), j);
		addLinksToJSON(testCase.getLinks(), j);
		addAttachmentsToJSON(testCase.getAttachments(), j);
		addStepsToJSON(testCase, j);
	}

	void TestCaseJSONSerializer::addLabelsToJSON(const std::vector<model::Label>& labels, json& j) const
	{
		if (labels.size() > 0)
		{
			json labelsArray = json::array();
			for (const auto& label : labels)
			{
				json labelObj = {
					{"name", label.getName()},
					{"value", label.getValue()}
				};
				labelsArray.push_back(labelObj);
			}
			j["labels"] = labelsArray;
		}
	}

	void TestCaseJSONSerializer::addParametersToJSON(const std::vector<model::Parameter>& parameters, json& j) const
	{
		if (parameters.size() > 0)
		{
			json parametersArray = json::array();
			for (const auto& parameter : parameters)
			{
				json paramObj = {
					{"name", parameter.getName()},
					{"value", parameter.getValue()}
				};

				// Add optional fields only if they differ from defaults
				if (parameter.getExcluded())
				{
					paramObj["excluded"] = true;
				}

				if (parameter.getMode() != "default")
				{
					paramObj["mode"] = parameter.getMode();
				}

				parametersArray.push_back(paramObj);
			}
			j["parameters"] = parametersArray;
		}
	}

	void TestCaseJSONSerializer::addLinksToJSON(const std::vector<model::Link>& links, json& j) const
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

	void TestCaseJSONSerializer::addAttachmentsToJSON(const std::vector<model::Attachment>& attachments, json& j) const
	{
		if (attachments.size() > 0)
		{
			json attachmentsArray = json::array();
			for (const auto& attachment : attachments)
			{
				json attachmentObj = {
					{"name", attachment.getName()},
					{"source", attachment.getSource()},
					{"type", attachment.getType()}
				};
				attachmentsArray.push_back(attachmentObj);
			}
			j["attachments"] = attachmentsArray;
		}
	}

	void TestCaseJSONSerializer::addStepsToJSON(const model::TestCase& testCase, json& j) const
	{
		unsigned int nSteps = testCase.getStepCount();
		if (nSteps > 0)
		{
			json stepsArray = json::array();
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto step = testCase.getStep(i);
				json stepObj = json::object();
				addStepToJSON(step, stepObj);
				stepsArray.push_back(stepObj);
			}
			j["steps"] = stepsArray;
		}
	}

	void TestCaseJSONSerializer::addStepToJSON(const model::Step* step, json& j) const
	{
		auto actionPrefix = (step->getStepType() == model::StepType::ACTION_STEP) ? "Action: " : "";

		j["name"] = actionPrefix + step->getName();
		j["status"] = translateStatusToString(step->getStatus());
		j["stage"] = translateStageToString(step->getStage());
		// Timestamps are already in milliseconds (from TimeService)
		j["start"] = step->getStart();
		j["stop"] = step->getStop();

		// Add parameters if present
		const auto& parameters = step->getParameters();
		if (parameters.size() > 0)
		{
			json parametersArray = json::array();
			for (const auto& parameter : parameters)
			{
				json paramObj = {
					{"name", parameter.getName()},
					{"value", parameter.getValue()}
				};

				if (parameter.getExcluded())
				{
					paramObj["excluded"] = true;
				}

				if (parameter.getMode() != "default")
				{
					paramObj["mode"] = parameter.getMode();
				}

				parametersArray.push_back(paramObj);
			}
			j["parameters"] = parametersArray;
		}

		// Add attachments if present
		const auto& attachments = step->getAttachments();
		if (attachments.size() > 0)
		{
			json attachmentsArray = json::array();
			for (const auto& attachment : attachments)
			{
				json attachmentObj = {
					{"name", attachment.getName()},
					{"source", attachment.getSource()},
					{"type", attachment.getType()}
				};
				attachmentsArray.push_back(attachmentObj);
			}
			j["attachments"] = attachmentsArray;
		}

		// Add nested steps (recursive structure)
		unsigned int nSteps = step->getStepCount();
		if (nSteps > 0)
		{
			json stepsArray = json::array();
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto nestedStep = step->getStep(i);
				json nestedStepObj = json::object();
				addStepToJSON(nestedStep, nestedStepObj);
				stepsArray.push_back(nestedStepObj);
			}
			j["steps"] = stepsArray;
		}
	}

	std::string TestCaseJSONSerializer::translateStatusToString(model::Status status) const
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

	std::string TestCaseJSONSerializer::translateStageToString(model::Stage stage) const
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

}} // namespace allure_cpp::service
