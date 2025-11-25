#include "Framework/Adapters/CppUTest/CppUTestStatusProvider.h"
#include <CppUTest/TestHarness.h>


namespace allure_cpp {
namespace adapters {
namespace cpputest {

	bool CppUTestStatusProvider::isCurrentTestFailed() const
	{
		// CppUTest doesn't provide a global method to check current test status
		// during execution like GoogleTest does. We can check the global test result
		// but this is limited. Return false as we can't reliably determine this.
		// This is a known limitation of CppUTest.
		return false;
	}

	bool CppUTestStatusProvider::isCurrentTestSkipped() const
	{
		// CppUTest doesn't provide a way to check if the current test is skipped
		// during execution. IGNORE_TEST causes the test not to run at all.
		return false;
	}

}}} // namespace allure_cpp::adapters::cpputest
