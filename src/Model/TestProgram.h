#pragma once

#include "Format.h"
#include "TestSuite.h"


namespace allure { namespace model {

	class TestProgram
	{
	public:
		TestProgram();
		TestProgram(const TestProgram& other);
		~TestProgram() = default;

		std::string getName() const;
		void setName(const std::string&);

		std::string getOutputFolder() const;
		void setOutputFolder(const std::string&);

		std::string getTMSLinksPattern() const;
		void setTMSLinksPattern(const std::string&);

		std::string getExecutorBuildOrder() const;
		void setExecutorBuildOrder(const std::string&);

		std::string getExecutorBuildName() const;
		void setExecutorBuildName(const std::string&);

		std::string getFrameworkName() const;
		void setFrameworkName(const std::string&);

		Format getFormat() const;
		void setFormat(Format);

		size_t getTestSuitesCount() const;
		const TestSuite& getTestSuite(unsigned int index) const;
		TestSuite& getTestSuite(unsigned int index);
		void addTestSuite(const TestSuite&);
		void clearTestSuites();

		TestSuite* getRunningTestSuite();
		const TestSuite* getRunningTestSuite() const;

		TestCase* getRunningTestCase();
		const TestCase* getRunningTestCase() const;

		void setRunningTestSuite(TestSuite* testSuite);
		void setRunningTestCase(TestCase* testCase);

		TestProgram& operator= (const TestProgram&);
		friend bool operator== (const TestProgram& lhs, const TestProgram& rhs);
		friend bool operator!= (const TestProgram& lhs, const TestProgram& rhs);

	private:
		std::string m_name;
		std::string m_outputFolder;
		std::string m_tmsLinksPattern;
		std::string m_executorBuildOrder;
		std::string m_executorBuildName;
		std::string m_frameworkName;
		Format m_format;
		std::vector<TestSuite> m_testSuites;

		// Cache pointers to currently running test suite and test case for performance
		TestSuite* m_runningTestSuite = nullptr;
		TestCase* m_runningTestCase = nullptr;
	};

}} // namespace allure::model
