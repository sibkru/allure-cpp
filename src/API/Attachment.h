#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace allure {

/**
 * @file Attachment.h
 * @brief Builders for adding attachments to the running test or step.
 */

/**
 * @brief Attachment builder for adding files, screenshots, and logs to test reports.
 *
 * Attachments are added to the current running test case or step.
 *
 * Example usage:
 * @code
 *   allure::Attachment::fromText("log", "test output").attach();
 *   allure::Attachment::fromFile("screenshot", "/tmp/screen.png").attach();
 * @endcode
 */
class Attachment {
public:
    /**
     * @brief Creates an attachment from a binary data buffer.
     * @param name The name of the attachment.
     * @param mimeType The MIME type of the attachment (e.g., "image/png").
     * @param data A pointer to the binary data.
     * @param size The size of the data in bytes.
     * @note The data buffer is copied into the attachment.
     * @return An Attachment object.
     */
    static Attachment fromBinary(std::string_view name, std::string_view mimeType,
                                const void* data, size_t size);

    /**
     * @brief Creates a plain text attachment.
     * @param name The name of the attachment.
     * @param content The text content.
     * @note Content is copied into the attachment buffer.
     * @return An Attachment object.
     */
    static Attachment fromText(std::string_view name, std::string_view content);

    /**
     * @brief Creates an attachment from a file.
     * @param name The name of the attachment.
     * @param filePath The path to the file. The MIME type will be detected automatically.
     * @note File content is read eagerly and stored inside the attachment.
     * @return An Attachment object.
     */
    static Attachment fromFile(std::string_view name, std::string_view filePath);

    /**
     * @brief Attaches the built attachment to the current test or step.
     *
     * No-op if there is no running test/step or if the buffer is empty.
     */
    void attach();

private:
    Attachment() = default;

    std::string m_name;       ///< The name of the attachment.
    std::string m_type;       ///< The MIME type of the attachment.
    std::vector<char> m_data; ///< The attachment data.
};

/**
 * @brief Convenience function to attach plain text content.
 * @param name The name of the attachment.
 * @param content The text content to attach.
 */
void attachText(std::string_view name, std::string_view content);

/**
 * @brief Convenience function to attach a file.
 * @param name The name of the attachment.
 * @param filePath The path to the file to attach.
 */
void attachFile(std::string_view name, std::string_view filePath);

} // namespace allure
