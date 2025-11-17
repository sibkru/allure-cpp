#pragma once

#include "ITestCaseJSONSerializer.h"

#include <memory>
#include <vector>


namespace systelab { namespace json {
	class IJSONAdapter;
	class IJSONValue;
}}

namespace allure_cpp { namespace model {
	class Attachment;
	class Label;
	class Link;
	class Parameter;
	class Step;
	class TestCase;
	enum class Status;
	enum class Stage;
}} // namespace allure_cpp::model

namespace allure_cpp { namespace service {

	class TestCaseJSONSerializer : public ITestCaseJSONSerializer
	{
	public:
		TestCaseJSONSerializer(std::unique_ptr<systelab::json::IJSONAdapter>);
		virtual ~TestCaseJSONSerializer() = default;

		std::string serialize(const model::TestCase&) const override;

	private:
		void addTestCaseToJSON(const model::TestCase&, systelab::json::IJSONValue&) const;
		void addLabelsToJSON(const std::vector<model::Label>&, systelab::json::IJSONValue&) const;
		void addParametersToJSON(const std::vector<model::Parameter>&, systelab::json::IJSONValue&) const;
		void addLinksToJSON(const std::vector<model::Link>&, systelab::json::IJSONValue&) const;
		void addAttachmentsToJSON(const std::vector<model::Attachment>&, systelab::json::IJSONValue&) const;
		void addStepsToJSON(const model::TestCase&, systelab::json::IJSONValue&) const;
		void addStepToJSON(const model::Step*, systelab::json::IJSONValue&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;

	private:
		std::unique_ptr<systelab::json::IJSONAdapter> m_jsonAdapter;
	};

}} // namespace allure_cpp::service
