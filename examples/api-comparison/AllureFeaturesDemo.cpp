#include "GTestAllureUtilities/AllureAPI.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <fstream>

using namespace allure_cpp;

/**
 * @brief Comprehensive test suite demonstrating ALL Allure 2 features
 *
 * This file showcases every feature supported by the Allure 2 format:
 * - Links (issue, tms, custom)
 * - Attachments (text, file, binary)
 * - Parameters (default, masked, hidden, excluded)
 * - Labels (epic, feature, story, severity, owner, tag, custom)
 * - Descriptions (markdown and HTML)
 * - Status details (known, muted, flaky)
 * - Nested steps
 */

// ==============================================================================
// Test Suite 1: Links and Attachments
// ==============================================================================

class LinksAndAttachmentsTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Links and Attachments Demo");
		AllureAPI::setTestSuiteDescription("Demonstrating links and attachment features");
		AllureAPI::setTestSuiteEpic("Allure Features");
		AllureAPI::setTestSuiteSeverity("critical");
	}
};

TEST_F(LinksAndAttachmentsTestSuite, testWithLinks)
{
	AllureAPI::setTestCaseName("Test with various link types");
	AllureAPI::setTestCaseDescription("This test demonstrates how to add different types of links to a test case.");

	// Add different types of links
	AllureAPI::addIssue("BUG-123", "https://github.com/myorg/myrepo/issues/123");
	AllureAPI::addIssue("BUG-456", "https://jira.example.com/browse/BUG-456");
	AllureAPI::addTmsLink("TEST-789", "https://testrail.example.com/index.php?/cases/view/789");
	AllureAPI::addLink("Documentation", "https://docs.example.com/feature", "custom");

	// Add some labels for organization
	AllureAPI::addEpic("Bug Tracking");
	AllureAPI::addFeature("Link Management");
	AllureAPI::addStory("User can add links to tests");
	AllureAPI::addOwner("qa-team");
	AllureAPI::addTag("links");
	AllureAPI::addTag("metadata");

	EXPECT_TRUE(true);
}

TEST_F(LinksAndAttachmentsTestSuite, testWithTextAttachments)
{
	AllureAPI::setTestCaseName("Test with text attachments");
	AllureAPI::setTestCaseDescription("Demonstrates adding text attachments to test results.");

	AllureAPI::addAction("Generate test data", []() {
		// Create some test data
		std::string testData = "Test execution data:\n";
		testData += "Timestamp: 2025-11-14 12:00:00\n";
		testData += "Environment: Test\n";
		testData += "User: test-user\n";

		// Attach it to the test
		AllureAPI::addTextAttachment("Test Data", testData);
	});

	AllureAPI::addAction("Generate log output", []() {
		std::string logOutput = "[INFO] Test started\n";
		logOutput += "[DEBUG] Loading configuration\n";
		logOutput += "[INFO] Configuration loaded successfully\n";
		logOutput += "[DEBUG] Executing test steps\n";
		logOutput += "[INFO] Test completed\n";

		AllureAPI::addTextAttachment("Execution Log", logOutput);
	});

	AllureAPI::addExpectedResult("Attachments should be present", []() {
		EXPECT_TRUE(true);
	});
}

TEST_F(LinksAndAttachmentsTestSuite, testWithFileAttachment)
{
	AllureAPI::setTestCaseName("Test with file attachment");
	AllureAPI::setTestCaseDescription("Demonstrates attaching a file to test results.");

	AllureAPI::addAction("Create temporary test file", []() {
		// Create a temporary file
		std::ofstream tempFile("/tmp/test-data.json");
		tempFile << "{\n";
		tempFile << "  \"testName\": \"testWithFileAttachment\",\n";
		tempFile << "  \"status\": \"passed\",\n";
		tempFile << "  \"duration\": 150\n";
		tempFile << "}\n";
		tempFile.close();

		// Attach the file
		AllureAPI::addFileAttachment("Test Configuration", "/tmp/test-data.json");
	});

	EXPECT_TRUE(true);
}

// ==============================================================================
// Test Suite 2: Parameters
// ==============================================================================

class ParametersTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Parameters Demo");
		AllureAPI::setTestSuiteDescription("Demonstrating parameter features");
		AllureAPI::setTestSuiteEpic("Allure Features");
		AllureAPI::setTestSuiteSeverity("normal");
	}
};

TEST_F(ParametersTestSuite, testWithRegularParameters)
{
	AllureAPI::setTestCaseName("Test with regular parameters");
	AllureAPI::setTestCaseDescription("Shows how to add regular parameters to tests.");

	// Add regular parameters
	AllureAPI::addParameter("username", "test-user");
	AllureAPI::addParameter("email", "test@example.com");
	AllureAPI::addParameter("role", "admin");
	AllureAPI::addParameter("environment", "staging");

	AllureAPI::addTag("parameters");

	EXPECT_TRUE(true);
}

TEST_F(ParametersTestSuite, testWithMaskedParameters)
{
	AllureAPI::setTestCaseName("Test with masked parameters");
	AllureAPI::setTestCaseDescription("Demonstrates masked parameters for sensitive data (values are hidden in report).");

	// Regular parameters
	AllureAPI::addParameter("username", "admin");

	// Masked parameters (value will be shown as *****)
	AllureAPI::addMaskedParameter("password", "super-secret-password");
	AllureAPI::addMaskedParameter("apiKey", "sk-1234567890abcdef");
	AllureAPI::addMaskedParameter("token", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...");

	AllureAPI::addTag("security");
	AllureAPI::addTag("parameters");

	EXPECT_TRUE(true);
}

TEST_F(ParametersTestSuite, testWithHiddenParameters)
{
	AllureAPI::setTestCaseName("Test with hidden parameters");
	AllureAPI::setTestCaseDescription("Demonstrates hidden parameters (completely hidden from report).");

	// Visible parameters
	AllureAPI::addParameter("database", "test-db");

	// Hidden parameters (won't appear in report at all)
	AllureAPI::addHiddenParameter("dbPassword", "db-secret-123");
	AllureAPI::addHiddenParameter("encryptionKey", "aes-256-key-12345");

	AllureAPI::addTag("security");

	EXPECT_TRUE(true);
}

TEST_F(ParametersTestSuite, testWithExcludedParameters)
{
	AllureAPI::setTestCaseName("Test with excluded parameters");
	AllureAPI::setTestCaseDescription("Shows parameters excluded from history comparison.");

	// Regular parameter (affects history)
	AllureAPI::addParameter("testType", "functional");

	// Excluded parameters (won't affect test history ID - useful for timestamps, random IDs, etc.)
	AllureAPI::addParameter("timestamp", "2025-11-14T12:00:00Z", true, "default");
	AllureAPI::addParameter("runId", "run-abc-123", true, "default");
	AllureAPI::addParameter("randomSeed", "42", true, "default");

	AllureAPI::addTag("history");

	EXPECT_TRUE(true);
}

// ==============================================================================
// Test Suite 3: Descriptions and Status Details
// ==============================================================================

class DescriptionsTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Descriptions and Status Demo");
		AllureAPI::setTestSuiteDescription("Demonstrating description and status detail features");
		AllureAPI::setTestSuiteEpic("Allure Features");
	}
};

TEST_F(DescriptionsTestSuite, testWithMarkdownDescription)
{
	AllureAPI::setTestCaseName("Test with Markdown description");

	std::string markdownDesc = R"(# Test Purpose

This test demonstrates **Markdown** formatting in test descriptions.

## Features Tested
- List item 1
- List item 2
- List item 3

## Expected Behavior
The test should `pass` without any failures.

### Notes
> This is a blockquote with important information.

[Link to documentation](https://example.com/docs))";

	AllureAPI::setTestCaseDescription(markdownDesc);
	AllureAPI::addTag("documentation");

	EXPECT_TRUE(true);
}

TEST_F(DescriptionsTestSuite, testWithHtmlDescription)
{
	AllureAPI::setTestCaseName("Test with HTML description");

	std::string htmlDesc = R"(<div style="font-family: Arial, sans-serif;">
<h2 style="color: #2c3e50;">Test Purpose</h2>
<p>This test demonstrates <strong>HTML</strong> formatting in test descriptions.</p>
<h3>Features Tested</h3>
<ul>
  <li>HTML formatting</li>
  <li>Styled content</li>
  <li>Rich text support</li>
</ul>
<p><em>Note: HTML descriptions provide richer formatting options.</em></p>
</div>)";

	AllureAPI::setTestCaseDescriptionHtml(htmlDesc);
	AllureAPI::addTag("documentation");

	EXPECT_TRUE(true);
}

TEST_F(DescriptionsTestSuite, testMarkedAsFlaky)
{
	AllureAPI::setTestCaseName("Flaky test (intermittent failures)");
	AllureAPI::setTestCaseDescription("This test is marked as flaky because it has intermittent failures.");

	// Mark this test as flaky
	AllureAPI::markTestAsFlaky();

	AllureAPI::addTag("flaky");
	AllureAPI::addSeverity("minor");

	// Simulate flaky behavior
	auto currentTime = std::chrono::system_clock::now().time_since_epoch().count();
	bool shouldPass = (currentTime % 2 == 0);

	EXPECT_TRUE(shouldPass) << "Flaky test failed (this is expected sometimes)";
}

TEST_F(DescriptionsTestSuite, testMarkedAsKnown)
{
	AllureAPI::setTestCaseName("Known failure (documented bug)");
	AllureAPI::setTestCaseDescription("This test has a known failure due to BUG-789.");

	// Mark as known failure
	AllureAPI::markTestAsKnown();

	AllureAPI::addIssue("BUG-789", "https://github.com/example/issues/789");
	AllureAPI::addTag("known-issue");
	AllureAPI::addSeverity("normal");

	// This failure is expected and documented
	EXPECT_EQ(2 + 2, 5) << "Known issue: Math module has calculation bug (BUG-789)";
}

TEST_F(DescriptionsTestSuite, testMarkedAsMuted)
{
	AllureAPI::setTestCaseName("Muted test (temporarily ignored)");
	AllureAPI::setTestCaseDescription("This test is muted while we investigate infrastructure issues.");

	// Mark as muted
	AllureAPI::markTestAsMuted();

	AllureAPI::addTag("muted");
	AllureAPI::addSeverity("critical");

	EXPECT_TRUE(false) << "This failure is muted and won't affect build status";
}

// ==============================================================================
// Test Suite 4: Complex Labels and Organization
// ==============================================================================

class OrganizationTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Organization Demo");
		AllureAPI::setTestSuiteDescription("Demonstrating test organization with labels");
		AllureAPI::setTestSuiteEpic("Allure Features");
	}
};

TEST_F(OrganizationTestSuite, testWithComprehensiveLabels)
{
	AllureAPI::setTestCaseName("Test with comprehensive labeling");
	AllureAPI::setTestCaseDescription("Shows all types of labels for test organization.");

	// Organizational labels
	AllureAPI::addEpic("E-Commerce Platform");
	AllureAPI::addFeature("Shopping Cart");
	AllureAPI::addStory("As a user, I want to add items to cart");

	// Classification labels
	AllureAPI::addSeverity("blocker");
	AllureAPI::addOwner("john.doe@example.com");

	// Tags for filtering
	AllureAPI::addTag("smoke");
	AllureAPI::addTag("critical-path");
	AllureAPI::addTag("p0");
	AllureAPI::addTag("shopping");

	// Custom labels
	AllureAPI::addLabel("component", "cart-service");
	AllureAPI::addLabel("layer", "api");
	AllureAPI::addLabel("testType", "functional");

	EXPECT_TRUE(true);
}

TEST_F(OrganizationTestSuite, testSeverityLevels)
{
	AllureAPI::setTestCaseName("Demonstrating severity levels");
	AllureAPI::setTestCaseDescription("This test shows how severity affects test prioritization.");

	// Severity can be: blocker, critical, normal, minor, trivial
	AllureAPI::addSeverity("blocker");

	AllureAPI::addTag("severity-demo");

	EXPECT_TRUE(true);
}

// ==============================================================================
// Test Suite 5: Advanced Features Combined
// ==============================================================================

class AdvancedFeaturesTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Advanced Features Demo");
		AllureAPI::setTestSuiteDescription("Combining multiple Allure features");
		AllureAPI::setTestSuiteEpic("Allure Features");
		AllureAPI::setTestSuiteSeverity("high");
	}
};

TEST_F(AdvancedFeaturesTestSuite, testCombiningAllFeatures)
{
	AllureAPI::setTestCaseName("Comprehensive feature demonstration");

	std::string description = R"(# Comprehensive Test

This test combines **all** Allure features:
- Links to issues and test management
- Multiple attachments
- Various parameter types
- Rich descriptions
- Full labeling

## Purpose
Demonstrate the complete Allure reporting capability.)";

	AllureAPI::setTestCaseDescription(description);

	// Links
	AllureAPI::addIssue("FEAT-100", "https://jira.example.com/browse/FEAT-100");
	AllureAPI::addTmsLink("TC-200", "https://testrail.example.com/cases/200");

	// Labels
	AllureAPI::addEpic("Platform Testing");
	AllureAPI::addFeature("Integration Tests");
	AllureAPI::addStory("End-to-end user flow");
	AllureAPI::addSeverity("critical");
	AllureAPI::addOwner("qa-lead@example.com");
	AllureAPI::addTag("integration");
	AllureAPI::addTag("e2e");

	// Parameters
	AllureAPI::addParameter("testEnv", "staging");
	AllureAPI::addParameter("browser", "Chrome 120");
	AllureAPI::addMaskedParameter("authToken", "Bearer abc123...");

	AllureAPI::addAction("Execute step 1", []() {
		AllureAPI::addTextAttachment("Step 1 Output", "Step 1 completed successfully");
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});

	AllureAPI::addAction("Execute step 2", []() {
		AllureAPI::addTextAttachment("Step 2 Output", "Step 2 completed successfully");
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	});

	AllureAPI::addExpectedResult("All steps should pass", []() {
		EXPECT_TRUE(true);
	});

	// Final attachment
	AllureAPI::addTextAttachment("Test Summary", "All features demonstrated successfully");
}

TEST_F(AdvancedFeaturesTestSuite, testWithNestedStepsAndAttachments)
{
	AllureAPI::setTestCaseName("Nested steps with attachments at each level");
	AllureAPI::setTestCaseDescription("Shows how attachments can be added to nested steps.");

	AllureAPI::addAction("Main workflow", []() {
		AllureAPI::addTextAttachment("Workflow started", "Main workflow initialization");

		AllureAPI::beginSubStep("Initialize database");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		AllureAPI::addTextAttachment("DB Connection", "Connected to: test-db.example.com:5432");
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Load test data");
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		AllureAPI::addTextAttachment("Data Loaded", "Loaded 1000 test records");
		AllureAPI::endSubStep();

		AllureAPI::beginSubStep("Execute business logic");
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		AllureAPI::addTextAttachment("Processing Results", "Processed 950/1000 records successfully");
		AllureAPI::endSubStep();
	});

	AllureAPI::addExpectedResult("Workflow should complete", []() {
		EXPECT_TRUE(true);
	});
}

// ==============================================================================
// Main function
// ==============================================================================

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	// Initialize Allure with new API
	AllureAPI::initializeGoogleTest();
	AllureAPI::setTestProgramName("Allure Features Demo");
	AllureAPI::setOutputFolder("allure-results-features-demo");
	AllureAPI::setFormat(allure_cpp::model::Format::DEFAULT);

	return RUN_ALL_TESTS();
}
