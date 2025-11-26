#pragma once

#include <CppUTest/TestOutput.h>
#include <string>

namespace allure {
namespace adapters {
namespace cpputest {

class AllureCppUTestOutput : public TestOutput
{
public:
    AllureCppUTestOutput();
    ~AllureCppUTestOutput() override;

    void printFailure(const TestFailure& failure) override;

    static std::string getAndClearFailureMessage();

private:
    void printBuffer(const char* s) override;
    void flush() override;
    static std::string m_failureMessage;
};

} // namespace cpputest
} // namespace adapters
} // namespace allure
