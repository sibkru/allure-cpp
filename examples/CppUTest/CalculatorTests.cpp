#include "../shared/Calculator.h"
#include <allure-cpp.h>
#include <CppUTest/TestHarness.h>

using namespace allure;

/**
 * Tests that use the Calculator class - demonstrates modern API usage
 */

TEST_GROUP(CalculatorTestSuite)
{
    void setup() override
    {
        suite()
			.name("Calculator Test Suite")
			.description("Tests for calculator functionality - demonstrates trend history")
			.epic("Calculator Module")
			.severity("critical");
    }
};

TEST(CalculatorTestSuite, testAddition)
{
    test()
		.name("Test addition operation")
		.feature("Arithmetic")
		.story("User can add two numbers");

    int result = Calculator::add(2, 3);
    CHECK_EQUAL(5, result);
}

TEST(CalculatorTestSuite, testSubtraction)
{
    test()
		.name("Test subtraction operation")
		.feature("Arithmetic")
		.story("User can subtract two numbers");

    int result = Calculator::subtract(10, 4);
    CHECK_EQUAL(6, result);
}

TEST(CalculatorTestSuite, testMultiplication)
{
    test()
		.name("Test multiplication operation")
		.feature("Arithmetic")
		.story("User can multiply two numbers");

    int result = Calculator::multiply(5, 0);
    CHECK_EQUAL(0, result);
}

TEST(CalculatorTestSuite, testDivision)
{
    test()
		.name("Test division operation")
		.feature("Arithmetic")
		.story("User can divide two numbers");

    int result = Calculator::divide(10, 2);
    CHECK_EQUAL(5, result);
}

TEST(CalculatorTestSuite, testPrimeCheck)
{
    test()
		.name("Test prime number detection")
		.feature("Number Theory")
		.story("User can check if a number is prime");

    CHECK_TRUE(Calculator::isPrime(7));
    CHECK_FALSE(Calculator::isPrime(4));
    CHECK_TRUE(Calculator::isPrime(13));
}

TEST(CalculatorTestSuite, testFibonacci)
{
    test()
		.name("Test Fibonacci sequence")
		.feature("Number Theory")
		.story("User can calculate Fibonacci numbers");

    CHECK_EQUAL(0, Calculator::fibonacci(0));
    CHECK_EQUAL(1, Calculator::fibonacci(1));
    CHECK_EQUAL(1, Calculator::fibonacci(2));
    CHECK_EQUAL(2, Calculator::fibonacci(3));
    CHECK_EQUAL(5, Calculator::fibonacci(5));
}
