#include "Core/Utilities/FileUtils.h"

#include "Core/Utilities/PathUtils.h"
#include "Core/Utilities/Require.h"

#include <filesystem>
#include <fstream>
#include <regex>
#include <thread>

namespace {
    void RecursePath(const std::filesystem::path& path, const std::unordered_set<std::string>& extensions, std::vector<std::filesystem::path>& resolvedPaths) {
        using namespace std::filesystem;

        for(auto&& it: recursive_directory_iterator(path)) {
            if(it.is_regular_file() && extensions.find(it.path().extension().string()) != extensions.cend()) {
                resolvedPaths.push_back(it.path());
            }
        }
    }

    void RecurseAndFilterPath(const std::string& filter, const std::unordered_set<std::string>& extensions, std::vector<std::filesystem::path>& resolvedPaths) {
        static auto current_path = std::filesystem::current_path();
        std::string absoluteFilter = filter;
        auto parentPath = PathUtils::GetWildcardPathParent(filter);
        std::filesystem::path parent;
        if(parentPath == filter) {
            parent = current_path;
            absoluteFilter = (current_path / absoluteFilter).string();
        } else {
            parent = std::filesystem::path(PathUtils::GetWildcardPathParent(filter));
        }
        auto regex = PathUtils::PathWithWildcardToRegex(absoluteFilter);

        for(auto&& fileOrDir: std::filesystem::directory_iterator(parent)) {
            if(std::regex_match(fileOrDir.path().string(), regex)) {
                if(fileOrDir.is_directory()) {
                    RecursePath(fileOrDir.path(), extensions, resolvedPaths);
                } else if(fileOrDir.is_regular_file() && extensions.find(fileOrDir.path().extension().string()) != extensions.cend()) {
                    resolvedPaths.push_back(fileOrDir.path());
                }
            }
        }
    }
} // namespace

namespace FileUtils {
    using namespace std::filesystem;

    bool Exists(const std::string_view filePath) {
        return exists(path(filePath));
    }

    std::unique_ptr<std::istream> OpenForRead(const std::string_view filePath) {
        return std::make_unique<std::ifstream>(std::string(filePath), std::ifstream::in);
    }

    std::unique_ptr<std::ostream> OpenForWrite(const std::string_view filePath) {
        return std::make_unique<std::ofstream>(std::string(filePath), std::ifstream::out | std::ifstream::trunc);
    }

    std::vector<std::string> GetFullyQualifiedFilesWithExtension(const std::filesystem::path& path, const std::string& extension) {
        std::vector<std::string> result;
        std::string ext = StrUtil::StartsWith(extension, ".") ? extension : "." + extension;
        for(const std::filesystem::path& file : std::filesystem::directory_iterator{path}) {
            if(file.extension() == ext) {
                result.push_back(std::filesystem::absolute(file).string());
            }
        }
        return result;
    }

    std::vector<std::string> GetFullyQualifiedFilesWithExtension(const std::string& directory, const std::string& extension) {
        return GetFullyQualifiedFilesWithExtension(std::filesystem::path(directory), extension);
    }

    void DeleteFiles(const std::vector<std::string>& files) {
        using namespace std::chrono_literals;

        for(const auto& file : files) {
            int attempt = 0;
            std::error_code ec;
            while(attempt < 3) {
                if(std::filesystem::remove(file, ec)) {
                    break;
                }
                if(ec.message().find("used by another process") == ec.message().npos) {
                    std::cout << ec.message();
                    break;
                }
                std::this_thread::sleep_for(100ms);
                attempt++;
            }
        }
    }

    void DeleteFiles(const std::string& directory) {
        std::vector<std::string> files;
        for(const std::filesystem::path& file : std::filesystem::directory_iterator{directory}) {
            files.push_back(std::filesystem::absolute(file).string());
        }

        DeleteFiles(files);
    }

    void DeleteFiles(const std::string& directory, const std::string& extension) {
        DeleteFiles(GetFullyQualifiedFilesWithExtension(directory, extension));
    }

    FileNameCollector::FileNameCollector(const std::vector<std::string>& inputs, const std::vector<std::string>& extensions, bool recurse) : m_Recurse(recurse) {
        Require::NotEmpty(inputs, "Expected to receive at least one file path");
        Require::NotEmpty(extensions, "Expected at least one extension");

        m_Inputs.insert(inputs.begin(), inputs.end());
        m_Extensions.insert(extensions.begin(), extensions.end());
    }

    std::vector<std::string> FileNameCollector::GetAllFullyQualifiedPaths() {
        std::vector<std::filesystem::path> resolved;

        for(auto&& name: m_Inputs) {
            if(PathUtils::PathContainsWildcard(name)) {
                RecurseAndFilterPath(name, m_Extensions, resolved);
            } else {
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
        }

        std::unordered_set<std::string> dedupe;
        for(auto&& path: resolved) {
            dedupe.insert(path.string());
        }

        return std::vector<std::string>{dedupe.begin(), dedupe.end()};
    }
} // namespace FileUtils