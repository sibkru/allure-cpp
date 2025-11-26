#pragma once

#include "ITestProgramJSONBuilder.h"

#include <chrono>
#include <memory>
#include <string>


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
		virtual void buildMetadataFiles(const model::TestProgram&) const;

	private:
		model::Container createContainerFromTestSuite(const model::TestSuite&) const;
		void generateEnvironmentProperties(const std::string& outputFolder, const model::TestProgram& testProgram) const;
		void generateExecutorJson(const std::string& outputFolder, const model::TestProgram& testProgram) const;
		void generateCategoriesJson(const std::string& outputFolder) const;
		std::string resolveBuildOrder(const model::TestProgram& testProgram, long long currentMillis, const std::string& outputFolder) const;
		std::string resolveBuildName(const model::TestProgram& testProgram, const std::chrono::system_clock::time_point& now) const;
		std::string resolveExecutorName(const model::TestProgram& testProgram) const;
		std::string formatTimestamp(const std::chrono::system_clock::time_point& now) const;
		std::string buildCiName() const;
		std::string escapeJsonString(const std::string& value) const;
		bool shouldPreserveExistingExecutor(const std::string& filepath) const;
		bool fileExists(const std::string& filepath) const;
		std::string getEnvVar(const std::string& name) const;
		bool isTruthy(const std::string& value) const;
		bool isNumeric(const std::string& value) const;
		long long nextBuildOrderFromHistory(const std::string& outputFolder) const;
		std::string historyFilepath(const std::string& outputFolder) const;

	private:
		std::unique_ptr<ITestCaseJSONSerializer> m_testCaseJSONSerializer;
		std::unique_ptr<IContainerJSONSerializer> m_containerJSONSerializer;
		std::unique_ptr<IFileService> m_fileService;
	};

}} // namespace allure_cpp::service
