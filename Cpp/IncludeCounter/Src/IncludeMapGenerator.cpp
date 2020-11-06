#include "IncludeMapGenerator.h"

#include "Platform/Types.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtilities.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

static void RecurseIncludes(FileData& file,
                            std::unordered_map<std::string, u64>& resolved,
                            std::unordered_map<std::string, FileData*>& knownIncludes,
                            std::unordered_set<std::string>& currentPaths,
                            bool failOnCircularDependencies) {
    if(resolved.find(file.FileName) != resolved.end()) {
        file.TotalIncludeCount = resolved[file.FileName];
        return;
    }

    auto includeCount = file.IncludeFiles.size();
    for(auto&& include: file.IncludeFiles) {
        auto fileName = PathUtils::GetFileName(include);
        if(resolved.find(fileName) != resolved.end()) {
            includeCount += resolved[fileName];
            if(knownIncludes.find(fileName) != knownIncludes.end()) {
                knownIncludes[fileName]->IncludedByCount++;
            }
        } else if(knownIncludes.find(fileName) == knownIncludes.end()) {
            resolved[fileName] = 0; // Only care about files we can resolve.  System files can count for 0 for this purpose
        } else {
            bool circularDependency = currentPaths.find(file.FileName) != currentPaths.end();
            if(circularDependency) {
                if(failOnCircularDependencies) {
                    // TODO: unordered_set (naturally) doesn't preserve order.  Potentially keep the order in a vector
                    std::vector<std::string> dependencyChain{currentPaths.begin(), currentPaths.end()};
                    dependencyChain.push_back(file.FileName);
                    Require::False(circularDependency,
                                   StrUtil::Format("Circular dependency detected! %s", StrUtil::JoinVec(" -> ", dependencyChain)));
                } else {
                    break;
                }
            }
            currentPaths.insert(file.FileName);
            RecurseIncludes(*knownIncludes[fileName], resolved, knownIncludes, currentPaths, failOnCircularDependencies);
            includeCount += resolved[fileName];
            knownIncludes[fileName]->IncludedByCount++; // TODO: This only tracks direct includes, not transitive includes
            currentPaths.erase(file.FileName);
        }
    }

    file.TotalIncludeCount = includeCount;
    resolved[file.FileName] = includeCount;
}

void IncludeMapGenerator::Generate() {
    ScopedTimer timer("IncludeMapGenerator::Generate");
    std::unordered_map<std::string, u64> resolved;
    std::unordered_map<std::string, FileData*> knownIncludes;
    std::unordered_set<std::string> currentPaths;

    for(auto&& file: m_Files) {
        knownIncludes[file.FileName] = &file;
    }

    for(auto& file: m_Files) {
        RecurseIncludes(file, resolved, knownIncludes, currentPaths, m_FailOnCircularDependencies);
    }
}