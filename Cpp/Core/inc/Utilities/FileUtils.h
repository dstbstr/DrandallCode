#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <iostream>
#include <unordered_set>

namespace FileUtils {
    bool Exists(const std::string_view filePath);
    std::unique_ptr<std::istream> OpenForRead(const std::string_view filePath);
    std::unique_ptr<std::ostream> OpenForWrite(const std::string_view filePath);

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
#endif // __FILEUTILS_H__