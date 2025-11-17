#include "TestCaseJSONSerializer.h"

#include "Model/Attachment.h"
#include "Model/Label.h"
#include "Model/Link.h"
#include "Model/Parameter.h"
#include "Model/Step.h"
#include "Model/StepType.h"
#include "Model/TestCase.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONValue.h"


namespace allure_cpp { namespace service {

	TestCaseJSONSerializer::TestCaseJSONSerializer(std::unique_ptr<systelab::json::IJSONAdapter> jsonAdapter)
		:m_jsonAdapter(std::move(jsonAdapter))
	{
	}

	std::string TestCaseJSONSerializer::serialize(const model::TestCase& testCase) const
	{
		auto jsonDocument = m_jsonAdapter->buildEmptyDocument();
		auto& jsonDocumentRoot = jsonDocument->getRootValue();
		jsonDocumentRoot.setType(systelab::json::OBJECT_TYPE);

		addTestCaseToJSON(testCase, jsonDocumentRoot);

		return jsonDocument->serialize();
	}

	void TestCaseJSONSerializer::addTestCaseToJSON(const model::TestCase& testCase, systelab::json::IJSONValue& jsonParent) const
	{
		// Required fields
		jsonParent.addMember("uuid", testCase.getUUID());
		jsonParent.addMember("historyId", testCase.getHistoryId());
		jsonParent.addMember("name", testCase.getName());
		jsonParent.addMember("fullName", testCase.getFullName());
		jsonParent.addMember("status", translateStatusToString(testCase.getStatus()));
		jsonParent.addMember("stage", translateStageToString(testCase.getStage()));

		// Timestamps are already in milliseconds (from TimeService)
		jsonParent.addMember("start", testCase.getStart());
		jsonParent.addMember("stop", testCase.getStop());

		// Optional fields
		if (!testCase.getTestCaseId().empty())
		{
			jsonParent.addMember("testCaseId", testCase.getTestCaseId());
		}

		if (!testCase.getDescription().empty())
		{
			jsonParent.addMember("description", testCase.getDescription());
		}

		if (!testCase.getDescriptionHtml().empty())
		{
			jsonParent.addMember("descriptionHtml", testCase.getDescriptionHtml());
		}

		// Add statusDetails if any status detail fields are present
		if (!testCase.getStatusMessage().empty() || !testCase.getStatusTrace().empty() ||
		    testCase.getStatusKnown() || testCase.getStatusMuted() || testCase.getStatusFlaky())
		{
			auto jsonStatusDetails = jsonParent.buildValue(systelab::json::OBJECT_TYPE);

			if (testCase.getStatusKnown())
			{
				jsonStatusDetails->addMember("known", true);
			}

			if (testCase.getStatusMuted())
			{
				jsonStatusDetails->addMember("muted", true);
			}

			if (testCase.getStatusFlaky())
			{
				jsonStatusDetails->addMember("flaky", true);
			}

			if (!testCase.getStatusMessage().empty())
			{
				jsonStatusDetails->addMember("message", testCase.getStatusMessage());
			}

			if (!testCase.getStatusTrace().empty())
			{
				jsonStatusDetails->addMember("trace", testCase.getStatusTrace());
			}

			jsonParent.addMember("statusDetails", std::move(jsonStatusDetails));
		}

		// Add arrays
		addLabelsToJSON(testCase.getLabels(), jsonParent);
		addParametersToJSON(testCase.getParameters(), jsonParent);
		addLinksToJSON(testCase.getLinks(), jsonParent);
		addAttachmentsToJSON(testCase.getAttachments(), jsonParent);
		addStepsToJSON(testCase, jsonParent);
	}

	void TestCaseJSONSerializer::addLabelsToJSON(const std::vector<model::Label>& labels, systelab::json::IJSONValue& jsonParent) const
	{
		if (labels.size() > 0)
		{
			auto jsonLabelsArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& label : labels)
			{
				auto jsonLabel = jsonParent.buildValue(systelab::json::OBJECT_TYPE);
				jsonLabel->addMember("name", label.getName());
				jsonLabel->addMember("value", label.getValue());
				jsonLabelsArray->addArrayValue(std::move(jsonLabel));
			}
			jsonParent.addMember("labels", std::move(jsonLabelsArray));
		}
	}

	void TestCaseJSONSerializer::addParametersToJSON(const std::vector<model::Parameter>& parameters, systelab::json::IJSONValue& jsonParent) const
	{
		if (parameters.size() > 0)
		{
			auto jsonParametersArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& parameter : parameters)
			{
				auto jsonParameter = jsonParent.buildValue(systelab::json::OBJECT_TYPE);
				jsonParameter->addMember("name", parameter.getName());
				jsonParameter->addMember("value", parameter.getValue());

				// Add optional fields only if they differ from defaults
				if (parameter.getExcluded())
				{
					jsonParameter->addMember("excluded", true);
				}

				if (parameter.getMode() != "default")
				{
					jsonParameter->addMember("mode", parameter.getMode());
				}

				jsonParametersArray->addArrayValue(std::move(jsonParameter));
			}
			jsonParent.addMember("parameters", std::move(jsonParametersArray));
		}
	}

	void TestCaseJSONSerializer::addLinksToJSON(const std::vector<model::Link>& links, systelab::json::IJSONValue& jsonParent) const
	{
		if (links.size() > 0)
		{
			auto jsonLinksArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& link : links)
			{
				auto jsonLink = jsonParent.buildValue(systelab::json::OBJECT_TYPE);
				jsonLink->addMember("name", link.getName());
				jsonLink->addMember("url", link.getURL());
				jsonLink->addMember("type", link.getType());
				jsonLinksArray->addArrayValue(std::move(jsonLink));
			}
			jsonParent.addMember("links", std::move(jsonLinksArray));
		}
	}

	void TestCaseJSONSerializer::addAttachmentsToJSON(const std::vector<model::Attachment>& attachments, systelab::json::IJSONValue& jsonParent) const
	{
		if (attachments.size() > 0)
		{
			auto jsonAttachmentsArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& attachment : attachments)
			{
				auto jsonAttachment = jsonParent.buildValue(systelab::json::OBJECT_TYPE);
				jsonAttachment->addMember("name", attachment.getName());
				jsonAttachment->addMember("source", attachment.getSource());
				jsonAttachment->addMember("type", attachment.getType());
				jsonAttachmentsArray->addArrayValue(std::move(jsonAttachment));
			}
			jsonParent.addMember("attachments", std::move(jsonAttachmentsArray));
		}
	}

	void TestCaseJSONSerializer::addStepsToJSON(const model::TestCase& testCase, systelab::json::IJSONValue& jsonParent) const
	{
		unsigned int nSteps = testCase.getStepCount();
		if (nSteps > 0)
		{
			auto jsonStepsArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto step = testCase.getStep(i);
				auto jsonStep = jsonStepsArray->buildValue(systelab::json::OBJECT_TYPE);
				addStepToJSON(step, *jsonStep);
				jsonStepsArray->addArrayValue(std::move(jsonStep));
			}
			jsonParent.addMember("steps", std::move(jsonStepsArray));
		}
	}

	void TestCaseJSONSerializer::addStepToJSON(const model::Step* step, systelab::json::IJSONValue& jsonStep) const
	{
		auto actionPrefix = (step->getStepType() == model::StepType::ACTION_STEP) ? "Action: " : "";

		jsonStep.addMember("name", actionPrefix + step->getName());
		jsonStep.addMember("status", translateStatusToString(step->getStatus()));
		jsonStep.addMember("stage", translateStageToString(step->getStage()));
		// Timestamps are already in milliseconds (from TimeService)
		jsonStep.addMember("start", step->getStart());
		jsonStep.addMember("stop", step->getStop());

		// Add parameters if present
		const auto& parameters = step->getParameters();
		if (parameters.size() > 0)
		{
			auto jsonParametersArray = jsonStep.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& parameter : parameters)
			{
				auto jsonParameter = jsonStep.buildValue(systelab::json::OBJECT_TYPE);
				jsonParameter->addMember("name", parameter.getName());
				jsonParameter->addMember("value", parameter.getValue());

				if (parameter.getExcluded())
				{
					jsonParameter->addMember("excluded", true);
				}

				if (parameter.getMode() != "default")
				{
					jsonParameter->addMember("mode", parameter.getMode());
				}

				jsonParametersArray->addArrayValue(std::move(jsonParameter));
			}
			jsonStep.addMember("parameters", std::move(jsonParametersArray));
		}

		// Add attachments if present
		const auto& attachments = step->getAttachments();
		if (attachments.size() > 0)
		{
			auto jsonAttachmentsArray = jsonStep.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& attachment : attachments)
			{
				auto jsonAttachment = jsonStep.buildValue(systelab::json::OBJECT_TYPE);
				jsonAttachment->addMember("name", attachment.getName());
				jsonAttachment->addMember("source", attachment.getSource());
				jsonAttachment->addMember("type", attachment.getType());
				jsonAttachmentsArray->addArrayValue(std::move(jsonAttachment));
			}
			jsonStep.addMember("attachments", std::move(jsonAttachmentsArray));
		}

		// Add nested steps (recursive structure)
		unsigned int nSteps = step->getStepCount();
		if (nSteps > 0)
		{
			auto jsonStepsArray = jsonStep.buildValue(systelab::json::ARRAY_TYPE);
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto nestedStep = step->getStep(i);
				auto jsonNestedStep = jsonStep.buildValue(systelab::json::OBJECT_TYPE);
				addStepToJSON(nestedStep, *jsonNestedStep);
				jsonStepsArray->addArrayValue(std::move(jsonNestedStep));
			}
			jsonStep.addMember("steps", std::move(jsonStepsArray));
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
