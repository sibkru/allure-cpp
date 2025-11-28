#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>

namespace allure {
namespace detail {

/**
 * @file Utils.h
 * @brief Thin wrappers around fmt utilities used by the public API.
 */

/**
 * Format utilities for string formatting with fmt library.
 * These helpers make it easy to format step names and other strings.
 */

// Format a string using fmt syntax
/**
 * Format a string using fmt syntax.
 * @tparam Args Variadic argument types.
 * @param fmt_str Format string checked by fmt at compile time.
 * @param args Arguments to substitute into the format string.
 * @return A formatted std::string.
 */
template<typename... Args>
inline std::string format(fmt::format_string<Args...> fmt_str, Args&&... args) {
    return fmt::format(fmt_str, std::forward<Args>(args)...);
}

} // namespace detail
} // namespace allure
