#include "GTestAllureUtilities/Framework/Adapters/GoogleTest/GTestMetadata.h"


namespace allure_cpp {
namespace adapters {
namespace googletest {

	GTestMetadata::GTestMetadata(const ::testing::TestInfo& testInfo)
		: m_testInfo(testInfo)
	{
	}

	std::string GTestMetadata::getTestName() const
	{
		return m_testInfo.name();
	}

	std::string GTestMetadata::getSuiteName() const
	{
		// GoogleTest changed API from test_case_name() to test_suite_name()
		// in version 1.10. Use the newer API (test_suite_name) since we're
		// using GoogleTest 1.14.0
		return m_testInfo.test_suite_name();
	}

	std::string GTestMetadata::getFullName() const
	{
		// Format: "SuiteName.TestName"
		return getSuiteName() + "." + getTestName();
	}

	std::string GTestMetadata::getFileName() const
	{
		const char* file = m_testInfo.file();
		return file ? file : "";
	}

	int GTestMetadata::getLineNumber() const
	{
		return m_testInfo.line();
	}

	bool GTestMetadata::hasTypeParameter() const
	{
		return m_testInfo.type_param() != nullptr;
	}

	bool GTestMetadata::hasValueParameter() const
	{
		return m_testInfo.value_param() != nullptr;
	}

	std::string GTestMetadata::getTypeParameter() const
	{
		const char* typeParam = m_testInfo.type_param();
		return typeParam ? typeParam : "";
	}

	std::string GTestMetadata::getValueParameter() const
	{
		const char* valueParam = m_testInfo.value_param();
		return valueParam ? valueParam : "";
	}

}}} // namespace allure_cpp::adapters::googletest
