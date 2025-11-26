#include "AllureCppUTestOutput.h"
#include <CppUTest/TestFailure.h>
#include <sstream>

namespace allure {
namespace adapters {
namespace cpputest {

std::string AllureCppUTestOutput::m_failureMessage = "";

AllureCppUTestOutput::AllureCppUTestOutput() {}

AllureCppUTestOutput::~AllureCppUTestOutput() {}

void AllureCppUTestOutput::printFailure(const TestFailure& failure)
{
    // Reformat failure messages so expected/actual show clearly in Allure.
    auto raw = std::string(failure.getMessage().asCharString());

    // Normalize tabs to spaces for consistent rendering
    for (auto& ch : raw)
    {
        if (ch == '\t') ch = ' ';
    }

    std::string formatted;
	const std::string expectedTag = "expected <";
	const std::string butWasTag = "but was  <";
	const std::string actualTag = "actual <";
	const std::string notContainTag = "did not contain  <";

    auto expectedPos = raw.find(expectedTag);
    auto butWasPos = raw.find(butWasTag);

    if (expectedPos != std::string::npos && butWasPos != std::string::npos && butWasPos > expectedPos)
    {
        auto endExpected = raw.find('>', expectedPos + expectedTag.size());
        auto endActual = raw.find('>', butWasPos + butWasTag.size());

        if (endExpected != std::string::npos && endActual != std::string::npos)
        {
            auto expectedValue = raw.substr(expectedPos + expectedTag.size(), endExpected - (expectedPos + expectedTag.size()));
            auto actualValue = raw.substr(butWasPos + butWasTag.size(), endActual - (butWasPos + butWasTag.size()));

            std::ostringstream oss;
            oss << "Expected: " << expectedValue << "\n";
            oss << "Actual  : " << actualValue;

            // Append any additional context (e.g., difference markers) after the actual block
            auto detailStart = raw.find('\n', endActual);
            if (detailStart != std::string::npos)
            {
                auto detail = raw.substr(detailStart + 1);
                // Trim leading spaces from detail for tidier output
                while (!detail.empty() && (detail.front() == ' ' || detail.front() == '\n'))
                {
                    detail.erase(detail.begin());
                }
                if (!detail.empty())
                {
                    oss << "\n" << detail;
                }
            }

            formatted = oss.str();
        }
    }
    else
    {
	    auto actualPos = raw.find(actualTag);
	    if (actualPos != std::string::npos)
	    {
	        auto notContainPos = raw.find(notContainTag, actualPos);
	        auto endActual = raw.find('>', actualPos + actualTag.size());
	        if (notContainPos != std::string::npos && endActual != std::string::npos)
	        {
	            auto endExpected = raw.find('>', notContainPos + notContainTag.size());
	            if (endExpected != std::string::npos)
	            {
	                auto actualValue = raw.substr(actualPos + actualTag.size(), endActual - (actualPos + actualTag.size()));
	                auto expectedValue = raw.substr(notContainPos + notContainTag.size(), endExpected - (notContainPos + notContainTag.size()));

	                std::ostringstream oss;
	                oss << "Expected to contain: " << expectedValue << "\n";
	                oss << "Actual             : " << actualValue;

	                auto detailStart = raw.find('\n', endExpected);
	                if (detailStart != std::string::npos)
	                {
	                    auto detail = raw.substr(detailStart + 1);
	                    while (!detail.empty() && (detail.front() == ' ' || detail.front() == '\n'))
	                    {
	                        detail.erase(detail.begin());
	                    }
	                    if (!detail.empty())
	                    {
	                        oss << "\n" << detail;
	                    }
	                }

	                formatted = oss.str();
	            }
	        }
	    }
    }

    if (formatted.empty())
    {
        formatted = raw;
    }

    m_failureMessage = formatted;
}

std::string AllureCppUTestOutput::getAndClearFailureMessage()
{
    std::string message = m_failureMessage;
    m_failureMessage.clear();
    return message;
}

void AllureCppUTestOutput::printBuffer(const char* /*s*/)
{
    // Don't print anything, we just want to capture the failure
}

void AllureCppUTestOutput::flush()
{
    // Don't print anything, we just want to capture the failure
}

} // namespace cpputest
} // namespace adapters
} // namespace allure
