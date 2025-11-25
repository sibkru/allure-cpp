#pragma once

#include "IContainerJSONSerializer.h"

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

namespace allure_cpp { namespace model {
	class Container;
	class FixtureStep;
	enum class Status;
	enum class Stage;
}} // namespace allure_cpp::model

namespace allure_cpp { namespace service {

	class ContainerJSONSerializer : public IContainerJSONSerializer
	{
	public:
		ContainerJSONSerializer() = default;
		virtual ~ContainerJSONSerializer() = default;

		std::string serialize(const model::Container&) const override;

	private:
		void addContainerToJSON(const model::Container&, json&) const;
		void addChildrenToJSON(const std::vector<std::string>&, json&) const;
		void addFixtureStepsToJSON(const std::vector<model::FixtureStep>&, json&, const std::string& fieldName) const;
		void addFixtureStepToJSON(const model::FixtureStep&, json&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;
	};

}} // namespace allure_cpp::service
