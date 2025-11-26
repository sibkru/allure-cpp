#include "../shared/Calculator.h"
#include "AllureAPI.h"
#include <CppUTest/TestHarness.h>

/**
 * Tests that use the Calculator class - some will fail initially and pass after BUGS_FIXED is defined
 */

TEST_GROUP(CalculatorTestSuite)
{
    void setup() override
    {
        allure_cpp::AllureAPI::setTestSuiteName("Calculator Test Suite");
        allure_cpp::AllureAPI::setTestSuiteDescription("Tests for calculator functionality - demonstrates trend history");
        allure_cpp::AllureAPI::setTestSuiteEpic("Calculator Module");
        allure_cpp::AllureAPI::setTestSuiteSeverity("critical");
    }
};

TEST(CalculatorTestSuite, testAddition)
{
    allure_cpp::AllureAPI::setTestCaseName("Test addition operation");
    allure_cpp::AllureAPI::addFeature("Arithmetic");
    allure_cpp::AllureAPI::addStory("User can add two numbers");

    int result = Calculator::add(2, 3);
    CHECK_EQUAL(5, result);
}

TEST(CalculatorTestSuite, testSubtraction)
{
    allure_cpp::AllureAPI::setTestCaseName("Test subtraction operation");
    allure_cpp::AllureAPI::addFeature("Arithmetic");
    allure_cpp::AllureAPI::addStory("User can subtract two numbers");

    int result = Calculator::subtract(10, 4);
    CHECK_EQUAL(6, result);
}

TEST(CalculatorTestSuite, testMultiplication)
{
    allure_cpp::AllureAPI::setTestCaseName("Test multiplication operation");
    allure_cpp::AllureAPI::addFeature("Arithmetic");
    allure_cpp::AllureAPI::addStory("User can multiply two numbers");

    int result = Calculator::multiply(5, 0);
    CHECK_EQUAL(0, result);
}

TEST(CalculatorTestSuite, testDivision)
{
    allure_cpp::AllureAPI::setTestCaseName("Test division operation");
    allure_cpp::AllureAPI::addFeature("Arithmetic");
    allure_cpp::AllureAPI::addStory("User can divide two numbers");

    int result = Calculator::divide(10, 2);
    CHECK_EQUAL(5, result);
}

TEST(CalculatorTestSuite, testPrimeCheck)
{
    allure_cpp::AllureAPI::setTestCaseName("Test prime number detection");
    allure_cpp::AllureAPI::addFeature("Number Theory");
    allure_cpp::AllureAPI::addStory("User can check if a number is prime");

    CHECK_TRUE(Calculator::isPrime(7));
    CHECK_FALSE(Calculator::isPrime(4));
    CHECK_TRUE(Calculator::isPrime(13));
}

TEST(CalculatorTestSuite, testFibonacci)
{
    allure_cpp::AllureAPI::setTestCaseName("Test Fibonacci sequence");
    allure_cpp::AllureAPI::addFeature("Number Theory");
    allure_cpp::AllureAPI::addStory("User can calculate Fibonacci numbers");

    CHECK_EQUAL(0, Calculator::fibonacci(0));
    CHECK_EQUAL(1, Calculator::fibonacci(1));
    CHECK_EQUAL(1, Calculator::fibonacci(2));
    CHECK_EQUAL(2, Calculator::fibonacci(3));
    CHECK_EQUAL(5, Calculator::fibonacci(5));
}
