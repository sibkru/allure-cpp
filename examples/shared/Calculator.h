#pragma once

// Toggle this to simulate bug fixes across runs
// Run 1: Undefined (has bugs)
// Run 2: Defined (bugs fixed)
#define BUGS_FIXED

class Calculator {
public:
    static int add(int a, int b);
    static int subtract(int a, int b);
    static int multiply(int a, int b);
    static int divide(int a, int b);
    static bool isPrime(int n);
    static int fibonacci(int n);
};
