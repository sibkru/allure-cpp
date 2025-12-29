#include "Framework/Adapters/RapidCheck/AllureRapidCheck.h"

#include "API/Core.h"
#include "API/StepFunctions.h"
#include "API/TestMetadata.h"
#include "API/Attachment.h"

#include <sstream>
#include <iomanip>


namespace allure {
namespace adapters {
namespace rapidcheck {

void AllureRCListener::onTestCaseFinished(const rc::detail::CaseDescription& desc)
{
    m_testCaseCount++;
    for (const auto& tag : desc.tags)
    {
        m_tagCounts[tag]++;
    }
}

void AllureRCListener::onShrinkTried(const rc::detail::CaseDescription& desc, bool accepted)
{
    (void)desc; // Unused but part of interface
    m_shrinkAttempts++;
    if (accepted)
    {
        m_acceptedShrinks++;
    }
}

void AllureRCListener::onTestFinished(const rc::detail::TestMetadata& metadata,
                                       const rc::detail::TestResult& result)
{
    // Result is handled in checkWithAllure after this returns
    (void)metadata;
    (void)result;
}

// Helper to format tags (vector of strings) as a single string
static std::string formatTags(const std::vector<std::string>& tags)
{
    if (tags.empty())
    {
        return "(empty)";
    }
    if (tags.size() == 1)
    {
        return tags[0];
    }
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < tags.size(); ++i)
    {
        if (i > 0) ss << ", ";
        ss << tags[i];
    }
    ss << "]";
    return ss.str();
}

void reportSuccessToAllure(const AllureRCListener& listener,
                           const rc::detail::SuccessResult& success)
{
    allure::step("Property verified", [&]() {
        allure::step("Ran " + std::to_string(success.numSuccess) +
                     " test cases", [](){});

        if (!success.distribution.empty())
        {
            allure::step("Distribution", [&]() {
                for (const auto& entry : success.distribution)
                {
                    double pct = 100.0 * static_cast<double>(entry.second) /
                                 static_cast<double>(success.numSuccess);
                    std::ostringstream ss;
                    ss << formatTags(entry.first) << ": " << std::fixed << std::setprecision(1)
                       << pct << "%";
                    allure::step(ss.str(), [](){});
                }
            });
        }
    });

    // Also add the distribution tags as parameters for visibility
    if (!listener.tagCounts().empty())
    {
        for (const auto& entry : listener.tagCounts())
        {
            double pct = 100.0 * static_cast<double>(entry.second) /
                         static_cast<double>(listener.testCaseCount());
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << pct << "% ("
               << entry.second << "/" << listener.testCaseCount() << ")";
            allure::test().parameter(entry.first, ss.str());
        }
    }
}

void reportFailureToAllure(const AllureRCListener& listener,
                           const rc::detail::FailureResult& failure)
{
    // Add counterexample as test parameters
    for (const auto& param : failure.counterExample)
    {
        allure::test().parameter(param.first, param.second);
    }

    allure::step("Property falsified", [&]() {
        allure::step("Ran " + std::to_string(failure.numSuccess) +
                     " tests before failure", [](){});

        std::ostringstream shrinkMsg;
        shrinkMsg << "Shrunk " << listener.acceptedShrinks()
                  << " times (tried " << listener.shrinkAttempts() << ")";
        allure::step(shrinkMsg.str(), [](){});

        // Attach counterexample as text file
        std::ostringstream counterexample;
        counterexample << "Counterexample:\n";
        for (const auto& param : failure.counterExample)
        {
            counterexample << "  " << param.first << " = " << param.second << "\n";
        }
        counterexample << "\nDescription: " << failure.description << "\n";
        counterexample << "\nReproduce: " << failure.reproduce << "\n";
        allure::attachText("counterexample.txt", counterexample.str());
    });
}

void reportGaveUpToAllure(const AllureRCListener& listener,
                          const rc::detail::GaveUpResult& gaveUp)
{
    (void)listener; // May be used for additional stats in the future

    allure::step("Property gave up", [&]() {
        allure::step("Only " + std::to_string(gaveUp.numSuccess) +
                     " tests passed preconditions", [](){});
        allure::step("Description: " + gaveUp.description, [](){});
    });

    // Attach details
    std::ostringstream details;
    details << "Property gave up after " << gaveUp.numSuccess << " successful tests.\n";
    details << "Description: " << gaveUp.description << "\n";
    details << "\nThis typically happens when RC_PRE() preconditions are too restrictive,\n";
    details << "causing RapidCheck to discard too many generated test cases.\n";
    allure::attachText("gave-up-details.txt", details.str());
}

} // namespace rapidcheck
} // namespace adapters
} // namespace allure
