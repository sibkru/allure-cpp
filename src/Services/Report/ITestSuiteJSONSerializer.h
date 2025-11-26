#pragma once

#include <string>

namespace allure { namespace model {
	class TestSuite;
}} // namespace allure::service

namespace allure { namespace service {

	class ITestSuiteJSONSerializer
	{
	public:
		virtual ~ITestSuiteJSONSerializer() = default;

		virtual std::string serialize(const model::TestSuite&) const = 0;
	};

}} // namespace allure::service
