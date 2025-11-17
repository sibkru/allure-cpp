#pragma once

#include <string>

namespace allure_cpp { namespace model {
	class TestSuite;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITestSuiteJSONSerializer
	{
	public:
		virtual ~ITestSuiteJSONSerializer() = default;

		virtual std::string serialize(const model::TestSuite&) const = 0;
	};

}} // namespace allure_cpp::service
