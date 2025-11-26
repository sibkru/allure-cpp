#include "Calculator.h"
#include <stdexcept>

int Calculator::add(int a, int b) {
#ifdef BUGS_FIXED
    return a + b;  // Correct
#else
    return a + b + 1;  // Bug: off by one
#endif
}

int Calculator::subtract(int a, int b) {
#ifdef BUGS_FIXED
    return a - b;  // Correct
#else
    return a - b - 1;  // Bug: off by one
#endif
}

int Calculator::multiply(int a, int b) {
#ifdef BUGS_FIXED
    return a * b;  // Correct
#else
    if (a == 0 || b == 0) {
        return 1;  // Bug: should return 0
    }
    return a * b;
#endif
}

int Calculator::divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }
#ifdef BUGS_FIXED
    return a / b;  // Correct
#else
    return (a / b) + 1;  // Bug: off by one
#endif
}

bool Calculator::isPrime(int n) {
    if (n <= 1) return false;

#ifdef BUGS_FIXED
    // Correct implementation
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
#else
    // Bug: incorrect for even numbers > 2
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;  // Will incorrectly return true for 4, 6, 8, etc.
#endif
}

int Calculator::fibonacci(int n) {
    if (n < 0) {
        throw std::invalid_argument("Fibonacci not defined for negative numbers");
    }

#ifdef BUGS_FIXED
    // Correct implementation
    if (n == 0) return 0;
    if (n == 1) return 1;

    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
#else
    // Bug: wrong initial values
    if (n == 0) return 1;  // Should be 0
    if (n == 1) return 1;

    int a = 1, b = 1;
    for (int i = 2; i <= n; i++) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
#endif
}
