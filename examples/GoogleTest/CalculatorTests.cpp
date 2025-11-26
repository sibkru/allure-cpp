#include "AllureAPI.h"
#include <gtest/gtest.h>
#include "../shared/Calculator.h"

using namespace allure_cpp;

/**
 * Tests that use the Calculator class - some will fail initially and pass after BUGS_FIXED is defined
 */

class CalculatorTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		AllureAPI::setTestSuiteName("Calculator Test Suite");
		AllureAPI::setTestSuiteDescription("Tests for calculator functionality - demonstrates trend history");
		AllureAPI::setTestSuiteEpic("Calculator Module");
		AllureAPI::setTestSuiteSeverity("critical");
	}
};

TEST_F(CalculatorTestSuite, testAddition)
{
	AllureAPI::setTestCaseName("Test addition operation");
	AllureAPI::addFeature("Arithmetic");
	AllureAPI::addStory("User can add two numbers");

	int result = Calculator::add(2, 3);
	EXPECT_EQ(5, result) << "2 + 3 should equal 5";
}

TEST_F(CalculatorTestSuite, testSubtraction)
{
	AllureAPI::setTestCaseName("Test subtraction operation");
	AllureAPI::addFeature("Arithmetic");
	AllureAPI::addStory("User can subtract two numbers");

	int result = Calculator::subtract(10, 4);
	EXPECT_EQ(6, result) << "10 - 4 should equal 6";
}

TEST_F(CalculatorTestSuite, testMultiplication)
{
	AllureAPI::setTestCaseName("Test multiplication operation");
	AllureAPI::addFeature("Arithmetic");
	AllureAPI::addStory("User can multiply two numbers");

	int result = Calculator::multiply(5, 0);
	EXPECT_EQ(0, result) << "5 * 0 should equal 0";
}

TEST_F(CalculatorTestSuite, testDivision)
{
	AllureAPI::setTestCaseName("Test division operation");
	AllureAPI::addFeature("Arithmetic");
	AllureAPI::addStory("User can divide two numbers");

	int result = Calculator::divide(10, 2);
	EXPECT_EQ(5, result) << "10 / 2 should equal 5";
}

TEST_F(CalculatorTestSuite, testPrimeCheck)
{
	AllureAPI::setTestCaseName("Test prime number detection");
	AllureAPI::addFeature("Number Theory");
	AllureAPI::addStory("User can check if a number is prime");

	EXPECT_TRUE(Calculator::isPrime(7)) << "7 is prime";
	EXPECT_FALSE(Calculator::isPrime(4)) << "4 is not prime";
	EXPECT_TRUE(Calculator::isPrime(13)) << "13 is prime";
}

TEST_F(CalculatorTestSuite, testFibonacci)
{
	AllureAPI::setTestCaseName("Test Fibonacci sequence");
	AllureAPI::addFeature("Number Theory");
	AllureAPI::addStory("User can calculate Fibonacci numbers");

	EXPECT_EQ(0, Calculator::fibonacci(0)) << "fib(0) = 0";
	EXPECT_EQ(1, Calculator::fibonacci(1)) << "fib(1) = 1";
	EXPECT_EQ(1, Calculator::fibonacci(2)) << "fib(2) = 1";
	EXPECT_EQ(2, Calculator::fibonacci(3)) << "fib(3) = 2";
	EXPECT_EQ(5, Calculator::fibonacci(5)) << "fib(5) = 5";
}
