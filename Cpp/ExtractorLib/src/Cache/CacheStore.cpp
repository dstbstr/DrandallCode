#include "Extractor/Cache/CacheStore.h"

#include "Extractor/Data/DefineData.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/Data/IncludeMap.h"
#include "Utilities/FileUtils.h"
#include "Utilities/PathUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtils.h"
#include "Utilities/TimeUtils.h"

#include <algorithm>
#include <fstream>

constexpr char TimeFormat[]{"%Y/%m/%d %H_%M_%S"};
constexpr char CategorySeparator[]{"~:~"};
constexpr char ValueSeparator[]{"~,~"};
constexpr char KvpSeparator[]{"~=~"};

namespace Extractor {
    CacheStore::CacheStore(std::string cacheFileName) : m_FilePath(cacheFileName) {}
    CacheStore::CacheStore(std::shared_ptr<std::istream> inStream, std::shared_ptr<std::ostream> outStream) : m_InStream(inStream), m_OutStream(outStream) {}

    void CacheStore::WriteCache(const std::vector<FileData>& fileData, const DefineData& defines, const IncludeMap& includes) {
        ScopedTimer writeTimer("Write Cache");
        if(m_OutStream == nullptr) {
            std::filesystem::remove(m_FilePath); // clear out old results
            m_OutStream = FileUtils::OpenForWrite(m_FilePath);
        }

        auto& stream = *m_OutStream;
        stream << "Time" << CategorySeparator << TimeUtils::DateTimeLocalToString(std::filesystem::file_time_type::clock::now(), TimeFormat) << "\n";

        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> definesBySource{};
        for(const auto& [define, source]: defines.DefineSource) {
            if(definesBySource.find(source) == definesBySource.end()) {
                definesBySource.emplace(source, std::unordered_map<std::string, std::string>{});
            }
            definesBySource[source].emplace(define, defines.Defines.at(define));
        }

        const auto combiner = [](const std::pair<std::string, std::string>& kvp) {
            return kvp.first + KvpSeparator + kvp.second;
        };

        if(definesBySource.find(UserProvidedDefineName) != definesBySource.end()) {
            std::vector<std::string> defineStrings{definesBySource.at(UserProvidedDefineName).size()};
            std::transform(definesBySource.at(UserProvidedDefineName).begin(), definesBySource.at(UserProvidedDefineName).end(), defineStrings.begin(), combiner);
            stream << "Define" << CategorySeparator << UserProvidedDefineName << CategorySeparator << StrUtil::JoinVec(ValueSeparator, defineStrings) << "\n";
        }

        for(const auto& file: fileData) {
            if(definesBySource.find(file.FilePath) == definesBySource.end()) {
                stream << "Define" << CategorySeparator << file.FilePath << CategorySeparator << "\n";
            } else {
                std::vector<std::string> defineStrings{definesBySource.at(file.FilePath).size()};
                std::transform(definesBySource.at(file.FilePath).begin(), definesBySource.at(file.FilePath).end(), defineStrings.begin(), combiner);
                stream << "Define" << CategorySeparator << file.FilePath << CategorySeparator << StrUtil::JoinVec(ValueSeparator, defineStrings) << "\n";
            }
            stream << "Dependency" << CategorySeparator << file.FilePath << CategorySeparator << StrUtil::JoinVec(ValueSeparator, includes.Dependencies.at(file.FileName)) << "\n";
            stream << "DependsOnMe" << CategorySeparator << file.FilePath << CategorySeparator << StrUtil::JoinVec(ValueSeparator, includes.DependsOnMe.at(file.FileName)) << "\n";
        }

        m_OutStream.reset();
    }

    // TODO: Refactor all of this logic into serializers
    CacheResult CacheStore::ReadCache() {
        ScopedTimer writeTimer("Read Cache");

        auto result = CacheResult{};
        if(m_InStream == nullptr) {
            if(!std::filesystem::exists(m_FilePath)) {
                return result;
            }
            m_InStream = FileUtils::OpenForRead(m_FilePath);
        }

        auto& stream = *m_InStream;
        auto line = std::string{};
        while(std::getline(stream, line)) {
            auto split = StrUtil::Split(line, CategorySeparator);
            if(split.size() <= 1) {
                continue;
            }
            if(split[0] == "Time") {
                auto cacheTime = TimeUtils::StringToTimePoint(std::string(split[1]), TimeFormat);
                result.CacheTime = TimeUtils::SystemTimeToFileTime(cacheTime);
            } else if(split[0] == "Define") {
                auto& defines = result.DefineCache.FileDefines;
                auto source = std::string(split[1]);
                if(defines.find(source) == defines.end()) {
                    defines.emplace(source, std::unordered_map<std::string, std::string>{});
                }

                if(split.size() < 3) {
                    continue;
                }

                auto kvpSplit = StrUtil::Split(split[2], ValueSeparator);
                for(const auto& kvp: kvpSplit) {
                    auto defineAndValue = StrUtil::Split(kvp, KvpSeparator);
                    if(defineAndValue.size() == 1) {
                        defines.at(source).emplace(defineAndValue[0], "");
                    } else {
                        defines.at(source).emplace(defineAndValue[0], defineAndValue[1]);
                    }
                }
            } else if(split[0] == "Dependency") {
                auto& includes = result.IncludeCache.FileIncludes;
                auto source = std::string(split[1]);

                if(includes.find(source) == includes.end()) {
                    includes.emplace(source, std::unordered_set<std::string>{});
                }
                if(split.size() == 3) {
                    auto dependencies = StrUtil::Split(split[2], ValueSeparator);
                    includes.at(source).insert(dependencies.begin(), dependencies.end());
                }
            } else if(split[0] == "DependsOnMe") {
                auto& includeBy = result.IncludeCache.FileIncludedBy;
                auto source = std::string(split[1]);
                if(includeBy.find(source) == includeBy.end()) {
                    includeBy.emplace(source, std::unordered_set<std::string>{});
                }

                if(split.size() == 3) {
                    auto dependencies = StrUtil::Split(split[2], ValueSeparator);
                    includeBy.at(source).insert(dependencies.begin(), dependencies.end());
                }
            }
        }

        m_InStream.reset(); // close the file
        return result;
    }
} // namespace Extractor