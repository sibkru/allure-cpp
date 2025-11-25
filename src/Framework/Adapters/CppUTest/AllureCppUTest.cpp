#include "Framework/Adapters/CppUTest/AllureCppUTest.h"

#include "AllureAPI.h"

namespace allure_cpp { namespace adapters { namespace cpputest {

class AllureCppUTest::Impl
{
public:
	explicit Impl(const std::string& outputFolder)
	{
		AllureAPI::setOutputFolder(outputFolder);
		AllureAPI::setFrameworkName("CppUTest");
	}

	~Impl()
	{
		AllureAPI::cleanup();
	}
};

AllureCppUTest::AllureCppUTest(const std::string& outputFolder)
	: m_impl(std::make_unique<Impl>(outputFolder))
{
}

AllureCppUTest::~AllureCppUTest() = default;

}}} // namespace allure_cpp::adapters::cpputest
