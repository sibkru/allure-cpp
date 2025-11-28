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
 * @file StepFunctions.h
 * @brief This file contains functions for creating Allure test steps using RAII guards.
 *
 * There are two primary ways to create a step:
 * 1. Manual RAII Guard: The step starts when a `StepGuard` is created and ends when it goes out of scope.
 *    @code
 *    {
 *        auto guard = allure::step("My Step");
 *        // ... code for the step ...
 *    } // Step ends here
 *    @endcode
 * 2. Lambda-based: A lambda function containing the step's code is passed to the `step` function.
 *    @code
 *    allure::step("My Step", [&]() {
 *        // ... code for the step ...
 *    });
 *    @endcode
 *
 * The `step` functions also support `fmt`-style format strings for dynamic step names.
 * Steps are closed automatically, even when the callable throws.
 */

// ============================================================================
// Manual RAII guard (user controls scope)
// ============================================================================

/**
 * @brief Starts a test step and returns a guard object.
 *
 * The step is considered active until the returned `StepGuard` is destroyed.
 * This is useful for steps that span a clear lexical scope.
 *
 * @param name The name of the step. Keep the guard alive for the full step scope.
 * @return A `StepGuard` object that controls the lifetime of the step.
 */
[[nodiscard]] inline StepGuard step(std::string_view name) {
    return StepGuard(name);
}

// ============================================================================
// Lambda-based steps (automatic scoping)
// ============================================================================

/**
 * @brief Executes a callable (e.g., a lambda) within a test step.
 *
 * The step automatically starts before the callable is invoked and ends after it completes.
 *
 * @tparam Func Callable type; must be invocable with no arguments.
 * @param name The name of the step.
 * @param func The callable object to execute as the body of the step.
 */
template<typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(std::string_view name, Func&& func) {
    StepGuard guard(name);
    std::forward<Func>(func)();
}

// ============================================================================
// Formatted step names (with arguments)
// ============================================================================

/**
 * @brief Executes a callable within a step with a formatted name (1 argument).
 *
 * @tparam Arg1 Type of the first format argument.
 * @tparam Func Callable type; must be invocable with no arguments.
 * @param fmt_str A `fmt`-style format string for the step name.
 * @param arg1 The first argument to format into the step name.
 * @param func The callable object to execute as the body of the step.
 */
template<typename Arg1, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1)));
    std::forward<Func>(func)();
}

/**
 * @brief Executes a callable within a step with a formatted name (2 arguments).
 *
 * @tparam Arg1 Type of the first format argument.
 * @tparam Arg2 Type of the second format argument.
 * @tparam Func Callable type; must be invocable with no arguments.
 * @param fmt_str A `fmt`-style format string for the step name.
 * @param arg1 The first argument to format into the step name.
 * @param arg2 The second argument to format into the step name.
 * @param func The callable object to execute as the body of the step.
 */
template<typename Arg1, typename Arg2, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Arg2&& arg2, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));
    std::forward<Func>(func)();
}

/**
 * @brief Executes a callable within a step with a formatted name (3 arguments).
 *
 * @tparam Arg1 Type of the first format argument.
 * @tparam Arg2 Type of the second format argument.
 * @tparam Arg3 Type of the third format argument.
 * @tparam Func Callable type; must be invocable with no arguments.
 * @param fmt_str A `fmt`-style format string for the step name.
 * @param arg1 The first argument to format into the step name.
 * @param arg2 The second argument to format into the step name.
 * @param arg3 The third argument to format into the step name.
 * @param func The callable object to execute as the body of the step.
 */
template<typename Arg1, typename Arg2, typename Arg3, typename Func, typename = std::enable_if_t<detail::is_callable<std::decay_t<Func>>::value>>
inline void step(const char* fmt_str, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Func&& func) {
    StepGuard guard(detail::format(fmt::runtime(fmt_str), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3)));
    std::forward<Func>(func)();
}

} // namespace allure
