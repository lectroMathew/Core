#include <core/FileSystem.h>
#include <fstream>

namespace core {

int64_t FileSystem::GetFileSize(const string& path) {
    std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
    return file.tellg();
}
std::vector<char> FileSystem::ReadFile(const string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        Logger::Log(IMPORT, ERR_HERE) << "Failed to open file at " << filename;
        throw std::runtime_error("Failed to open file at " + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    Logger::Log(MEMORY, INFO) << "|ALLOC| Buffer size : " << fileSize << " bytes";
    file.seekg(0);
    file.read(buffer.data(), static_cast<int32_t>(fileSize));
    file.close();

    Logger::Log(IMPORT, INFO) << "Read " << buffer.size() << " bytes from " << filename;

    return buffer;
}

}
