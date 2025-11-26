#pragma once

#include "../Model/TestProgram.h"
#include "../Framework/ITestStatusProvider.h"
#include "../Framework/ITestFrameworkAdapter.h"
#include "../Services/IServicesFactory.h"

#include <memory>

namespace allure {
namespace detail {

/**
 * Core infrastructure for the Allure API.
 * Provides access to global state (TestProgram, ServicesFactory, FrameworkAdapter).
 * This replaces the static members from the old AllureAPI class.
 */
class Core {
public:
    // Get the singleton instance
    static Core& instance();

    // Access to test program (main model)
    model::TestProgram& getTestProgram();

    // Access to services factory
    service::IServicesFactory* getServicesFactory();

    // Access to status provider (from framework adapter)
    std::unique_ptr<ITestStatusProvider> getStatusProvider();

    // Set framework adapter (called by AllureGTest/AllureCppUTest)
    void setFrameworkAdapter(std::shared_ptr<ITestFrameworkAdapter> adapter);

private:
    Core();
    ~Core() = default;

    // Delete copy and move
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;
    Core(Core&&) = delete;
    Core& operator=(Core&&) = delete;

    model::TestProgram m_testProgram;
    std::unique_ptr<service::IServicesFactory> m_servicesFactory;
    std::shared_ptr<ITestFrameworkAdapter> m_frameworkAdapter;
};

// Convenience accessors for internal use by new API
inline model::TestProgram& getTestProgram() {
    return Core::instance().getTestProgram();
}

inline service::IServicesFactory* getServicesFactory() {
    return Core::instance().getServicesFactory();
}

inline std::unique_ptr<ITestStatusProvider> getStatusProvider() {
    return Core::instance().getStatusProvider();
}

} // namespace detail
} // namespace allure
