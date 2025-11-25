#pragma once

#include <string>

namespace allure_cpp { namespace model {
	class TestProgram;
}} // namespace allure_cpp::service

namespace allure_cpp { namespace service {

	class ITestProgramJSONBuilder
	{
	public:
		virtual ~ITestProgramJSONBuilder() = default;

		virtual void buildJSONFiles(const model::TestProgram&) const = 0;
		virtual void buildMetadataFiles(const model::TestProgram&) const = 0;
	};

}} // namespace allure_cpp::service
