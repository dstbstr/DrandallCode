#include "Extractor/Workers/IncludeMapGenerator.h"

#include "Extractor/Data/FileData.h"

#include "Core/Utilities/PathUtils.h"
#include "Core/Utilities/ScopedTimer.h"

namespace {
    void Recurse(const std::string& fileName, const std::unordered_map<std::string, const Extractor::FileData*>& firstParty, Extractor::IncludeMap& result) {
        if(result.Dependencies.find(fileName) != result.Dependencies.end()) {
            return; // already processed
        }

        result.Dependencies.emplace(fileName, std::unordered_set<std::string>{});
        result.DependsOnMe.emplace(fileName, std::unordered_set<std::string>{});
        result.LineCounts.emplace(fileName, 0);

        if(firstParty.find(fileName) == firstParty.end()) {
            return; // Third party
        }
        const auto& file = firstParty.at(fileName);
        result.LineCounts[fileName] = file->LineCount;
        for(const auto& includePath: file->IncludeFiles) {
            auto include = PathUtils::GetFileName(includePath);
            Recurse(include, firstParty, result);

            result.Dependencies[fileName].insert(include);
            result.Dependencies[fileName].insert(result.Dependencies[include].begin(), result.Dependencies[include].end());

            for(const auto& dep: result.Dependencies[fileName]) {
                result.DependsOnMe[dep].insert(fileName);
            }
        }
    }

    void RecursiveFind(const Extractor::IncludeMap& includeMap, const std::string& fileName, const std::string& targetFile, std::vector<std::string>& result) {
        if(fileName == targetFile) {
            return;
        }
        for(const auto& dependency : includeMap.Dependencies.at(fileName)) {
            if(dependency == targetFile) {
                result.push_back(dependency);
                return;
            }
            if(includeMap.Dependencies.at(dependency).contains(targetFile)) {
                if(std::find(result.begin(), result.end(), dependency) != result.end()) {
                    continue; //already added this dependency, try a different one
                }
                result.push_back(dependency);
                RecursiveFind(includeMap, dependency, targetFile, result);
                return; //for now, just find a single circular dependency path
            }
        }

        throw std::runtime_error("No circular dependency path found");
    }
} // namespace

namespace Extractor {
    IncludeMap GenerateIncludeMap(const std::vector<FileData>& files) {
        ScopedTimer timer("IncludeMapGenerator::Generate");
        std::unordered_map<std::string, const FileData*> firstPartyIncludes;

        for(const auto& file: files) {
            firstPartyIncludes[file.FileName] = &file;
        }

        IncludeMap result{};
        for(const auto& file: files) {
            Recurse(file.FileName, firstPartyIncludes, result);
        }

        return result;
    }

    bool HasCircularDependency(const IncludeMap& includeMap, const std::string& fileName) {
        if(includeMap.Dependencies.find(fileName) == includeMap.Dependencies.end()) {
            return false;
        }

        return includeMap.Dependencies.at(fileName).contains(fileName);
    }

    std::vector<std::string> GetCircularDependencyTrail(const IncludeMap& includeMap, const std::string& fileName) {
        std::vector<std::string> result;
        result.push_back(fileName);
        for(const auto& dependency : includeMap.Dependencies.at(fileName)) {
            if(includeMap.Dependencies.at(dependency).contains(fileName)) {
                result.push_back(dependency);
                RecursiveFind(includeMap, dependency, fileName, result);
                break;
            }
        }
        return result;
    }
} // namespace Extractor