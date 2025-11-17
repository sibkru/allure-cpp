#include "stdafx.h"
#include "AllureAPI.h"


int main(int argc, char* argv[])
{
	::testing::FLAGS_gmock_verbose = "error";
	::testing::InitGoogleTest(&argc, argv);

	allure_cpp::AllureAPI::setTestProgramName("SampleTestProject");
	allure_cpp::AllureAPI::setOutputFolder("Reports/Allure");
	allure_cpp::AllureAPI::setTMSLinksPattern("https://mycompany.com/tms/{}");
	
	::testing::UnitTest::GetInstance()->listeners().Append(allure_cpp::AllureAPI::buildListener().release());

	int res = RUN_ALL_TESTS();

	return res;
}
