#include "Framework/Adapters/CppUTest/AllureCppUTest.h"

#include "API/Core.h"

namespace allure { namespace adapters { namespace cpputest {

class AllureCppUTest::Impl
{
public:
	explicit Impl(const std::string& outputFolder)
	{
		auto& testProgram = detail::Core::instance().getTestProgram();
		testProgram.setOutputFolder(outputFolder);
		testProgram.setFrameworkName("CppUTest");
	}

	~Impl() = default;
};

AllureCppUTest::AllureCppUTest(const std::string& outputFolder)
	: m_impl(std::make_unique<Impl>(outputFolder))
{
}

AllureCppUTest::~AllureCppUTest() = default;

}}} // namespace allure::adapters::cpputest
