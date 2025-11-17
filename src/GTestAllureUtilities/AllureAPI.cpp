#include "AllureAPI.h"

#include "Model/TestProperty.h"
#include "Model/Link.h"
#include "Model/Attachment.h"
#include "Model/Parameter.h"
#include "Model/Label.h"
#include "Services/ServicesFactory.h"
#include "Services/EventHandlers/ITestStepStartEventHandler.h"
#include "Services/EventHandlers/ITestStepEndEventHandler.h"
#include "Services/GoogleTest/IGTestStatusChecker.h"
#include "Services/Property/ITestCasePropertySetter.h"
#include "Services/Property/ITestSuitePropertySetter.h"
#include "Services/System/IUUIDGeneratorService.h"
#include "Framework/Adapters/GoogleTest/GTestAdapter.h"
#include "Framework/Adapters/GoogleTest/GTestStatusProvider.h"
#include "Framework/ITestFrameworkAdapter.h"
#include "Framework/ITestStatusProvider.h"

#include <fstream>
#include <vector>


namespace allure_cpp {

	model::TestProgram AllureAPI::m_testProgram = model::TestProgram();
	service::IServicesFactory* AllureAPI::m_servicesFactory = new service::ServicesFactory(m_testProgram);
	std::unique_ptr<allure_cpp::ITestFrameworkAdapter> AllureAPI::m_frameworkAdapter = nullptr;

	std::unique_ptr<::testing::TestEventListener> AllureAPI::buildListener()
	{
		return getServicesFactory()->buildGTestEventListener();
	}

	void AllureAPI::initializeGoogleTest()
	{
		auto servicesFactory = getServicesFactory();

		// Create the GoogleTest adapter using the new Phase 2 classes
		auto adapter = std::make_unique<allure_cpp::adapters::googletest::GTestAdapter>(
			servicesFactory->buildTestProgramStartEventHandler().release(),
			servicesFactory->buildTestProgramEndEventHandler().release(),
			servicesFactory->buildTestSuiteStartEventHandler().release(),
			servicesFactory->buildTestSuiteEndEventHandler().release(),
			servicesFactory->buildTestCaseStartEventHandler().release(),
			servicesFactory->buildTestCaseEndEventHandler().release()
		);

		// Initialize the adapter (registers with GoogleTest)
		adapter->initialize();

		// Store the adapter for later use (e.g., for status provider)
		m_frameworkAdapter = std::move(adapter);
	}

	std::unique_ptr<allure_cpp::ITestStatusProvider> AllureAPI::getStatusProvider()
	{
		if (m_frameworkAdapter)
		{
			return m_frameworkAdapter->createStatusProvider();
		}

		// Fallback: If using old API, create GoogleTest status provider directly
		return std::make_unique<allure_cpp::adapters::googletest::GTestStatusProvider>();
	}

	model::TestProgram& AllureAPI::getTestProgram()
	{
		return m_testProgram;
	}

	void AllureAPI::setTestProgramName(const std::string& name)
	{
		m_testProgram.setName(name);
	}

	void AllureAPI::setOutputFolder(const std::string& outputFolder)
	{
		m_testProgram.setOutputFolder(outputFolder);
	}

	void AllureAPI::setTMSLinksPattern(const std::string& tmsLinkPattern)
	{
		m_testProgram.setTMSLinksPattern(tmsLinkPattern);
	}

	void AllureAPI::setFormat(model::Format format)
	{
		m_testProgram.setFormat(format);
	}

	void AllureAPI::setTMSId(const std::string& value)
	{
		auto testSuitePropertySetter = getServicesFactory()->buildTestSuitePropertySetter();
		testSuitePropertySetter->setProperty(model::test_property::TMS_ID_PROPERTY, value);
	}

	void AllureAPI::setTestSuiteName(const std::string& name)
	{
		setTestSuiteLabel(model::test_property::NAME_PROPERTY, name);
	}

	void AllureAPI::setTestSuiteDescription(const std::string& description)
	{
		setTestSuiteLabel(model::test_property::FEATURE_PROPERTY, description);
	}

	void AllureAPI::setTestSuiteEpic(const std::string& epic)
	{
		setTestSuiteLabel(model::test_property::EPIC_PROPERTY, epic);
	}

	void AllureAPI::setTestSuiteSeverity(const std::string& severity)
	{
		setTestSuiteLabel(model::test_property::SEVERITY_PROPERTY, severity);
	}

	void AllureAPI::setTestSuiteLabel(const std::string& name, const std::string& value)
	{
		auto testSuitePropertySetter = getServicesFactory()->buildTestSuitePropertySetter();
		testSuitePropertySetter->setProperty(name, value);
	}

	void AllureAPI::setTestCaseName(const std::string& name)
	{
		auto testCasePropertySetter = getServicesFactory()->buildTestCasePropertySetter();
		testCasePropertySetter->setProperty(model::test_property::NAME_PROPERTY, name);
	}

	void AllureAPI::addAction(const std::string& name, std::function<void()> actionFunction)
	{
		addStep(name, true, actionFunction);
	}

	void AllureAPI::addExpectedResult(const std::string& name, std::function<void()> verificationFunction)
	{
		addStep(name, false, verificationFunction);
	}

	void AllureAPI::addStep(const std::string& name, bool isAction, std::function<void()> stepFunction)
	{
		auto stepStartEventHandler = getServicesFactory()->buildTestStepStartEventHandler();
		stepStartEventHandler->handleTestStepStart(name, isAction);

		stepFunction();

		auto statusChecker = getServicesFactory()->buildGTestStatusChecker();
		auto currentStatus = statusChecker->getCurrentTestStatus();

		auto stepEndEventHandler = getServicesFactory()->buildTestStepEndEventHandler();
		stepEndEventHandler->handleTestStepEnd(currentStatus);
	}

	void AllureAPI::beginSubStep(const std::string& name)
	{
		auto stepStartEventHandler = getServicesFactory()->buildTestStepStartEventHandler();
		stepStartEventHandler->handleTestStepStart(name, true);
	}

	void AllureAPI::endSubStep()
	{
		auto statusChecker = getServicesFactory()->buildGTestStatusChecker();
		auto currentStatus = statusChecker->getCurrentTestStatus();

		auto stepEndEventHandler = getServicesFactory()->buildTestStepEndEventHandler();
		stepEndEventHandler->handleTestStepEnd(currentStatus);
	}

	void AllureAPI::markTestAsFlaky()
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			testCase->setStatusFlaky(true);
		}
	}

	void AllureAPI::markTestAsKnown()
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			testCase->setStatusKnown(true);
		}
	}

	void AllureAPI::markTestAsMuted()
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			testCase->setStatusMuted(true);
		}
	}

	void AllureAPI::setTestCaseDescription(const std::string& description)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			testCase->setDescription(description);
		}
	}

	void AllureAPI::setTestCaseDescriptionHtml(const std::string& descriptionHtml)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			testCase->setDescriptionHtml(descriptionHtml);
		}
	}

	void AllureAPI::addLink(const std::string& name, const std::string& url, const std::string& type)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			model::Link link;
			link.setName(name);
			link.setURL(url);
			link.setType(type);
			testCase->addLink(link);
		}
	}

	void AllureAPI::addIssue(const std::string& name, const std::string& url)
	{
		addLink(name, url, "issue");
	}

	void AllureAPI::addTmsLink(const std::string& name, const std::string& url)
	{
		addLink(name, url, "tms");
	}

	void AllureAPI::addAttachment(const std::string& name, const std::string& type, const void* data, size_t size)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase && data && size > 0)
		{
			// Generate unique filename for attachment
			auto uuidGenerator = getServicesFactory()->buildUUIDGeneratorService();
			auto uuid = uuidGenerator->generateUUID();
			std::string extension;

			// Determine extension from MIME type
			if (type.find("text/") == 0) extension = ".txt";
			else if (type.find("image/png") == 0) extension = ".png";
			else if (type.find("image/jpeg") == 0) extension = ".jpg";
			else if (type.find("application/json") == 0) extension = ".json";
			else if (type.find("application/xml") == 0) extension = ".xml";
			else extension = ".dat";

			std::string filename = uuid + "-attachment" + extension;

			// Write attachment file to output folder
			std::string outputFolder = m_testProgram.getOutputFolder();
			std::string filepath = outputFolder + "/" + filename;

			std::ofstream file(filepath, std::ios::binary);
			if (file.is_open())
			{
				file.write(static_cast<const char*>(data), size);
				file.close();

				// Add attachment reference to test case
				model::Attachment attachment;
				attachment.setName(name);
				attachment.setSource(filename);
				attachment.setType(type);
				testCase->addAttachment(attachment);
			}
		}
	}

	void AllureAPI::addTextAttachment(const std::string& name, const std::string& content)
	{
		addAttachment(name, "text/plain", content.c_str(), content.size());
	}

	void AllureAPI::addFileAttachment(const std::string& name, const std::string& filePath)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			// Read file content
			std::ifstream file(filePath, std::ios::binary | std::ios::ate);
			if (file.is_open())
			{
				std::streamsize size = file.tellg();
				file.seekg(0, std::ios::beg);

				std::vector<char> buffer(size);
				if (file.read(buffer.data(), size))
				{
					// Determine MIME type from file extension
					std::string type = "application/octet-stream";
					size_t dotPos = filePath.find_last_of('.');
					if (dotPos != std::string::npos)
					{
						std::string ext = filePath.substr(dotPos);
						if (ext == ".txt") type = "text/plain";
						else if (ext == ".png") type = "image/png";
						else if (ext == ".jpg" || ext == ".jpeg") type = "image/jpeg";
						else if (ext == ".json") type = "application/json";
						else if (ext == ".xml") type = "application/xml";
						else if (ext == ".log") type = "text/plain";
					}

					addAttachment(name, type, buffer.data(), size);
				}
			}
		}
	}

	void AllureAPI::addParameter(const std::string& name, const std::string& value)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			model::Parameter param;
			param.setName(name);
			param.setValue(value);
			param.setExcluded(false);
			param.setMode("default");
			testCase->addParameter(param);
		}
	}

	void AllureAPI::addParameter(const std::string& name, const std::string& value, bool excluded, const std::string& mode)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			model::Parameter param;
			param.setName(name);
			param.setValue(value);
			param.setExcluded(excluded);
			param.setMode(mode);
			testCase->addParameter(param);
		}
	}

	void AllureAPI::addMaskedParameter(const std::string& name, const std::string& value)
	{
		addParameter(name, value, false, "masked");
	}

	void AllureAPI::addHiddenParameter(const std::string& name, const std::string& value)
	{
		addParameter(name, value, false, "hidden");
	}

	void AllureAPI::addLabel(const std::string& name, const std::string& value)
	{
		auto* testCase = getTestProgram().getRunningTestCase();
		if (testCase)
		{
			model::Label label;
			label.setName(name);
			label.setValue(value);
			testCase->addLabel(label);
		}
	}

	void AllureAPI::addEpic(const std::string& epic)
	{
		addLabel("epic", epic);
	}

	void AllureAPI::addFeature(const std::string& feature)
	{
		addLabel("feature", feature);
	}

	void AllureAPI::addStory(const std::string& story)
	{
		addLabel("story", story);
	}

	void AllureAPI::addSeverity(const std::string& severity)
	{
		addLabel("severity", severity);
	}

	void AllureAPI::addOwner(const std::string& owner)
	{
		addLabel("owner", owner);
	}

	void AllureAPI::addTag(const std::string& tag)
	{
		addLabel("tag", tag);
	}

	service::IServicesFactory* AllureAPI::getServicesFactory()
	{
		auto configuredServicesFactoryInstance = service::ServicesFactory::getInstance();
		if (configuredServicesFactoryInstance)
		{
			return configuredServicesFactoryInstance;
		}
		else
		{
			return m_servicesFactory;
		}
	}

} // namespace allure_cpp
