#pragma once

#include "ITestSuiteJSONSerializer.h"

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

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
		TestSuiteJSONSerializer() = default;
		virtual ~TestSuiteJSONSerializer() = default;

		std::string serialize(const model::TestSuite&) const override;

	private:
		void addTestSuiteToJSON(const model::TestSuite&, json&) const;
		void addLabelsToJSON(const model::TestSuite&, json&) const;
		void addLinksToJSON(const std::vector<model::Link>&, json&) const;
		void addTestCasesToJSON(const std::vector<model::TestCase>&, json&) const;
		void addTestCaseStepsToJSON(const model::TestCase& testCase, json&) const;

		std::string translateStatusToString(model::Status) const;
		std::string translateStageToString(model::Stage) const;
	};

}} // namespace allure_cpp::service
