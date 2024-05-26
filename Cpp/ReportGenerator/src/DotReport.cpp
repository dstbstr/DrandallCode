#include "Report/DotReport.h"

#include "Extractor/Data/IncludeMap.h"
#include "Extractor/Workers/IncludeMapGenerator.h"

#include "Core/Utilities/Format.h"
#include "Core/Utilities/StringUtils.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <unordered_set>

namespace {
    std::unordered_set<std::string> seen{};

    bool IsSystemDependency(const std::string& fileName) {
        return !(StrUtil::EndsWith(fileName, ".h") || StrUtil::EndsWith(fileName, ".cpp"));
    }

    void AppendDotData(const Extractor::IncludeMap& includeMap, const std::string& currentFile, std::ostream& targetStream) {
        if(seen.contains(currentFile)) {
            return;
        }
        seen.emplace(currentFile);

        auto nonSystemDependencies = std::unordered_set<std::string>{};
        auto quotedDependencies = std::unordered_set<std::string>{};

        for(const auto& dep: includeMap.Dependencies.at(currentFile)) {
            if(!IsSystemDependency(dep)) {
                nonSystemDependencies.insert(dep);
                quotedDependencies.insert(StrUtil::Format("\"%s\"", dep));
            }
        }

        auto dependencies = StrUtil::JoinVec(';', quotedDependencies);
        targetStream << "\"" << currentFile << "\"->{" << dependencies << "};";

        for(const auto& dependency: nonSystemDependencies) {
            AppendDotData(includeMap, dependency, targetStream);
        }
    }

} // namespace
namespace Report {
    void DotReport::WriteReport(const Extractor::IncludeMap& includeMap, std::string directory) const {
        auto parentDir = std::filesystem::current_path() / directory;
        std::filesystem::create_directory(parentDir);

        for(const auto& [fileName, dependencies]: includeMap.Dependencies) {
            if(IsSystemDependency(fileName)) {
                continue;
            }
            if(!Extractor::HasCircularDependency(includeMap, fileName)) {
                continue;
            }
            auto filePath = parentDir / fileName;
            filePath += ".dot";
            std::filesystem::remove(filePath); // clear out old results
            auto stream = std::ofstream(filePath);

            auto trail = Extractor::GetCircularDependencyTrail(includeMap, fileName);
            std::transform(trail.begin(), trail.end(), trail.begin(), [](const std::string& node) {
                return "\"" + node + "\"";
            });

            stream << "digraph {";          
            stream << StrUtil::JoinVec("->", trail);
            stream << "}";
        }
    }

    void DotReport::WriteReportToStream(const Extractor::IncludeMap& includeMap, std::ostream& targetStream) const {
        for(const auto& [fileName, dependencies]: includeMap.Dependencies) {
            if(IsSystemDependency(fileName)) {
                continue;
            }
            if(!Extractor::HasCircularDependency(includeMap, fileName)) {
                continue;
            }
            auto trail = Extractor::GetCircularDependencyTrail(includeMap, fileName);
            std::transform(trail.begin(), trail.end(), trail.begin(), [](const std::string& node) {
                return "\"" + node + "\"";
            });

            targetStream << "digraph {";          
            targetStream << StrUtil::JoinVec("->", trail);
            targetStream << "}";
        }
    }
} // namespace Report
