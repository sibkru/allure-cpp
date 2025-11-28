#pragma once

#include <string>
#include <string_view>

namespace allure {

/**
 * @file StepGuard.h
 * @brief RAII guard that manages the lifetime of an Allure step.
 */

/**
 * RAII guard for automatic step lifecycle management.
 *
 * The step starts when the guard is constructed and automatically ends
 * when the guard is destroyed (goes out of scope).
 * If a guard is moved, only the destination guard remains active.
 *
 * Example:
 *   {
 *       StepGuard guard("My step");
 *       // ... test code ...
 *   } // Step automatically ends here
 */
class StepGuard {
public:
    /**
     * Construct a step guard and start the step.
     * @param name The name of the step.
     */
    explicit StepGuard(std::string_view name);

    /**
     * Destructor automatically ends the step.
     * This is noexcept to prevent exceptions during stack unwinding.
     */
    ~StepGuard() noexcept;

    // Non-copyable
    StepGuard(const StepGuard&) = delete;
    StepGuard& operator=(const StepGuard&) = delete;

    // Movable (to support returning from functions)
    StepGuard(StepGuard&& other) noexcept;
    StepGuard& operator=(StepGuard&& other) noexcept;

private:
    bool m_active{true};  ///< True if the guard is active. Becomes false if it has been moved from.
};

} // namespace allure
