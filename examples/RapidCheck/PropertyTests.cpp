/**
 * @file PropertyTests.cpp
 * @brief Example RapidCheck property-based tests with Allure integration.
 *
 * This file demonstrates how to use RapidCheck property-based testing with
 * Allure reporting. Replace your #include <rapidcheck/gtest.h> with
 * #include <Framework/Adapters/RapidCheck/AllureRapidCheck.h> to get
 * rich Allure reports from your property tests.
 */

#include <Framework/Adapters/RapidCheck/AllureRapidCheck.h>
#include <allure-cpp.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <cmath>

using namespace allure;


//==============================================================================
// String Properties
//==============================================================================

/**
 * Property: Reversing a string twice yields the original string.
 */
RC_GTEST_PROP(StringProperties, ReverseTwiceIsIdentity, (const std::string& str))
{
    std::string reversed = str;
    std::reverse(reversed.begin(), reversed.end());
    std::reverse(reversed.begin(), reversed.end());
    RC_ASSERT(reversed == str);
}

/**
 * Property: Appending two strings maintains combined length.
 */
RC_GTEST_PROP(StringProperties, AppendPreservesLength, (const std::string& a, const std::string& b))
{
    std::string combined = a + b;
    RC_ASSERT(combined.length() == a.length() + b.length());
}


//==============================================================================
// Integer Properties
//==============================================================================

/**
 * Property: Absolute value is always non-negative.
 * Demonstrates RC_CLASSIFY for distribution statistics.
 */
RC_GTEST_PROP(IntegerProperties, AbsoluteValueNonNegative, (int x))
{
    // Classify inputs for distribution reporting
    RC_CLASSIFY(x < 0, "negative");
    RC_CLASSIFY(x == 0, "zero");
    RC_CLASSIFY(x > 0, "positive");

    // Handle INT_MIN edge case (abs(INT_MIN) overflows on most systems)
    if (x != std::numeric_limits<int>::min())
    {
        RC_ASSERT(std::abs(x) >= 0);
    }
}

/**
 * Property: Addition is commutative.
 */
RC_GTEST_PROP(IntegerProperties, AdditionIsCommutative, (int a, int b))
{
    RC_ASSERT(a + b == b + a);
}

/**
 * Property: Multiplication by zero yields zero.
 */
RC_GTEST_PROP(IntegerProperties, MultiplyByZeroIsZero, (int x))
{
    RC_ASSERT(x * 0 == 0);
}


//==============================================================================
// Vector Properties
//==============================================================================

/**
 * Property: Sorting a vector puts elements in non-decreasing order.
 */
RC_GTEST_PROP(VectorProperties, SortProducesOrderedResult, (std::vector<int> vec))
{
    std::sort(vec.begin(), vec.end());

    for (size_t i = 1; i < vec.size(); i++)
    {
        RC_ASSERT(vec[i - 1] <= vec[i]);
    }
}

/**
 * Property: Sorting preserves vector size.
 */
RC_GTEST_PROP(VectorProperties, SortPreservesSize, (std::vector<int> vec))
{
    size_t originalSize = vec.size();
    std::sort(vec.begin(), vec.end());
    RC_ASSERT(vec.size() == originalSize);
}

/**
 * Property: Sum of elements is the same after sorting.
 */
RC_GTEST_PROP(VectorProperties, SortPreservesSum, (std::vector<int> vec))
{
    // Classify by vector size for distribution
    RC_CLASSIFY(vec.empty(), "empty");
    RC_CLASSIFY(vec.size() == 1, "singleton");
    RC_CLASSIFY(vec.size() > 1 && vec.size() <= 10, "small");
    RC_CLASSIFY(vec.size() > 10, "large");

    long long originalSum = std::accumulate(vec.begin(), vec.end(), 0LL);
    std::sort(vec.begin(), vec.end());
    long long sortedSum = std::accumulate(vec.begin(), vec.end(), 0LL);

    RC_ASSERT(originalSum == sortedSum);
}


//==============================================================================
// Edge Case Properties
//==============================================================================

/**
 * Property: Empty string reversed is still empty.
 */
RC_GTEST_PROP(EdgeCaseProperties, EmptyStringReversesEmpty, ())
{
    std::string empty;
    std::reverse(empty.begin(), empty.end());
    RC_ASSERT(empty.empty());
}

/**
 * Property: Single-character string reversed is unchanged.
 */
RC_GTEST_PROP(EdgeCaseProperties, SingleCharReversesUnchanged, (char c))
{
    std::string single(1, c);
    std::string original = single;
    std::reverse(single.begin(), single.end());
    RC_ASSERT(single == original);
}


//==============================================================================
// Failing Property (Disabled) - Demonstrates counterexample reporting
//==============================================================================

/**
 * Property: This property intentionally fails to demonstrate
 * how Allure reports counterexamples.
 *
 * Note: This test is DISABLED (won't run by default).
 * Enable it to see the failure reporting in action.
 */
RC_GTEST_PROP(FailingProperties, DISABLED_VectorSumAlwaysPositive,
              (const std::vector<int>& vec))
{
    RC_PRE(!vec.empty());  // Require non-empty vector

    long long sum = 0;
    for (int x : vec)
    {
        sum += x;
    }

    // This will fail for vectors with negative sums
    RC_ASSERT(sum > 0);
}


//==============================================================================
// Fixture-based Property Test
//==============================================================================

/**
 * A simple fixture that provides a multiplier value.
 */
class MultiplierFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        multiplier = 42;
    }

    void TearDown() override
    {
        // Cleanup if needed
    }

    int getMultiplier() const { return multiplier; }

private:
    int multiplier;
};

/**
 * Property: Fixture value is accessible during property test.
 */
RC_GTEST_FIXTURE_PROP(MultiplierFixture, FixtureValueIsAccessible, ())
{
    RC_ASSERT(getMultiplier() == 42);
}

/**
 * Property: Multiplication with fixture value distributes over addition.
 */
RC_GTEST_FIXTURE_PROP(MultiplierFixture, MultiplicationDistributesOverAddition,
                      (int a, int b))
{
    int m = getMultiplier();
    RC_ASSERT(m * (a + b) == m * a + m * b);
}
