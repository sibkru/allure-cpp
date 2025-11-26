#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace allure {

/**
 * Attachment builder for adding files, screenshots, logs to test reports.
 * Attachments are added to the current test or step.
 *
 * Example:
 *   Attachment::fromText("log", "test output").attach();
 *   Attachment::fromFile("screenshot", "/tmp/screen.png").attach();
 */
class Attachment {
public:
    // Factory methods for creating attachments
    static Attachment fromBinary(std::string_view name, std::string_view mimeType,
                                const void* data, size_t size);
    static Attachment fromText(std::string_view name, std::string_view content);
    static Attachment fromFile(std::string_view name, std::string_view filePath);

    // Add this attachment to the current test/step
    void attach();

private:
    Attachment() = default;

    std::string m_name;
    std::string m_type;
    std::vector<char> m_data;
};

// Convenience free functions (simplified API)
void attachText(std::string_view name, std::string_view content);
void attachFile(std::string_view name, std::string_view filePath);

} // namespace allure
