#include "GTestAllureUtilities/AllureAPI.h"
#include <gtest/gtest.h>

/**
 * @brief Example using the NEW API (initializeGoogleTest)
 *
 * This demonstrates the new framework-agnostic way of initializing Allure reporting
 * with GoogleTest. This is the recommended approach going forward.
 */

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	// Configure Allure output
	allure_cpp::AllureAPI::setOutputFolder("./allure-results-new-api");
	allure_cpp::AllureAPI::setTestProgramName("API-Comparison-NewAPI");
	allure_cpp::AllureAPI::setTMSLinksPattern("https://github.com/issue/{}");

	// NEW API: Initialize with GoogleTest adapter (automatically registers listener)
	allure_cpp::AllureAPI::initializeGoogleTest();

	// Run all tests
	return RUN_ALL_TESTS();
}
