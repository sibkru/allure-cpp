#include "Framework/Adapters/CppUTest/CppUTestMetadata.h"


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	CppUTestMetadata::CppUTestMetadata(const UtestShell& test)
		: m_test(test)
	{
	}

	std::string CppUTestMetadata::getTestName() const
	{
		return m_test.getName().asCharString();
	}

	std::string CppUTestMetadata::getSuiteName() const
	{
		return m_test.getGroup().asCharString();
	}

	std::string CppUTestMetadata::getFullName() const
	{
		std::string suiteName = getSuiteName();
		std::string testName = getTestName();

		if (suiteName.empty())
		{
			return testName;
		}

		return suiteName + "." + testName;
	}

	std::string CppUTestMetadata::getFileName() const
	{
		return m_test.getFile().asCharString();
	}

	int CppUTestMetadata::getLineNumber() const
	{
		return m_test.getLineNumber();
	}

}}} // namespace allure_cpp::adapters::cpputest
