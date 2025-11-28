#pragma once

#include "../Model/TestProgram.h"
#include "../Framework/ITestStatusProvider.h"
#include "../Framework/ITestFrameworkAdapter.h"
#include "../Services/IServicesFactory.h"

#include <memory>

namespace allure {
namespace detail {

/**
 * @brief Core infrastructure for the Allure API.
 *
 * This singleton class provides access to global state, including the
 * TestProgram model, the services factory, and the test framework adapter.
 * The singleton is lazily created on first use; calls are expected to happen
 * from the test runner thread(s) only.
 * It replaces the static members from the old AllureAPI class.
 */
class Core {
public:
    /**
     * @brief Gets the singleton instance of the Core.
     * @return Reference to the singleton Core instance.
     */
    static Core& instance();

    /**
     * @brief Gets the test program model.
     * @return Reference to the TestProgram object, which is the root of the test model.
     */
    model::TestProgram& getTestProgram();

    /**
     * @brief Gets the services factory.
     * @return Pointer to the IServicesFactory implementation (non-owning).
     */
    service::IServicesFactory* getServicesFactory();

    /**
     * @brief Gets a status provider from the current framework adapter.
     * @return A unique_ptr to a new ITestStatusProvider.
     */
    std::unique_ptr<ITestStatusProvider> getStatusProvider();

    /**
     * @brief Sets the test framework adapter.
     *
     * This is typically called by a framework-specific listener like
     * AllureGoogleTest or AllureCppUTest to integrate with the Allure lifecycle.
     * Call this before tests start running so lifecycle events are routed
     * correctly.
     * @param adapter A shared_ptr to the ITestFrameworkAdapter implementation.
     */
    void setFrameworkAdapter(std::shared_ptr<ITestFrameworkAdapter> adapter);

private:
    Core();
    ~Core() = default;

    // Delete copy and move constructors and assignment operators
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;
    Core(Core&&) = delete;
    Core& operator=(Core&&) = delete;

    model::TestProgram m_testProgram; ///< The root of the Allure test model.
    std::unique_ptr<service::IServicesFactory> m_servicesFactory; ///< The factory for creating services.
    std::shared_ptr<ITestFrameworkAdapter> m_frameworkAdapter; ///< The adapter for the current test framework.
};

// Convenience accessors for internal use by new API

/**
 * @brief Convenience function to get the global TestProgram.
 * @return Reference to the TestProgram.
 */
inline model::TestProgram& getTestProgram() {
    return Core::instance().getTestProgram();
}

/**
 * @brief Convenience function to get the global services factory.
 * @return Pointer to the IServicesFactory.
 */
inline service::IServicesFactory* getServicesFactory() {
    return Core::instance().getServicesFactory();
}

/**
     * @brief Convenience function to get a status provider.
     * @return A unique_ptr to a new ITestStatusProvider.
     */
    inline std::unique_ptr<ITestStatusProvider> getStatusProvider() {
        return Core::instance().getStatusProvider();
    }

} // namespace detail
} // namespace allure
