#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace allure {

/**
 * @file TestMetadata.h
 * @brief Fluent builders for attaching metadata to running tests and suites.
 *
 * Each builder queues mutations and applies them when the object is destroyed,
 * making it convenient to write scoped, chainable metadata declarations.
 */

/**
 * Fluent builder for test case metadata.
 * Metadata is automatically applied when the builder is destroyed (RAII). Each
 * setter records an operation that is executed once the builder leaves scope,
 * so chains stay cheap and side-effect free until destruction. Builders are
 * intended for use on the thread executing the current test case.
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
    /**
     * Set the test display name.
     * @param name Human-readable test name.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& name(std::string_view name);
    /**
     * Set a plain-text description of the test.
     * @param desc Description text.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& description(std::string_view desc);
    /**
     * Set an HTML-formatted description of the test.
     * @param html HTML description; caller is responsible for valid markup.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& descriptionHtml(std::string_view html);

    // Labels (for organization and filtering)
    /**
     * Add an epic label.
     * @param epic Epic identifier.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& epic(std::string_view epic);
    /**
     * Add a feature label.
     * @param feature Feature identifier.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& feature(std::string_view feature);
    /**
     * Add a story label.
     * @param story Story identifier.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& story(std::string_view story);
    /**
     * Add a severity label.
     * @param severity Severity value (e.g., "critical", "minor").
     * @return Reference to the builder for chaining.
     */
    TestMetadata& severity(std::string_view severity);
    /**
     * Add an owner label.
     * @param owner Responsible person or team.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& owner(std::string_view owner);
    /**
     * Add a tag label.
     * @param tag Arbitrary tag value.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& tag(std::string_view tag);
    /**
     * Add a custom label.
     * @param name Label name.
     * @param value Label value.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& label(std::string_view name, std::string_view value);

    // Links (to external systems)
    /**
     * Add a link to an external system.
     * @param name Display name of the link.
     * @param url Target URL.
     * @param type Link type (e.g., "issue", "tms", or custom). Defaults to "custom".
     * @return Reference to the builder for chaining.
     */
    TestMetadata& link(std::string_view name, std::string_view url,
                      std::string_view type = "custom");
    /**
     * Shortcut for adding an issue link.
     * @param name Issue identifier.
     * @param url Issue URL.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& issue(std::string_view name, std::string_view url);
    /**
     * Shortcut for adding a TMS link.
     * @param name TMS case identifier.
     * @param url TMS URL.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& tms(std::string_view name, std::string_view url);

    // Parameters (for parametric tests)
    /**
     * Add a visible parameter.
     * @param name Parameter name.
     * @param value Parameter value.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& parameter(std::string_view name, std::string_view value);
    /**
     * Add a parameter whose value is masked in the report.
     * @param name Parameter name.
     * @param value Parameter value.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& maskedParameter(std::string_view name, std::string_view value);
    /**
     * Add a parameter whose value is hidden in the report.
     * @param name Parameter name.
     * @param value Parameter value.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& hiddenParameter(std::string_view name, std::string_view value);

    // Status flags
    /**
     * Mark the test as flaky.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& flaky();
    /**
     * Mark the test as known (expected) failure.
     * @return Reference to the builder for chaining.
     */
    TestMetadata& known();
    /**
     * Mark the test as muted (do not affect build status).
     * @return Reference to the builder for chaining.
     */
    TestMetadata& muted();

private:
    std::vector<std::function<void()>> m_operations;
};

/**
 * Factory function to create a TestMetadata builder.
 *
 * Example:
 *   test().epic("Auth").feature("Login");
 *
 * Builders are lightweight temporaries; assign to a local if you need to
 * extend the lifetime beyond a single statement.
 */
inline TestMetadata test() {
    return TestMetadata();
}

// ============================================================================
// Suite Metadata
// ============================================================================

/**
 * Fluent builder for test suite metadata.
 * Metadata is automatically applied when the builder is destroyed (RAII). Each
 * setter queues work to be executed once the suite builder leaves scope.
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
    /**
     * Set the suite name.
     * @param name Human-readable suite name.
     * @return Reference to the builder for chaining.
     */
    SuiteMetadata& name(std::string_view name);
    /**
     * Set the suite description.
     * @param desc Description text.
     * @return Reference to the builder for chaining.
     */
    SuiteMetadata& description(std::string_view desc);

    // Labels
    /**
     * Add an epic label to the suite.
     * @param epic Epic identifier.
     * @return Reference to the builder for chaining.
     */
    SuiteMetadata& epic(std::string_view epic);
    /**
     * Add a severity label to the suite.
     * @param severity Severity value.
     * @return Reference to the builder for chaining.
     */
    SuiteMetadata& severity(std::string_view severity);
    /**
     * Add a custom label to the suite.
     * @param name Label name.
     * @param value Label value.
     * @return Reference to the builder for chaining.
     */
    SuiteMetadata& label(std::string_view name, std::string_view value);

private:
    std::vector<std::function<void()>> m_operations;
};

/**
 * Factory function to create a SuiteMetadata builder.
 *
 * Builders are lightweight temporaries; assign to a local if you need to
 * extend the lifetime beyond a single statement.
 */
inline SuiteMetadata suite() {
    return SuiteMetadata();
}

// Parent/Sub suite support is handled by the test framework's structure
// (e.g., TEST_F class hierarchy in GTest, or test groups in CppUTest)

} // namespace allure
