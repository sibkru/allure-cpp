#include "StepGuard.h"
#include "Core.h"
#include "../Model/Status.h"
#include "../Services/EventHandlers/ITestStepStartEventHandler.h"
#include "../Services/EventHandlers/ITestStepEndEventHandler.h"

namespace allure {

namespace {
    // Convert status provider to model status
    model::Status convertStatus(const ITestStatusProvider& provider) {
        if (provider.isCurrentTestSkipped()) {
            return model::Status::SKIPPED;
        }
        if (provider.isCurrentTestFailed()) {
            return model::Status::FAILED;
        }
        return model::Status::PASSED;
    }
}

StepGuard::StepGuard(std::string_view name)
    : m_active(true)
{
    auto factory = detail::getServicesFactory();
    auto handler = factory->buildTestStepStartEventHandler();
    handler->handleTestStepStart(std::string(name), true);  // true = isAction
}

StepGuard::~StepGuard() noexcept {
    if (!m_active) {
        return;  // Moved-from guard, don't end step
    }

    try {
        auto statusProvider = detail::getStatusProvider();
        auto status = convertStatus(*statusProvider);

        auto factory = detail::getServicesFactory();
        auto handler = factory->buildTestStepEndEventHandler();
        handler->handleTestStepEnd(status);
    }
    catch (...) {
        // Destructor must not throw
        // Silently ignore errors during cleanup
    }
}

StepGuard::StepGuard(StepGuard&& other) noexcept
    : m_active(other.m_active)
{
    other.m_active = false;  // Prevent double-cleanup
}

StepGuard& StepGuard::operator=(StepGuard&& other) noexcept {
    if (this != &other) {
        m_active = other.m_active;
        other.m_active = false;
    }
    return *this;
}

} // namespace allure
