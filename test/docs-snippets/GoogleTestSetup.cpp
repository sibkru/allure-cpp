// [snippet:googletest-main]
#include <gtest/gtest.h>
#include "allure-cpp.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    allure::adapters::googletest::AllureGTest allureHelper;
    return RUN_ALL_TESTS();
}
// [/snippet:googletest-main]
