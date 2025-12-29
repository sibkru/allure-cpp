#pragma once

/**
 * @file AllureRapidCheck.h
 * @brief RapidCheck integration for Allure test reporting.
 *
 * This header provides integration between RapidCheck property-based tests
 * and the Allure reporting framework. It allows RapidCheck tests to generate
 * rich Allure reports with property-specific information including:
 * - Test case count
 * - Distribution statistics from RC_CLASSIFY
 * - On failure: shrink count, counterexample as parameters and attachment
 * - Labels: testType=property, framework=rapidcheck
 *
 * Usage:
 * Simply replace your include of <rapidcheck/gtest.h> with this header:
 *
 *   #include <Framework/Adapters/RapidCheck/AllureRapidCheck.h>
 *
 * All existing RC_GTEST_PROP and RC_GTEST_FIXTURE_PROP macros will work
 * unchanged while producing Allure reports.
 */

#include <rapidcheck.h>
#include <rapidcheck/detail/TestListener.h>
#include <rapidcheck/detail/Configuration.h>
#include <rapidcheck/detail/ExecFixture.h>
#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <functional>
#include <memory>


namespace allure {
namespace adapters {
namespace rapidcheck {

/**
 * @brief Listener that collects RapidCheck execution data for Allure reporting.
 *
 * This listener implements rc::detail::TestListener to receive callbacks
 * during property test execution. It collects:
 * - Number of test cases run
 * - Number of shrink attempts and accepted shrinks
 * - Tag counts from RC_CLASSIFY for distribution statistics
 */
class AllureRCListener : public rc::detail::TestListener
{
public:
    AllureRCListener() = default;
    ~AllureRCListener() override = default;

    /**
     * @brief Called when a test case finishes.
     * @param desc Description of the completed test case
     */
    void onTestCaseFinished(const rc::detail::CaseDescription& desc) override;

    /**
     * @brief Called when a shrink is attempted.
     * @param desc Description of the shrink candidate
     * @param accepted Whether the shrink was accepted
     */
    void onShrinkTried(const rc::detail::CaseDescription& desc, bool accepted) override;

    /**
     * @brief Called when the entire test finishes.
     * @param metadata Test metadata
     * @param result Test result
     */
    void onTestFinished(const rc::detail::TestMetadata& metadata,
                        const rc::detail::TestResult& result) override;

    // Accessors for collected statistics
    size_t testCaseCount() const { return m_testCaseCount; }
    size_t shrinkAttempts() const { return m_shrinkAttempts; }
    size_t acceptedShrinks() const { return m_acceptedShrinks; }
    const std::map<std::string, size_t>& tagCounts() const { return m_tagCounts; }

private:
    size_t m_testCaseCount = 0;
    size_t m_shrinkAttempts = 0;
    size_t m_acceptedShrinks = 0;
    std::map<std::string, size_t> m_tagCounts;
};

/**
 * @brief Report a successful property test to Allure.
 * @param listener The listener with collected statistics
 * @param success The success result from RapidCheck
 */
void reportSuccessToAllure(const AllureRCListener& listener,
                           const rc::detail::SuccessResult& success);

/**
 * @brief Report a failed property test to Allure.
 * @param listener The listener with collected statistics
 * @param failure The failure result from RapidCheck
 */
void reportFailureToAllure(const AllureRCListener& listener,
                           const rc::detail::FailureResult& failure);

/**
 * @brief Report a gave-up property test to Allure.
 * @param listener The listener with collected statistics
 * @param gaveUp The gave-up result from RapidCheck
 */
void reportGaveUpToAllure(const AllureRCListener& listener,
                          const rc::detail::GaveUpResult& gaveUp);

/**
 * @brief Main entry point - runs a property test with Allure reporting.
 *
 * This function replaces rc::detail::checkGTest to provide Allure integration.
 * It runs the property test with our custom listener and reports results to Allure.
 *
 * @tparam Testable The testable type (typically a function pointer)
 * @param testable The property to test
 */
template <typename Testable>
void checkWithAllure(Testable&& testable);

} // namespace rapidcheck
} // namespace adapters
} // namespace allure


//==============================================================================
// MACRO DEFINITIONS
// These replace the macros from <rapidcheck/gtest.h>
//==============================================================================

/**
 * @brief Define a property-based test with Allure integration.
 *
 * This macro is a drop-in replacement for RC_GTEST_PROP from rapidcheck/gtest.h.
 * Tests defined with this macro will generate Allure reports with property-specific
 * information.
 *
 * Usage:
 *   RC_GTEST_PROP(StringProperties, ReverseTwiceIsIdentity, (const std::string& str)) {
 *       std::string reversed = str;
 *       std::reverse(reversed.begin(), reversed.end());
 *       std::reverse(reversed.begin(), reversed.end());
 *       RC_ASSERT(reversed == str);
 *   }
 */
#define RC_GTEST_PROP(TestCase, Name, ArgList)                                 \
    void rapidCheck_propImpl_##TestCase##_##Name ArgList;                      \
                                                                               \
    TEST(TestCase, Name) {                                                     \
        ::allure::adapters::rapidcheck::checkWithAllure(                       \
            &rapidCheck_propImpl_##TestCase##_##Name);                         \
    }                                                                          \
                                                                               \
    void rapidCheck_propImpl_##TestCase##_##Name ArgList


/**
 * @brief Define a fixture-based property test with Allure integration.
 *
 * This macro is a drop-in replacement for RC_GTEST_FIXTURE_PROP from rapidcheck/gtest.h.
 * Tests defined with this macro will use the specified fixture for setup/teardown
 * and generate Allure reports with property-specific information.
 *
 * Usage:
 *   class DatabaseFixture : public ::testing::Test {
 *   protected:
 *       void SetUp() override { ... }
 *       void TearDown() override { ... }
 *   };
 *
 *   RC_GTEST_FIXTURE_PROP(DatabaseFixture, CanQueryData, (int id)) {
 *       RC_ASSERT(getValue(id) >= 0);
 *   }
 */
#define RC_GTEST_FIXTURE_PROP(Fixture, Name, ArgList)                          \
    class RapidCheckPropImpl_##Fixture##_##Name : public Fixture {             \
    public:                                                                    \
        void rapidCheck_fixtureSetUp() { this->SetUp(); }                      \
        void TestBody() override {}                                            \
        void operator() ArgList;                                               \
        void rapidCheck_fixtureTearDown() { this->TearDown(); }                \
    };                                                                         \
                                                                               \
    TEST(Fixture##_RapidCheck, Name) {                                         \
        ::allure::adapters::rapidcheck::checkWithAllure(                       \
            &::rc::detail::ExecFixture<                                        \
                RapidCheckPropImpl_##Fixture##_##Name>::exec);                 \
    }                                                                          \
                                                                               \
    void RapidCheckPropImpl_##Fixture##_##Name::operator() ArgList


//==============================================================================
// TEMPLATE IMPLEMENTATION
//==============================================================================

// Include necessary headers for the template implementation
#include "API/Core.h"
#include "API/StepFunctions.h"
#include "API/TestMetadata.h"
#include "API/Attachment.h"

namespace allure {
namespace adapters {
namespace rapidcheck {

template <typename Testable>
void checkWithAllure(Testable&& testable)
{
    // Get test info from GTest
    const auto* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();

    // Build RapidCheck metadata (use fully qualified name to avoid conflict with allure::TestMetadata)
    rc::detail::TestMetadata rcMetadata;
    rcMetadata.id = std::string(testInfo->test_suite_name()) + "/" +
                    std::string(testInfo->name());
    rcMetadata.description = std::string(testInfo->name());

    // Add property test labels to Allure
    allure::test()
        .label("testType", "property")
        .label("framework", "rapidcheck");

    // Create our listener
    AllureRCListener listener;

    // Convert testable to Property and call checkProperty with our listener
    const auto property = rc::detail::toProperty(std::forward<Testable>(testable));
    const auto result = rc::detail::checkProperty(
        property,
        rcMetadata,
        rc::detail::configuration().testParams,
        listener
    );

    // Report to Allure based on result type
    if (result.template is<rc::detail::SuccessResult>())
    {
        reportSuccessToAllure(listener, result.template get<rc::detail::SuccessResult>());
    }
    else if (result.template is<rc::detail::FailureResult>())
    {
        reportFailureToAllure(listener, result.template get<rc::detail::FailureResult>());

        // Fail the GTest
        std::ostringstream ss;
        rc::detail::printResultMessage(result, ss);
        GTEST_FAIL() << ss.str();
    }
    else if (result.template is<rc::detail::GaveUpResult>())
    {
        reportGaveUpToAllure(listener, result.template get<rc::detail::GaveUpResult>());

        // Fail the GTest
        std::ostringstream ss;
        rc::detail::printResultMessage(result, ss);
        GTEST_FAIL() << ss.str();
    }
}

} // namespace rapidcheck
} // namespace adapters
} // namespace allure
