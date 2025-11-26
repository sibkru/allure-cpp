#pragma once

#include "ITestCaseJSONSerializer.h"

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

namespace allure { namespace model {
	class Attachment;
	class Label;
	class Link;
	class Parameter;
	class Step;
	class TestCase;
	enum class Status;
	enum class Stage;
}} // namespace allure::model

namespace allure { namespace service {

	class TestCaseJSONSerializer : public ITestCaseJSONSerializer
	{
	public:
		TestCaseJSONSerializer() = default;
		virtual ~TestCaseJSONSerializer() = default;

		std::string serialize(const model::TestCase&) const override;

	private:
		void addTestCaseToJSON(const model::TestCase&, json&) const;
		void addLabelsToJSON(const std::vector<model::Label>&, json&) const;
		void addParametersToJSON(const std::vector<model::Parameter>&, json&) const;
		void addLinksToJSON(const std::vector<model::Link>&, json&) const;
		void addAttachmentsToJSON(const std::vector<model::Attachment>&, json&) const;
		void addStepsToJSON(const model::TestCase&, json&) const;
		void addStepToJSON(const model::Step*, json&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;
	};

}} // namespace allure::service
