#pragma once

#include <iostream>
#include <unordered_set>

namespace std::filesystem {
    class path;
}
namespace FileUtils {
    bool Exists(const std::string_view filePath);
    std::unique_ptr<std::istream> OpenForRead(const std::string_view filePath);
    std::unique_ptr<std::ostream> OpenForWrite(const std::string_view filePath);

    std::vector<std::string> GetFullyQualifiedFilesWithExtension(const std::filesystem::path& directory, const std::string& extension);
    std::vector<std::string> GetFullyQualifiedFilesWithExtension(const std::string& directory, const std::string& extension);

    void DeleteFiles(const std::vector<std::string>& files);
    void DeleteFiles(const std::string& directory);
    void DeleteFiles(const std::string& directory, const std::string& extension);

    class FileNameCollector {
    public:
        FileNameCollector(const std::vector<std::string>& inputs, const std::vector<std::string>& extensions, bool recurse);
        FileNameCollector(const FileNameCollector&) = delete;
        FileNameCollector& operator=(FileNameCollector) = delete;

        std::vector<std::string> GetAllFullyQualifiedPaths();

    private:
        std::unordered_set<std::string> m_Inputs;
        std::unordered_set<std::string> m_Extensions;
        bool m_Recurse;
    };

} // namespace FileUtils