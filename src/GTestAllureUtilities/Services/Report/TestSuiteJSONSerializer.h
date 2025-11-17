#pragma once

#include "ITestSuiteJSONSerializer.h"

#include <memory>
#include <vector>


namespace systelab { namespace json {
	class IJSONAdapter;
	class IJSONValue;
}}

namespace allure_cpp { namespace model {
	class Action;
	class ExpectedResult;
	class Label;
	class Link;
	class Parameter;
	class TestCase;
	class TestSuite;
	enum class Status;
	enum class Stage;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class TestSuiteJSONSerializer : public ITestSuiteJSONSerializer
	{
	public:
		TestSuiteJSONSerializer(std::unique_ptr<systelab::json::IJSONAdapter>);
		virtual ~TestSuiteJSONSerializer() = default;

		std::string serialize(const model::TestSuite&) const override;

	private:
		void addTestSuiteToJSON(const model::TestSuite&, systelab::json::IJSONValue&) const;
		void addLabelsToJSON(const model::TestSuite&, systelab::json::IJSONValue&) const;
		void addLinksToJSON(const std::vector<model::Link>&, systelab::json::IJSONValue&) const;
		void addTestCasesToJSON(const std::vector<model::TestCase>&, systelab::json::IJSONValue&) const;
		void addTestCaseStepsToJSON(const model::TestCase& testCase, systelab::json::IJSONValue&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;

	private:
		std::unique_ptr<systelab::json::IJSONAdapter> m_jsonAdapter;
	};

}} // namespace allure_cpp::service
