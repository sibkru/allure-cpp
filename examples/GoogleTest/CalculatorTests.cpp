#include <allure-cpp.h>
#include <gtest/gtest.h>
#include "../shared/Calculator.h"

using namespace allure;

/**
 * Tests that use the Calculator class - demonstrates modern API usage
 */

class CalculatorTestSuite : public testing::Test
{
public:
	static void SetUpTestSuite()
	{
		suite()
			.name("Calculator Test Suite")
			.description("Tests for calculator functionality - demonstrates trend history")
			.epic("Calculator Module")
			.severity("critical");
	}
};

TEST_F(CalculatorTestSuite, testAddition)
{
	test()
		.name("Test addition operation")
		.feature("Arithmetic")
		.story("User can add two numbers");

	int result = Calculator::add(2, 3);
	EXPECT_EQ(5, result) << "2 + 3 should equal 5";
}

TEST_F(CalculatorTestSuite, testSubtraction)
{
	test()
		.name("Test subtraction operation")
		.feature("Arithmetic")
		.story("User can subtract two numbers");

	int result = Calculator::subtract(10, 4);
	EXPECT_EQ(6, result) << "10 - 4 should equal 6";
}

TEST_F(CalculatorTestSuite, testMultiplication)
{
	test()
		.name("Test multiplication operation")
		.feature("Arithmetic")
		.story("User can multiply two numbers");

	int result = Calculator::multiply(5, 0);
	EXPECT_EQ(0, result) << "5 * 0 should equal 0";
}

TEST_F(CalculatorTestSuite, testDivision)
{
	test()
		.name("Test division operation")
		.feature("Arithmetic")
		.story("User can divide two numbers");

	int result = Calculator::divide(10, 2);
	EXPECT_EQ(5, result) << "10 / 2 should equal 5";
}

TEST_F(CalculatorTestSuite, testPrimeCheck)
{
	test()
		.name("Test prime number detection")
		.feature("Number Theory")
		.story("User can check if a number is prime");

	EXPECT_TRUE(Calculator::isPrime(7)) << "7 is prime";
	EXPECT_FALSE(Calculator::isPrime(4)) << "4 is not prime";
	EXPECT_TRUE(Calculator::isPrime(13)) << "13 is prime";
}

TEST_F(CalculatorTestSuite, testFibonacci)
{
	test()
		.name("Test Fibonacci sequence")
		.feature("Number Theory")
		.story("User can calculate Fibonacci numbers");

	EXPECT_EQ(0, Calculator::fibonacci(0)) << "fib(0) = 0";
	EXPECT_EQ(1, Calculator::fibonacci(1)) << "fib(1) = 1";
	EXPECT_EQ(1, Calculator::fibonacci(2)) << "fib(2) = 1";
	EXPECT_EQ(2, Calculator::fibonacci(3)) << "fib(3) = 2";
	EXPECT_EQ(5, Calculator::fibonacci(5)) << "fib(5) = 5";
}
