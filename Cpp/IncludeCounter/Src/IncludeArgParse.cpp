#include "IncludeArgParse.h"

#include "CommandParser/CommandSplitter.h"
#include "Extractor/Workers/DefineFileParser.h"
#include "Utilities/FileUtils.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtils.h"

#include <iostream>
#include <unordered_set>

namespace IncludeCounter {
    ArgParse::ArgParse(int argc, char* argv[]) {
        m_Options.Add(m_RecurseFlag).Add(m_HelpFlag).Add(m_FilePathOption).Add(m_OutFile).Add(m_Defines).Add(m_DefineFile);
        CommandParser::CommandSplitter splitter(argc, argv);
        m_Options.Apply(splitter.GetAll());
    }

    bool ArgParse::ShouldParse() const {
        if(m_HelpFlag.IsPopulated()) {
            m_Options.PrintUsage(std::cout);
            return false;
        }
        return true;
    }

    std::vector<std::string> ArgParse::GetFileNames() const {
        ScopedTimer timer("ArgParse::GetFileNames");
        std::vector<std::string> targets;
        if(m_FilePathOption.IsPopulated()) {
            targets = m_FilePathOption.GetValue();
        } else {
            targets.push_back(std::filesystem::current_path().string());
        }

        FileUtils::FileNameCollector fnc{targets, std::vector<std::string>{".h", ".cpp", ".hpp"}, m_RecurseFlag.IsPopulated()};
        return fnc.GetAllFullyQualifiedPaths();
    }

    std::unordered_map<std::string, std::string> ArgParse::GetDefines() const {
        std::unordered_map<std::string, std::string> defines{};
        if(m_Defines.IsPopulated()) {
            auto userDefines = m_Defines.GetValue();
            for(const auto& define: userDefines) {
                auto split = StrUtil::Split(define, "=");
                if(split.size() > 1) {
                    defines[std::string(split[0])] = split[1];
                } else {
                    defines[define] = "";
                }
            }
        }
        if(m_DefineFile.IsPopulated()) {
            Require::True(FileUtils::Exists(m_DefineFile.GetValue()), StrUtil::Format("Could not find file %s", m_DefineFile.GetValue()));
            auto stream = FileUtils::OpenForRead(m_DefineFile.GetValue());
            auto fromFile = Extractor::DefineFileParser::Parse(*stream);
            defines.insert(fromFile.begin(), fromFile.end());
        }

        return defines;
    }
} // namespace IncludeCounter