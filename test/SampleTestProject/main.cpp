#include "stdafx.h"
#include "AllureAPI.h"
#include "Framework/Adapters/GoogleTest/AllureGTest.h"


int main(int argc, char* argv[])
{
	::testing::FLAGS_gmock_verbose = "error";
	::testing::InitGoogleTest(&argc, argv);

	allure_cpp::AllureAPI::setTestProgramName("SampleTestProject");
	allure_cpp::AllureAPI::setOutputFolder("Reports/Allure");
	allure_cpp::AllureAPI::setTMSLinksPattern("https://mycompany.com/tms/{}");

	allure_cpp::adapters::googletest::AllureGTest allureHelper;

	int res = RUN_ALL_TESTS();

	return res;
}
