#include "GTestAllureUtilities/AllureAPI.h"
#include <gtest/gtest.h>

/**
 * @brief Example using the OLD API (buildListener)
 *
 * This demonstrates the traditional way of initializing Allure reporting
 * with GoogleTest. This API is still fully supported for backward compatibility.
 */

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	// Configure Allure output
	allure_cpp::AllureAPI::setOutputFolder("./allure-results-old-api");
	allure_cpp::AllureAPI::setTestProgramName("API-Comparison-OldAPI");
	allure_cpp::AllureAPI::setTMSLinksPattern("https://github.com/issue/{}");

	// OLD API: Build listener and manually register with GoogleTest
	auto listener = allure_cpp::AllureAPI::buildListener();
	::testing::UnitTest::GetInstance()->listeners().Append(listener.release());

	// Run all tests
	return RUN_ALL_TESTS();
}
