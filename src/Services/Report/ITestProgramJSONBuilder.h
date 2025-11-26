#pragma once

#include <string>

namespace allure { namespace model {
	class TestProgram;
}} // namespace allure::service

namespace allure { namespace service {

	class ITestProgramJSONBuilder
	{
	public:
		virtual ~ITestProgramJSONBuilder() = default;

		virtual void buildJSONFiles(const model::TestProgram&) const = 0;
		virtual void buildMetadataFiles(const model::TestProgram&) const = 0;
	};

}} // namespace allure::service
