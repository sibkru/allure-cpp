#include "TestMetadata.h"
#include "Core.h"
#include "../Model/TestCase.h"
#include "../Model/Label.h"
#include "../Model/Link.h"
#include "../Model/Parameter.h"
#include "../Services/Property/ITestSuitePropertySetter.h"
#include "../Model/TestProperty.h"

namespace allure {

// ============================================================================
// TestMetadata Implementation
// ============================================================================

TestMetadata::~TestMetadata() {
    // Apply all operations when builder is destroyed
    for (auto& op : m_operations) {
        op();
    }
}

TestMetadata& TestMetadata::name(std::string_view name) {
    m_operations.push_back([name = std::string(name)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setName(name);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::description(std::string_view desc) {
    m_operations.push_back([desc = std::string(desc)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setDescription(desc);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::descriptionHtml(std::string_view html) {
    m_operations.push_back([html = std::string(html)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setDescriptionHtml(html);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::epic(std::string_view epic) {
    return label("epic", epic);
}

TestMetadata& TestMetadata::feature(std::string_view feature) {
    return label("feature", feature);
}

TestMetadata& TestMetadata::story(std::string_view story) {
    return label("story", story);
}

TestMetadata& TestMetadata::severity(std::string_view severity) {
    return label("severity", severity);
}

TestMetadata& TestMetadata::owner(std::string_view owner) {
    return label("owner", owner);
}

TestMetadata& TestMetadata::tag(std::string_view tag) {
    return label("tag", tag);
}

TestMetadata& TestMetadata::label(std::string_view name, std::string_view value) {
    m_operations.push_back([name = std::string(name), value = std::string(value)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            model::Label label;
            label.setName(name);
            label.setValue(value);
            testCase->addLabel(label);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::link(std::string_view name, std::string_view url, std::string_view type) {
    m_operations.push_back([name = std::string(name), url = std::string(url), type = std::string(type)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            model::Link link;
            link.setName(name);
            link.setURL(url);
            link.setType(type);
            testCase->addLink(link);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::issue(std::string_view name, std::string_view url) {
    return link(name, url, "issue");
}

TestMetadata& TestMetadata::tms(std::string_view name, std::string_view url) {
    return link(name, url, "tms");
}

TestMetadata& TestMetadata::parameter(std::string_view name, std::string_view value) {
    m_operations.push_back([name = std::string(name), value = std::string(value)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            model::Parameter param;
            param.setName(name);
            param.setValue(value);
            param.setExcluded(false);
            param.setMode("default");
            testCase->addParameter(param);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::maskedParameter(std::string_view name, std::string_view value) {
    m_operations.push_back([name = std::string(name), value = std::string(value)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            model::Parameter param;
            param.setName(name);
            param.setValue(value);
            param.setExcluded(false);
            param.setMode("masked");
            testCase->addParameter(param);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::hiddenParameter(std::string_view name, std::string_view value) {
    m_operations.push_back([name = std::string(name), value = std::string(value)]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            model::Parameter param;
            param.setName(name);
            param.setValue(value);
            param.setExcluded(false);
            param.setMode("hidden");
            testCase->addParameter(param);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::flaky() {
    m_operations.push_back([]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setStatusFlaky(true);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::known() {
    m_operations.push_back([]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setStatusKnown(true);
        }
    });
    return *this;
}

TestMetadata& TestMetadata::muted() {
    m_operations.push_back([]() {
        auto* testCase = detail::getTestProgram().getRunningTestCase();
        if (testCase) {
            testCase->setStatusMuted(true);
        }
    });
    return *this;
}

// ============================================================================
// SuiteMetadata Implementation
// ============================================================================

SuiteMetadata::~SuiteMetadata() {
    // Apply all operations when builder is destroyed
    for (auto& op : m_operations) {
        op();
    }
}

SuiteMetadata& SuiteMetadata::name(std::string_view name) {
    m_operations.push_back([name = std::string(name)]() {
        auto factory = detail::getServicesFactory();
        auto setter = factory->buildTestSuitePropertySetter();
        setter->setProperty(model::test_property::NAME_PROPERTY, name);
    });
    return *this;
}

SuiteMetadata& SuiteMetadata::description(std::string_view desc) {
    m_operations.push_back([desc = std::string(desc)]() {
        auto factory = detail::getServicesFactory();
        auto setter = factory->buildTestSuitePropertySetter();
        setter->setProperty(model::test_property::FEATURE_PROPERTY, desc);
    });
    return *this;
}

SuiteMetadata& SuiteMetadata::epic(std::string_view epic) {
    return label(model::test_property::EPIC_PROPERTY, epic);
}

SuiteMetadata& SuiteMetadata::severity(std::string_view severity) {
    return label(model::test_property::SEVERITY_PROPERTY, severity);
}

SuiteMetadata& SuiteMetadata::label(std::string_view name, std::string_view value) {
    m_operations.push_back([name = std::string(name), value = std::string(value)]() {
        auto factory = detail::getServicesFactory();
        auto setter = factory->buildTestSuitePropertySetter();
        setter->setProperty(name, value);
    });
    return *this;
}

} // namespace allure
