#include "IncludeArgParse.h"

#include "CommandParser/CommandSplitter.h"
#include "Extractor/Workers/FileNameCollector.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtils.h"

#include <iostream>

namespace IncludeCounter {
    ArgParse::ArgParse(int argc, char* argv[]) {
        m_Options.Add(m_RecurseFlag).Add(m_HelpFlag).Add(m_FilePathOption).Add(m_OutFile);
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

        Extractor::FileNameCollector fnc{targets, std::vector<std::string>{".h", ".cpp", ".hpp"}, m_RecurseFlag.IsPopulated()};
        return fnc.GetAllFullyQualifiedPaths();
    }
} // namespace IncludeCounter