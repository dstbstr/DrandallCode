#include "Extractor/Workers/FileNameCollector.h"

#include "Utilities/Require.h"

#include <iostream>

namespace {
    void RecursePath(const std::filesystem::path& path,
                     const std::unordered_set<std::string>& extensions,
                     std::vector<std::filesystem::path>& resolvedPaths) {
        using namespace std::filesystem;

        for(auto&& it: recursive_directory_iterator(path)) {
            if(it.is_regular_file() && extensions.find(it.path().extension().string()) != extensions.cend()) {
                resolvedPaths.push_back(it.path());
            }
        }
    }
} // namespace

namespace Extractor {
    FileNameCollector::FileNameCollector(std::vector<std::string> inputs, std::vector<std::string> extensions, bool recurse) : m_Recurse(recurse) {
        Require::NotEmpty(inputs, "Expected to receive at least one file path");
        Require::NotEmpty(extensions, "Expected at least one extension");

        m_Inputs.insert(inputs.begin(), inputs.end());
        m_Extensions.insert(extensions.begin(), extensions.end());
    }

    std::vector<std::string> FileNameCollector::GetAllFullyQualifiedPaths() {
        std::vector<std::filesystem::path> resolved;

        for(auto&& name: m_Inputs) {
            auto path = std::filesystem::path(name);
            if(std::filesystem::exists(path)) {
                if(std::filesystem::is_directory(path)) {
                    if(m_Recurse) {
                        RecursePath(path, m_Extensions, resolved);
                    }
                } else {
                    if(m_Extensions.find(path.extension().string()) != m_Extensions.cend()) {
                        resolved.push_back(path);
                    }
                }
            }
        }

        std::unordered_set<std::string> dedupe;
        for(auto&& path: resolved) {
            dedupe.insert(path.string());
        }

        return std::vector<std::string>{dedupe.begin(), dedupe.end()};
    }
} // namespace Extractor