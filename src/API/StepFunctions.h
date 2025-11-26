#pragma once

#include "StepGuard.h"
#include "Utils.h"
#include <fmt/format.h>
#include <string_view>
#include <type_traits>
#include <utility>

namespace allure {

namespace detail {
    // Helper to detect if a type is callable (lambda, function, functor)
    template<typename T, typename = void>
    struct is_callable : std::false_type {};

    template<typename T>
    struct is_callable<T, std::void_t<decltype(&T::operator())>> : std::true_type {};

    template<typename Ret, typename... Args>
    struct is_callable<Ret(*)(Args...)> : std::true_type {};

    template<typename Ret, typename... Args>
    struct is_callable<Ret(Args...)> : std::true_type {};
}

/**
 * Step functions for creating test steps with RAII guards.
 *
 * Two usage patterns:
 * 1. Manual guard: auto guard = step("name"); // ... code ...
 * 2. Lambda-based: step("name", [&]() { ... code ... });
 */

// ============================================================================
// Manual RAII guard (user controls scope)
// ============================================================================

/**
 * Create a step guard that starts a step and returns the guard.
 * The step ends when the guard goes out of scope.
 *
 * Example:
 *   auto guard = step("Login");
 *   // ... test code ...
 *   // Step ends when guard is destroyed
 */
[[nodiscard]] inline StepGuard step(std::string_view name) {
    return StepGuard(name);
}

// ============================================================================
// Lambda-based steps (automatic scoping)
// ============================================================================

/**
 * Execute a lambda within a step.
 * The step automatically starts and ends around the lambda execution.
 *
 * Example:
 *   step("Login", [&]() {
 *       loginPage.login("user", "pass");
 *   });
 */
template<typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(std::string_view name, Func&& func) {
    StepGuard guard(name);
    std::forward<Func>(func)();
}

// ============================================================================
// Formatted step names (with arguments)
// ============================================================================

// Formatted step with 1 arg + lambda
template<typename Arg1, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1)));
    std::forward<Func>(func)();
}

// Formatted step with 2 args + lambda
template<typename Arg1, typename Arg2, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Arg2&& arg2, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));
    std::forward<Func>(func)();
}

// Formatted step with 3 args + lambda
template<typename Arg1, typename Arg2, typename Arg3, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3)));
    std::forward<Func>(func)();
}

} // namespace allure
