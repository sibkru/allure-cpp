#pragma once

#include <string>

namespace allure { namespace model {
	class TestCase;
}} // namespace allure::model

namespace allure { namespace service {

	class ITestCaseJSONSerializer
	{
	public:
		virtual ~ITestCaseJSONSerializer() = default;

		virtual std::string serialize(const model::TestCase&) const = 0;
	};

}} // namespace allure::service
