#include "Extractor/Workers/IncludeMapGenerator.h"

#include "Utilities/PathUtils.h"
#include "Utilities/ScopedTimer.h"

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
} // namespace Extractor