#pragma once

#include "IContainerJSONSerializer.h"

#include <memory>
#include <vector>


namespace systelab { namespace json {
	class IJSONAdapter;
	class IJSONValue;
}}

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
		ContainerJSONSerializer(std::unique_ptr<systelab::json::IJSONAdapter>);
		virtual ~ContainerJSONSerializer() = default;

		std::string serialize(const model::Container&) const override;

	private:
		void addContainerToJSON(const model::Container&, systelab::json::IJSONValue&) const;
		void addChildrenToJSON(const std::vector<std::string>&, systelab::json::IJSONValue&) const;
		void addFixtureStepsToJSON(const std::vector<model::FixtureStep>&, systelab::json::IJSONValue&, const std::string& fieldName) const;
		void addFixtureStepToJSON(const model::FixtureStep&, systelab::json::IJSONValue&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;

	private:
		std::unique_ptr<systelab::json::IJSONAdapter> m_jsonAdapter;
	};

}} // namespace allure_cpp::service
