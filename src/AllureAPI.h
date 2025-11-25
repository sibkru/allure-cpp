#pragma once

#include "Model/TestProgram.h"
#include "Model/Format.h"
#include "Framework/ITestStatusProvider.h"

#include <functional>
#include <memory>


namespace allure_cpp {
	class ITestFrameworkAdapter;
}

namespace allure_cpp {

	namespace service {
		class IServicesFactory;
	}

	class AllureAPI
	{
	public:
		static std::unique_ptr<allure_cpp::ITestStatusProvider> getStatusProvider();
		static void setFrameworkAdapter(std::shared_ptr<allure_cpp::ITestFrameworkAdapter>);

		static model::TestProgram& getTestProgram();
		static void cleanup();
		static void setTestProgramName(const std::string&);
		static void setOutputFolder(const std::string&);
		static void setTMSLinksPattern(const std::string&);
		static void setFrameworkName(const std::string&);
		static void setFormat(model::Format format);

		static void setTMSId(const std::string&);
		static void setTestSuiteName(const std::string&);
		static void setTestSuiteDescription(const std::string&);
		static void setTestSuiteEpic(const std::string&);
		static void setTestSuiteSeverity(const std::string&);
		static void setTestSuiteLabel(const std::string& name, const std::string& value);

		static void setTestCaseName(const std::string&);
		static void setTestCaseDescription(const std::string& description);
		static void setTestCaseDescriptionHtml(const std::string& descriptionHtml);

		static void addAction(const std::string& name, std::function<void()>);
		static void addExpectedResult(const std::string& name, std::function<void()>);

		// Substep support - allows nesting steps within actions/expected results
		static void beginSubStep(const std::string& name);
		static void endSubStep();

		// Mark test with statusDetails flags
		static void markTestAsFlaky();
		static void markTestAsKnown();
		static void markTestAsMuted();

		// Links - associate test with issue tracker, test management system, etc.
		static void addLink(const std::string& name, const std::string& url, const std::string& type = "custom");
		static void addIssue(const std::string& name, const std::string& url);
		static void addTmsLink(const std::string& name, const std::string& url);

		// Attachments - add files, screenshots, logs to test reports
		static void addAttachment(const std::string& name, const std::string& type, const void* data, size_t size);
		static void addTextAttachment(const std::string& name, const std::string& content);
		static void addFileAttachment(const std::string& name, const std::string& filePath);

		// Parameters - add test parameters (for parametric tests or runtime parameters)
		static void addParameter(const std::string& name, const std::string& value);
		static void addParameter(const std::string& name, const std::string& value, bool excluded, const std::string& mode = "default");
		static void addMaskedParameter(const std::string& name, const std::string& value);
		static void addHiddenParameter(const std::string& name, const std::string& value);

		// Labels - add custom labels for test organization
		static void addLabel(const std::string& name, const std::string& value);
		static void addEpic(const std::string& epic);
		static void addFeature(const std::string& feature);
		static void addStory(const std::string& story);
		static void addSeverity(const std::string& severity);
		static void addOwner(const std::string& owner);
		static void addTag(const std::string& tag);

	private:
		static void addStep(const std::string& name, bool isAction, std::function<void()>);
		static service::IServicesFactory* getServicesFactory();

	private:
		static model::TestProgram m_testProgram;
		static std::unique_ptr<service::IServicesFactory> m_servicesFactory;
		static std::shared_ptr<allure_cpp::ITestFrameworkAdapter> m_frameworkAdapter;
	};

} // namespace allure_cpp
