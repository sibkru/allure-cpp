#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>

namespace allure {
namespace detail {

/**
 * Format utilities for string formatting with fmt library.
 * These helpers make it easy to format step names and other strings.
 */

// Format a string using fmt syntax
template<typename... Args>
inline std::string format(fmt::format_string<Args...> fmt_str, Args&&... args) {
    return fmt::format(fmt_str, std::forward<Args>(args)...);
}

} // namespace detail
} // namespace allure
