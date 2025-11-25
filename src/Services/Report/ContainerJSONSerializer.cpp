#include "ContainerJSONSerializer.h"

#include "Model/Container.h"


namespace allure_cpp { namespace service {

	std::string ContainerJSONSerializer::serialize(const model::Container& container) const
	{
		json j = json::object();
		addContainerToJSON(container, j);
		return j.dump();
	}

	void ContainerJSONSerializer::addContainerToJSON(const model::Container& container, json& j) const
	{
		// Required fields
		j["uuid"] = container.getUUID();

		// Optional name field
		if (!container.getName().empty())
		{
			j["name"] = container.getName();
		}

		// Timestamps are already in milliseconds (from TimeService)
		j["start"] = container.getStart();
		j["stop"] = container.getStop();

		// Add children array (UUIDs of test case result files)
		addChildrenToJSON(container.getChildren(), j);

		// Add befores and afters (fixture steps)
		addFixtureStepsToJSON(container.getBefores(), j, "befores");
		addFixtureStepsToJSON(container.getAfters(), j, "afters");
	}

	void ContainerJSONSerializer::addChildrenToJSON(const std::vector<std::string>& children, json& j) const
	{
		if (children.size() > 0)
		{
			json childrenArray = json::array();
			for (const auto& childUUID : children)
			{
				childrenArray.push_back(childUUID);
			}
			j["children"] = childrenArray;
		}
	}

	void ContainerJSONSerializer::addFixtureStepsToJSON(const std::vector<model::FixtureStep>& fixtureSteps,
	                                                     json& j,
	                                                     const std::string& fieldName) const
	{
		// Always include befores/afters arrays, even if empty (required by Allure 2)
		json fixtureStepsArray = json::array();

		for (const auto& fixtureStep : fixtureSteps)
		{
			json fixtureStepObj = json::object();
			addFixtureStepToJSON(fixtureStep, fixtureStepObj);
			fixtureStepsArray.push_back(fixtureStepObj);
		}

		j[fieldName] = fixtureStepsArray;
	}

	void ContainerJSONSerializer::addFixtureStepToJSON(const model::FixtureStep& fixtureStep, json& j) const
	{
		j["name"] = fixtureStep.getName();
		j["status"] = translateStatusToString(fixtureStep.getStatus());
		j["stage"] = translateStageToString(fixtureStep.getStage());

		// Timestamps are already in milliseconds (from TimeService)
		j["start"] = fixtureStep.getStart();
		j["stop"] = fixtureStep.getStop();

		// Add parameters if present
		const auto& parameters = fixtureStep.getParameters();
		if (parameters.size() > 0)
		{
			json parametersArray = json::array();
			for (const auto& parameter : parameters)
			{
				json paramObj = {
					{"name", parameter.getName()},
					{"value", parameter.getValue()}
				};
				parametersArray.push_back(paramObj);
			}
			j["parameters"] = parametersArray;
		}

		// Add attachments if present
		const auto& attachments = fixtureStep.getAttachments();
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

		// Add nested steps if present (recursive structure)
		unsigned int nSteps = fixtureStep.getStepCount();
		if (nSteps > 0)
		{
			json stepsArray = json::array();
			for (unsigned int i = 0; i < nSteps; i++)
			{
				auto nestedStep = fixtureStep.getStep(i);
				json nestedStepObj = json::object();
				addFixtureStepToJSON(*nestedStep, nestedStepObj);
				stepsArray.push_back(nestedStepObj);
			}
			j["steps"] = stepsArray;
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
