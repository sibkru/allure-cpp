#include "Attachment.h"
#include "Core.h"
#include "../Model/Attachment.h"
#include "../Services/System/IUUIDGeneratorService.h"

#include <fstream>

namespace allure {

Attachment Attachment::fromBinary(std::string_view name, std::string_view mimeType,
                                  const void* data, size_t size) {
    Attachment att;
    att.m_name = name;
    att.m_type = mimeType;

    if (data && size > 0) {
        att.m_data.resize(size);
        std::memcpy(att.m_data.data(), data, size);
    }

    return att;
}

Attachment Attachment::fromText(std::string_view name, std::string_view content) {
    return fromBinary(name, "text/plain", content.data(), content.size());
}

Attachment Attachment::fromFile(std::string_view name, std::string_view filePath) {
    Attachment att;
    att.m_name = name;

    // Read file content
    std::ifstream file(std::string(filePath), std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        att.m_data.resize(size);
        if (file.read(att.m_data.data(), size)) {
            // Determine MIME type from file extension
            std::string path(filePath);
            size_t dotPos = path.find_last_of('.');
            if (dotPos != std::string::npos) {
                std::string ext = path.substr(dotPos);
                if (ext == ".txt") att.m_type = "text/plain";
                else if (ext == ".log") att.m_type = "text/plain";
                else if (ext == ".png") att.m_type = "image/png";
                else if (ext == ".jpg" || ext == ".jpeg") att.m_type = "image/jpeg";
                else if (ext == ".json") att.m_type = "application/json";
                else if (ext == ".xml") att.m_type = "application/xml";
                else att.m_type = "application/octet-stream";
            } else {
                att.m_type = "application/octet-stream";
            }
        }
    }

    return att;
}

void Attachment::attach() {
    auto* testCase = detail::getTestProgram().getRunningTestCase();
    if (!testCase || m_data.empty()) {
        return;
    }

    // Generate unique filename for attachment
    auto factory = detail::getServicesFactory();
    auto uuidGenerator = factory->buildUUIDGeneratorService();
    auto uuid = uuidGenerator->generateUUID();

    // Determine extension from MIME type
    std::string extension;
    if (m_type.find("text/") == 0) extension = ".txt";
    else if (m_type.find("image/png") == 0) extension = ".png";
    else if (m_type.find("image/jpeg") == 0) extension = ".jpg";
    else if (m_type.find("application/json") == 0) extension = ".json";
    else if (m_type.find("application/xml") == 0) extension = ".xml";
    else extension = ".dat";

    std::string filename = uuid + "-attachment" + extension;

    // Write attachment file to output folder
    std::string outputFolder = detail::getTestProgram().getOutputFolder();
    std::string filepath = outputFolder + "/" + filename;

    std::ofstream outFile(filepath, std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(m_data.data(), m_data.size());
        outFile.close();

        // Add attachment reference to test case
        model::Attachment attachment;
        attachment.setName(m_name);
        attachment.setSource(filename);
        attachment.setType(m_type);
        testCase->addAttachment(attachment);
    }
}

// Convenience free functions
void attachText(std::string_view name, std::string_view content) {
    Attachment::fromText(name, content).attach();
}

void attachFile(std::string_view name, std::string_view filePath) {
    Attachment::fromFile(name, filePath).attach();
}

} // namespace allure
