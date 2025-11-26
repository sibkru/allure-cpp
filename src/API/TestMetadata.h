#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace allure {

/**
 * Fluent builder for test case metadata.
 * Metadata is automatically applied when the builder is destroyed (RAII).
 *
 * Example:
 *   test()
 *       .name("Login test")
 *       .epic("Authentication")
 *       .feature("Login")
 *       .severity("critical")
 *       .owner("qa-team");
 *   // Metadata applied here when builder goes out of scope
 */
class TestMetadata {
public:
    TestMetadata() = default;
    ~TestMetadata();

    // Non-copyable, movable
    TestMetadata(const TestMetadata&) = delete;
    TestMetadata& operator=(const TestMetadata&) = delete;
    TestMetadata(TestMetadata&&) noexcept = default;
    TestMetadata& operator=(TestMetadata&&) noexcept = default;

    // Test identification
    TestMetadata& name(std::string_view name);
    TestMetadata& description(std::string_view desc);
    TestMetadata& descriptionHtml(std::string_view html);

    // Labels (for organization and filtering)
    TestMetadata& epic(std::string_view epic);
    TestMetadata& feature(std::string_view feature);
    TestMetadata& story(std::string_view story);
    TestMetadata& severity(std::string_view severity);
    TestMetadata& owner(std::string_view owner);
    TestMetadata& tag(std::string_view tag);
    TestMetadata& label(std::string_view name, std::string_view value);

    // Links (to external systems)
    TestMetadata& link(std::string_view name, std::string_view url,
                      std::string_view type = "custom");
    TestMetadata& issue(std::string_view name, std::string_view url);
    TestMetadata& tms(std::string_view name, std::string_view url);

    // Parameters (for parametric tests)
    TestMetadata& parameter(std::string_view name, std::string_view value);
    TestMetadata& maskedParameter(std::string_view name, std::string_view value);
    TestMetadata& hiddenParameter(std::string_view name, std::string_view value);

    // Status flags
    TestMetadata& flaky();
    TestMetadata& known();
    TestMetadata& muted();

private:
    std::vector<std::function<void()>> m_operations;
};

/**
 * Factory function to create a TestMetadata builder.
 *
 * Example:
 *   test().epic("Auth").feature("Login");
 */
inline TestMetadata test() {
    return TestMetadata();
}

// ============================================================================
// Suite Metadata
// ============================================================================

/**
 * Fluent builder for test suite metadata.
 * Metadata is automatically applied when the builder is destroyed (RAII).
 */
class SuiteMetadata {
public:
    SuiteMetadata() = default;
    ~SuiteMetadata();

    // Non-copyable, movable
    SuiteMetadata(const SuiteMetadata&) = delete;
    SuiteMetadata& operator=(const SuiteMetadata&) = delete;
    SuiteMetadata(SuiteMetadata&&) noexcept = default;
    SuiteMetadata& operator=(SuiteMetadata&&) noexcept = default;

    // Suite identification
    SuiteMetadata& name(std::string_view name);
    SuiteMetadata& description(std::string_view desc);

    // Labels
    SuiteMetadata& epic(std::string_view epic);
    SuiteMetadata& severity(std::string_view severity);
    SuiteMetadata& label(std::string_view name, std::string_view value);

private:
    std::vector<std::function<void()>> m_operations;
};

/**
 * Factory function to create a SuiteMetadata builder.
 */
inline SuiteMetadata suite() {
    return SuiteMetadata();
}

// Parent/Sub suite support is handled by the test framework's structure
// (e.g., TEST_F class hierarchy in GTest, or test groups in CppUTest)

} // namespace allure
