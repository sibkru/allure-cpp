#pragma once

#include "ITestProgramJSONBuilder.h"

#include <memory>


namespace allure_cpp {
	namespace model {
		class Container;
		class TestSuite;
	}
	namespace service {

	class IFileService;
	class ITestCaseJSONSerializer;
	class IContainerJSONSerializer;
	class ITestSuiteJSONSerializer;

	class TestProgramJSONBuilder : public ITestProgramJSONBuilder
	{
	public:
		TestProgramJSONBuilder(std::unique_ptr<ITestCaseJSONSerializer>,
							   std::unique_ptr<IContainerJSONSerializer>,
							   std::unique_ptr<IFileService>);
		virtual ~TestProgramJSONBuilder() = default;

		virtual void buildJSONFiles(const model::TestProgram&) const;

	private:
		model::Container createContainerFromTestSuite(const model::TestSuite&) const;
		void generateMetadataFiles(const model::TestProgram&) const;
		void generateEnvironmentProperties(const std::string& outputFolder) const;
		void generateExecutorJson(const std::string& outputFolder) const;
		void generateCategoriesJson(const std::string& outputFolder) const;

	private:
		std::unique_ptr<ITestCaseJSONSerializer> m_testCaseJSONSerializer;
		std::unique_ptr<IContainerJSONSerializer> m_containerJSONSerializer;
		std::unique_ptr<IFileService> m_fileService;
	};

}} // namespace allure_cpp::service
