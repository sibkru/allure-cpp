#include "Core.h"
#include "../Services/ServicesFactory.h"

namespace allure {
namespace detail {

namespace {
    // Fallback provider used when no framework adapter is registered
    class NullStatusProvider : public ITestStatusProvider {
    public:
        bool isCurrentTestFailed() const override { return false; }
        bool isCurrentTestSkipped() const override { return false; }
    };
}

Core::Core()
    : m_testProgram()
    , m_servicesFactory(std::make_unique<service::ServicesFactory>(m_testProgram))
    , m_frameworkAdapter(nullptr)
{
}

Core& Core::instance() {
    static Core instance;
    return instance;
}

model::TestProgram& Core::getTestProgram() {
    return m_testProgram;
}

service::IServicesFactory* Core::getServicesFactory() {
    // Check for configured instance first (for dependency injection)
    auto configuredInstance = service::ServicesFactory::getInstance();
    if (configuredInstance) {
        return configuredInstance;
    }
    return m_servicesFactory.get();
}

std::unique_ptr<ITestStatusProvider> Core::getStatusProvider() {
    if (m_frameworkAdapter) {
        return m_frameworkAdapter->createStatusProvider();
    }
    // Fallback: no adapter registered, return a no-op provider
    return std::make_unique<NullStatusProvider>();
}

void Core::setFrameworkAdapter(std::shared_ptr<ITestFrameworkAdapter> adapter) {
    m_frameworkAdapter = std::move(adapter);
}

} // namespace detail
} // namespace allure
