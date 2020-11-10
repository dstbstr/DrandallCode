#include "IncludeCounter/IncludeMapGenerator.h"

#include "Platform/Types.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtilities.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace IncludeCounter {
    static std::vector<std::string> CircDepPath{};

    static void RecurseIncludes(FileData& file,
                                std::unordered_map<std::string, std::unordered_set<std::string>>& resolved,
                                std::unordered_map<std::string, FileData*>& knownIncludes,
                                std::unordered_set<std::string>& currentPaths,
                                bool failOnCircularDependencies) {
        if(resolved.find(file.FileName) != resolved.end()) {
            return;
        }

        std::unordered_set<std::string> allDependencies{file.IncludeFiles};

        for(auto&& include: file.IncludeFiles) {
            auto fileName = PathUtils::GetFileName(include);
            if(resolved.find(fileName) != resolved.end()) {
                auto dependencies = resolved[fileName];
                allDependencies.insert(dependencies.begin(), dependencies.end());
            } else if(knownIncludes.find(fileName) == knownIncludes.end()) {
                resolved[fileName] = std::unordered_set<std::string>{}; // Assume all system dependencies have no transitive dependencies
            } else {
                bool circularDependency = currentPaths.find(file.FileName) != currentPaths.end();
                if(circularDependency) {
                    if(failOnCircularDependencies) {
                        CircDepPath.push_back(file.FileName);
                        Require::False(circularDependency,
                                       StrUtil::Format("Circular dependency detected! %s", StrUtil::JoinVec(" -> ", CircDepPath)));
                    }
                } else {
                    currentPaths.insert(file.FileName);
                    CircDepPath.push_back(file.FileName);
                    RecurseIncludes(*knownIncludes[fileName], resolved, knownIncludes, currentPaths, failOnCircularDependencies);
                    allDependencies.insert(resolved[fileName].begin(), resolved[fileName].end());
                    currentPaths.erase(file.FileName);
                    CircDepPath.pop_back();
                }
            }
        }

        file.TotalIncludeCount = allDependencies.size();
        for(auto&& dependency: allDependencies) {
            if(knownIncludes.find(dependency) != knownIncludes.end()) {
                knownIncludes[dependency]->IncludedByCount++;
                file.TotalLineCount += knownIncludes[dependency]->LineCount;
            }
        }

        resolved[file.FileName] = allDependencies;
    }

    void IncludeMapGenerator::Generate() {
        ScopedTimer timer("IncludeMapGenerator::Generate");
        std::unordered_map<std::string, std::unordered_set<std::string>> resolved;
        std::unordered_map<std::string, FileData*> knownIncludes;
        std::unordered_set<std::string> currentPaths;

        for(auto&& file: m_Files) {
            knownIncludes[file.FileName] = &file;
        }

        for(auto& file: m_Files) {
            RecurseIncludes(file, resolved, knownIncludes, currentPaths, m_FailOnCircularDependencies);
        }
    }
} // namespace IncludeCounter