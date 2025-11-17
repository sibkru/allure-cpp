#include <gtest/gtest.h>
#include "AllureAPI.h"

int main(int argc, char** argv)
{
	// Initialize Google Test and Allure
	::testing::InitGoogleTest(&argc, argv);

	// Configure Allure to output to allure-results directory
	allure_cpp::AllureAPI::setOutputFolder("allure-results");
	allure_cpp::AllureAPI::initializeGoogleTest();

	return RUN_ALL_TESTS();
}
