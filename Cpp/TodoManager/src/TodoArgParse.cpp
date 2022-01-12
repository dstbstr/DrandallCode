#include "TodoArgParse.h"

#include "CommandParser/CommandSplitter.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/FileUtils.h"
#include <iostream>

namespace TodoManager {
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

        FileUtils::FileNameCollector fnc{targets, std::vector<std::string>{".h", ".cpp", ".hpp", ".inl", ".cs", ".c"}, m_RecurseFlag.IsPopulated()};
        return fnc.GetAllFullyQualifiedPaths();
    }

} // namespace TypeCounter