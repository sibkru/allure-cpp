#include "TestProgram.h"


namespace allure_cpp { namespace model {

	TestProgram::TestProgram()
		:m_name("")
		,m_outputFolder("allure-results")
		,m_tmsLinksPattern("http://{}")
		,m_frameworkName("unknown")
		,m_format(Format::DEFAULT)
	{
	}

	TestProgram::TestProgram(const TestProgram& other)
		:m_name(other.m_name)
		,m_outputFolder(other.m_outputFolder)
		,m_tmsLinksPattern(other.m_tmsLinksPattern)
		,m_frameworkName(other.m_frameworkName)
		,m_format(other.m_format)
		,m_testSuites(other.m_testSuites)
	{
	}

	std::string TestProgram::getName() const
	{
		return m_name;
	}

	void TestProgram::setName(const std::string& name)
	{
		m_name = name;
	}

	std::string TestProgram::getOutputFolder() const
	{
		return m_outputFolder;
	}

	void TestProgram::setOutputFolder(const std::string& outputFolder)
	{
		m_outputFolder = outputFolder;
	}

	std::string TestProgram::getTMSLinksPattern() const
	{
		return m_tmsLinksPattern;
	}

	void TestProgram::setTMSLinksPattern(const std::string& tmsLinksPattern)
	{
		m_tmsLinksPattern = tmsLinksPattern;
	}

	std::string TestProgram::getFrameworkName() const
	{
		return m_frameworkName;
	}

	void TestProgram::setFrameworkName(const std::string& frameworkName)
	{
		m_frameworkName = frameworkName;
	}

	Format TestProgram::getFormat() const
	{
		return m_format;
	}

	void TestProgram::setFormat(Format format)
	{
		m_format = format;
	}

	size_t TestProgram::getTestSuitesCount() const
	{
		return m_testSuites.size();
	}

	const TestSuite& TestProgram::getTestSuite(unsigned int index) const
	{
		return m_testSuites[index];
	}

	TestSuite& TestProgram::getTestSuite(unsigned int index)
	{
		return m_testSuites[index];
	}

	void TestProgram::addTestSuite(const TestSuite& testSuite)
	{
		m_testSuites.push_back(testSuite);
	}

	void TestProgram::clearTestSuites()
	{
		m_testSuites.clear();
	}

	TestSuite* TestProgram::getRunningTestSuite()
	{
		for (auto& testSuite : m_testSuites)
		{
			if (testSuite.getStage() == Stage::RUNNING)
			{
				return &testSuite;
			}
		}
		return nullptr;
	}

	const TestSuite* TestProgram::getRunningTestSuite() const
	{
		for (const auto& testSuite : m_testSuites)
		{
			if (testSuite.getStage() == Stage::RUNNING)
			{
				return &testSuite;
			}
		}
		return nullptr;
	}

	TestCase* TestProgram::getRunningTestCase()
	{
		TestSuite* runningSuite = getRunningTestSuite();
		if (!runningSuite)
		{
			return nullptr;
		}

		auto& testCases = runningSuite->getTestCases();
		for (auto& testCase : testCases)
		{
			if (testCase.getStage() == Stage::RUNNING)
			{
				return &testCase;
			}
		}
		return nullptr;
	}

	const TestCase* TestProgram::getRunningTestCase() const
	{
		const TestSuite* runningSuite = getRunningTestSuite();
		if (!runningSuite)
		{
			return nullptr;
		}

		const auto& testCases = runningSuite->getTestCases();
		for (const auto& testCase : testCases)
		{
			if (testCase.getStage() == Stage::RUNNING)
			{
				return &testCase;
			}
		}
		return nullptr;
	}

	TestProgram& TestProgram::operator= (const TestProgram& other)
	{
		m_name = other.m_name;
		m_outputFolder = other.m_outputFolder;
		m_tmsLinksPattern = other.m_tmsLinksPattern;
		m_frameworkName = other.m_frameworkName;
		m_format = other.m_format;
		m_testSuites = other.m_testSuites;
		return *this;
	}

	bool operator== (const TestProgram& lhs, const TestProgram& rhs)
	{
		return (lhs.m_name == rhs.m_name) &&
			   (lhs.m_outputFolder == rhs.m_outputFolder) &&
			   (lhs.m_tmsLinksPattern == rhs.m_tmsLinksPattern) &&
			   (lhs.m_frameworkName == rhs.m_frameworkName) &&
			   (lhs.m_testSuites == rhs.m_testSuites) &&
			   (lhs.m_format == rhs.m_format);
	}

	bool operator!= (const TestProgram& lhs, const TestProgram& rhs)
	{
		return !(lhs == rhs);
	}

}} // namespace allure_cpp::model
