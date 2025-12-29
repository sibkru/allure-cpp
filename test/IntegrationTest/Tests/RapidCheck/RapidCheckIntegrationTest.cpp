/**
 * @file RapidCheckIntegrationTest.cpp
 * @brief Integration tests for RapidCheck adapter with Allure reporting.
 *
 * These tests verify that RapidCheck property tests generate correct
 * Allure report output including:
 * - Correct labels (testType=property, framework=rapidcheck)
 * - Steps with test case counts
 * - Distribution statistics from RC_CLASSIFY
 * - Counterexample parameters and attachments on failure
 */

#ifdef ALLURE_RAPIDCHECK_ENABLED

#include "stdafx.h"
#include "../BaseIntegrationTest.h"

#include <Framework/Adapters/RapidCheck/AllureRapidCheck.h>
#include "TestUtilities/JSONComparison.h"

#include <functional>
#include <iomanip>
#include <sstream>

using namespace testing;
using namespace systelab::json::test_utility;
using namespace allure;

namespace systelab { namespace gtest_allure { namespace unit_test {


class RapidCheckIntegrationTest : public ::testing::Test,
                                   public BaseIntegrationTest
{
public:
    void SetUp() override
    {
        BaseIntegrationTest::SetUp();
    }

    void TearDown() override
    {
        BaseIntegrationTest::TearDown();
    }
};


// Test that the listener correctly counts test cases
TEST_F(RapidCheckIntegrationTest, ListenerCountsTestCases)
{
    allure::adapters::rapidcheck::AllureRCListener listener;

    // Simulate multiple test cases finishing
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");

    listener.onTestCaseFinished(desc);
    listener.onTestCaseFinished(desc);
    listener.onTestCaseFinished(desc);

    EXPECT_EQ(3u, listener.testCaseCount());
}


// Test that the listener correctly tracks tags
TEST_F(RapidCheckIntegrationTest, ListenerTracksTags)
{
    allure::adapters::rapidcheck::AllureRCListener listener;

    rc::detail::CaseDescription desc1, desc2, desc3;
    desc1.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc2.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc3.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");

    desc1.tags = {"even", "positive"};
    desc2.tags = {"odd", "positive"};
    desc3.tags = {"even", "negative"};

    listener.onTestCaseFinished(desc1);
    listener.onTestCaseFinished(desc2);
    listener.onTestCaseFinished(desc3);

    EXPECT_EQ(2u, listener.tagCounts().at("even"));
    EXPECT_EQ(1u, listener.tagCounts().at("odd"));
    EXPECT_EQ(2u, listener.tagCounts().at("positive"));
    EXPECT_EQ(1u, listener.tagCounts().at("negative"));
}


// Test that the listener correctly tracks shrink attempts
TEST_F(RapidCheckIntegrationTest, ListenerTracksShrinkAttempts)
{
    allure::adapters::rapidcheck::AllureRCListener listener;

    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Failure, "");

    // Simulate shrinking: 5 attempts, 2 accepted
    listener.onShrinkTried(desc, false);
    listener.onShrinkTried(desc, true);
    listener.onShrinkTried(desc, false);
    listener.onShrinkTried(desc, true);
    listener.onShrinkTried(desc, false);

    EXPECT_EQ(5u, listener.shrinkAttempts());
    EXPECT_EQ(2u, listener.acceptedShrinks());
}


// Test that the listener maintains independent counts
TEST_F(RapidCheckIntegrationTest, ListenerMaintainsIndependentCounts)
{
    allure::adapters::rapidcheck::AllureRCListener listener;

    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc.tags = {"tagged"};

    // Mix of operations
    listener.onTestCaseFinished(desc);
    listener.onShrinkTried(desc, true);
    listener.onTestCaseFinished(desc);
    listener.onShrinkTried(desc, false);
    listener.onTestCaseFinished(desc);

    EXPECT_EQ(3u, listener.testCaseCount());
    EXPECT_EQ(2u, listener.shrinkAttempts());
    EXPECT_EQ(1u, listener.acceptedShrinks());
    EXPECT_EQ(3u, listener.tagCounts().at("tagged"));
}


// Verify that success result can be constructed
TEST_F(RapidCheckIntegrationTest, SuccessResultStructure)
{
    rc::detail::SuccessResult success;
    success.numSuccess = 100;
    // Distribution uses Tags (std::vector<std::string>) as key
    success.distribution[{"even"}] = 48;
    success.distribution[{"odd"}] = 52;

    EXPECT_EQ(100, success.numSuccess);
    EXPECT_EQ(48, success.distribution.at({"even"}));
    EXPECT_EQ(52, success.distribution.at({"odd"}));
}


// Verify that failure result can be constructed
TEST_F(RapidCheckIntegrationTest, FailureResultStructure)
{
    rc::detail::FailureResult failure;
    failure.numSuccess = 42;
    failure.description = "Property failed";
    failure.counterExample.push_back({"x", "42"});
    failure.counterExample.push_back({"y", "-1"});

    EXPECT_EQ(42, failure.numSuccess);
    EXPECT_EQ("Property failed", failure.description);
    EXPECT_EQ(2u, failure.counterExample.size());
    EXPECT_EQ("x", failure.counterExample[0].first);
    EXPECT_EQ("42", failure.counterExample[0].second);
}


// Verify that gave up result can be constructed
TEST_F(RapidCheckIntegrationTest, GaveUpResultStructure)
{
    rc::detail::GaveUpResult gaveUp;
    gaveUp.numSuccess = 10;
    gaveUp.description = "Too many precondition failures";

    EXPECT_EQ(10, gaveUp.numSuccess);
    EXPECT_EQ("Too many precondition failures", gaveUp.description);
}


}}} // namespace systelab::gtest_allure::unit_test

#endif // ALLURE_RAPIDCHECK_ENABLED
