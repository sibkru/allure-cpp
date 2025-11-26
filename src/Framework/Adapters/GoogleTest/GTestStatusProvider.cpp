#include "Framework/Adapters/GoogleTest/GTestStatusProvider.h"


namespace allure {
namespace adapters {
namespace googletest {

	bool GTestStatusProvider::isCurrentTestFailed() const
	{
		// GoogleTest provides a static method to check if the current test has failed
		return ::testing::Test::HasFailure();
	}

	bool GTestStatusProvider::isCurrentTestSkipped() const
	{
		// GoogleTest 1.10+ provides IsSkipped() to check if the test was skipped
		// (e.g., via GTEST_SKIP() or DISABLED_ prefix)
		return ::testing::Test::IsSkipped();
	}

}}} // namespace allure::adapters::googletest
