#include <gtest/gtest.h>
#include "Framework/Adapters/GoogleTest/AllureGTest.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	allure_cpp::adapters::googletest::AllureGTest allureHelper;
	return RUN_ALL_TESTS();
}
