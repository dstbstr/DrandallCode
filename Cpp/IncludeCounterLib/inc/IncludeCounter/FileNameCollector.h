#ifndef __FILENAMECOLLECTOR_H__
#define __FILENAMECOLLECTOR_H__

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>


namespace IncludeCounter {
    class FileNameCollector {
    public:
        FileNameCollector(std::vector<std::string> inputs, std::vector<std::string> extensions, bool recurse);
        FileNameCollector(const FileNameCollector&) = delete;
        FileNameCollector& operator=(FileNameCollector) = delete;

        std::vector<std::string> GetAllFullyQualifiedPaths();

    private:
        std::unordered_set<std::string> m_Inputs;
        std::unordered_set<std::string> m_Extensions;
        bool m_Recurse;
    };
} // namespace IncludeCounterLib
#endif // __FILENAMECOLLECTOR_H__