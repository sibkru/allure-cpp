#include "ContainerJSONSerializer.h"

#include "Model/Container.h"

#include "JSONAdapterInterface/IJSONAdapter.h"
#include "JSONAdapterInterface/IJSONDocument.h"
#include "JSONAdapterInterface/IJSONValue.h"


namespace allure_cpp { namespace service {

	ContainerJSONSerializer::ContainerJSONSerializer(std::unique_ptr<systelab::json::IJSONAdapter> jsonAdapter)
		:m_jsonAdapter(std::move(jsonAdapter))
	{
	}

	std::string ContainerJSONSerializer::serialize(const model::Container& container) const
	{
		auto jsonDocument = m_jsonAdapter->buildEmptyDocument();
		auto& jsonDocumentRoot = jsonDocument->getRootValue();
		jsonDocumentRoot.setType(systelab::json::OBJECT_TYPE);

		addContainerToJSON(container, jsonDocumentRoot);

		return jsonDocument->serialize();
	}

	void ContainerJSONSerializer::addContainerToJSON(const model::Container& container, systelab::json::IJSONValue& jsonParent) const
	{
		// Required fields
		jsonParent.addMember("uuid", container.getUUID());

		// Optional name field
		if (!container.getName().empty())
		{
			jsonParent.addMember("name", container.getName());
		}

		// Convert timestamps from seconds to milliseconds for Allure 2 compatibility
		jsonParent.addMember("start", container.getStart() * 1000LL);
		jsonParent.addMember("stop", container.getStop() * 1000LL);

		// Add children array (UUIDs of test case result files)
		addChildrenToJSON(container.getChildren(), jsonParent);

		// Add befores and afters (fixture steps)
		addFixtureStepsToJSON(container.getBefores(), jsonParent, "befores");
		addFixtureStepsToJSON(container.getAfters(), jsonParent, "afters");
	}

	void ContainerJSONSerializer::addChildrenToJSON(const std::vector<std::string>& children, systelab::json::IJSONValue& jsonParent) const
	{
		if (children.size() > 0)
		{
			auto jsonChildrenArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& childUUID : children)
			{
				auto jsonStringValue = jsonParent.buildValue(systelab::json::STRING_TYPE);
				jsonStringValue->setString(childUUID);
				jsonChildrenArray->addArrayValue(std::move(jsonStringValue));
			}
			jsonParent.addMember("children", std::move(jsonChildrenArray));
		}
	}

	void ContainerJSONSerializer::addFixtureStepsToJSON(const std::vector<model::FixtureStep>& fixtureSteps,
	                                                     systelab::json::IJSONValue& jsonParent,
	                                                     const std::string& fieldName) const
	{
		// Always include befores/afters arrays, even if empty (required by Allure 2)
		auto jsonFixtureStepsArray = jsonParent.buildValue(systelab::json::ARRAY_TYPE);

		for (const auto& fixtureStep : fixtureSteps)
		{
			auto jsonFixtureStep = jsonParent.buildValue(systelab::json::OBJECT_TYPE);
			addFixtureStepToJSON(fixtureStep, *jsonFixtureStep);
			jsonFixtureStepsArray->addArrayValue(std::move(jsonFixtureStep));
		}

		jsonParent.addMember(fieldName, std::move(jsonFixtureStepsArray));
	}

	void ContainerJSONSerializer::addFixtureStepToJSON(const model::FixtureStep& fixtureStep, systelab::json::IJSONValue& jsonStep) const
	{
		jsonStep.addMember("name", fixtureStep.getName());
		jsonStep.addMember("status", translateStatusToString(fixtureStep.getStatus()));
		jsonStep.addMember("stage", translateStageToString(fixtureStep.getStage()));

		// Convert timestamps from seconds to milliseconds
		jsonStep.addMember("start", fixtureStep.getStart() * 1000LL);
		jsonStep.addMember("stop", fixtureStep.getStop() * 1000LL);

		// Add parameters if present
		const auto& parameters = fixtureStep.getParameters();
		if (parameters.size() > 0)
		{
			auto jsonParametersArray = jsonStep.buildValue(systelab::json::ARRAY_TYPE);
			for (const auto& parameter : parameters)
			{
				auto jsonParameter = jsonStep.buildValue(systelab::json::OBJECT_TYPE);
				jsonParameter->addMember("name", parameter.getName());
				jsonParameter->addMember("value", parameter.getValue());
				jsonParametersArray->addArrayValue(std::move(jsonParameter));
			}
			jsonStep.addMember("parameters", std::move(jsonParametersArray));
		}

		// Add attachments if present
		const auto& attachments = fixtureStep.getAttachments();
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

		// Add nested steps if present (recursive structure)
		unsigned int nSteps = fixtureStep.getStepCount();
		if (nSteps > 0)
		{
			auto jsonStepsArray = jsonStep.buildValue(systelab::json::ARRAY_TYPE);
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto nestedStep = fixtureStep.getStep(i);
				auto jsonNestedStep = jsonStep.buildValue(systelab::json::OBJECT_TYPE);
				addFixtureStepToJSON(*nestedStep, *jsonNestedStep);
				jsonStepsArray->addArrayValue(std::move(jsonNestedStep));
			}
			jsonStep.addMember("steps", std::move(jsonStepsArray));
		}
	}

	std::string ContainerJSONSerializer::translateStatusToString(model::Status status) const
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

	std::string ContainerJSONSerializer::translateStageToString(model::Stage stage) const
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
