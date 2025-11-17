#pragma once

#include <string>

namespace allure_cpp { namespace model {
	class TestCase;
}} // namespace allure_cpp::model

namespace allure_cpp { namespace service {

	class ITestCaseJSONSerializer
	{
	public:
		virtual ~ITestCaseJSONSerializer() = default;

		virtual std::string serialize(const model::TestCase&) const = 0;
	};

}} // namespace allure_cpp::service
