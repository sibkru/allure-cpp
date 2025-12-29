/**
 * @file AllureRCListenerTest.cpp
 * @brief Unit tests for the AllureRCListener class.
 */

#ifdef ALLURE_RAPIDCHECK_ENABLED

#include "stdafx.h"
#include <gtest/gtest.h>

#include <Framework/Adapters/RapidCheck/AllureRapidCheck.h>

using namespace allure::adapters::rapidcheck;


class AllureRCListenerTest : public ::testing::Test
{
protected:
    AllureRCListener listener;
};


TEST_F(AllureRCListenerTest, InitialStateIsZero)
{
    EXPECT_EQ(0u, listener.testCaseCount());
    EXPECT_EQ(0u, listener.shrinkAttempts());
    EXPECT_EQ(0u, listener.acceptedShrinks());
    EXPECT_TRUE(listener.tagCounts().empty());
}


TEST_F(AllureRCListenerTest, OnTestCaseFinishedIncrementsCount)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");

    listener.onTestCaseFinished(desc);
    EXPECT_EQ(1u, listener.testCaseCount());

    listener.onTestCaseFinished(desc);
    EXPECT_EQ(2u, listener.testCaseCount());

    listener.onTestCaseFinished(desc);
    EXPECT_EQ(3u, listener.testCaseCount());
}


TEST_F(AllureRCListenerTest, OnTestCaseFinishedTracksTags)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc.tags = {"even", "positive"};

    listener.onTestCaseFinished(desc);

    EXPECT_EQ(1u, listener.tagCounts().at("even"));
    EXPECT_EQ(1u, listener.tagCounts().at("positive"));
}


TEST_F(AllureRCListenerTest, OnShrinkTriedTracksAttempts)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Failure, "");

    listener.onShrinkTried(desc, false);
    listener.onShrinkTried(desc, false);
    listener.onShrinkTried(desc, true);

    EXPECT_EQ(3u, listener.shrinkAttempts());
    EXPECT_EQ(1u, listener.acceptedShrinks());
}


TEST_F(AllureRCListenerTest, OnShrinkTriedCountsAcceptedShrinks)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Failure, "");

    listener.onShrinkTried(desc, true);
    listener.onShrinkTried(desc, true);
    listener.onShrinkTried(desc, false);
    listener.onShrinkTried(desc, true);
    listener.onShrinkTried(desc, false);

    EXPECT_EQ(5u, listener.shrinkAttempts());
    EXPECT_EQ(3u, listener.acceptedShrinks());
}


TEST_F(AllureRCListenerTest, MultipleTagOccurrencesAreCounted)
{
    rc::detail::CaseDescription desc1, desc2, desc3;
    desc1.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc2.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc3.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");

    desc1.tags = {"even"};
    desc2.tags = {"odd"};
    desc3.tags = {"even"};

    listener.onTestCaseFinished(desc1);
    listener.onTestCaseFinished(desc2);
    listener.onTestCaseFinished(desc3);

    EXPECT_EQ(2u, listener.tagCounts().at("even"));
    EXPECT_EQ(1u, listener.tagCounts().at("odd"));
}


TEST_F(AllureRCListenerTest, MultipleTagsPerCaseAreCounted)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc.tags = {"positive", "even", "small"};

    listener.onTestCaseFinished(desc);

    EXPECT_EQ(1u, listener.tagCounts().at("positive"));
    EXPECT_EQ(1u, listener.tagCounts().at("even"));
    EXPECT_EQ(1u, listener.tagCounts().at("small"));
    EXPECT_EQ(3u, listener.tagCounts().size());
}


TEST_F(AllureRCListenerTest, NoTagsDoesNotAddToMap)
{
    rc::detail::CaseDescription desc;
    desc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    desc.tags = {};  // Empty tags

    listener.onTestCaseFinished(desc);

    EXPECT_TRUE(listener.tagCounts().empty());
    EXPECT_EQ(1u, listener.testCaseCount());
}


TEST_F(AllureRCListenerTest, OnTestFinishedDoesNotModifyState)
{
    // First add some state
    rc::detail::CaseDescription caseDesc;
    caseDesc.result = rc::detail::CaseResult(rc::detail::CaseResult::Type::Success, "");
    caseDesc.tags = {"tag"};
    listener.onTestCaseFinished(caseDesc);
    listener.onShrinkTried(caseDesc, true);

    size_t countBefore = listener.testCaseCount();
    size_t shrinksBefore = listener.shrinkAttempts();
    size_t acceptedBefore = listener.acceptedShrinks();
    size_t tagsBefore = listener.tagCounts().size();

    // Call onTestFinished
    rc::detail::TestMetadata metadata;
    metadata.id = "test-id";
    metadata.description = "test description";

    rc::detail::SuccessResult success;
    success.numSuccess = 100;

    listener.onTestFinished(metadata, rc::detail::TestResult(success));

    // State should not have changed
    EXPECT_EQ(countBefore, listener.testCaseCount());
    EXPECT_EQ(shrinksBefore, listener.shrinkAttempts());
    EXPECT_EQ(acceptedBefore, listener.acceptedShrinks());
    EXPECT_EQ(tagsBefore, listener.tagCounts().size());
}


#endif // ALLURE_RAPIDCHECK_ENABLED
