#pragma once

#include <memory>
#include <string>

namespace allure_cpp { namespace adapters { namespace googletest {

class AllureGTest
{
public:
    explicit AllureGTest(const std::string& outputFolder = "allure-results");
    ~AllureGTest();

    AllureGTest(const AllureGTest&) = delete;
    AllureGTest& operator=(const AllureGTest&) = delete;
    AllureGTest(AllureGTest&&) = delete;
    AllureGTest& operator=(AllureGTest&&) = delete;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}}} // namespace allure_cpp::adapters::googletest

namespace allure {
using AllureGTest = allure_cpp::adapters::googletest::AllureGTest;
}
