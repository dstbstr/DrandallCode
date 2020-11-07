#include "ArgParse.h"

#include "CommandParser/CommandSplitter.h"
#include "IncludeCounter/FileNameCollector.h"
#include "IncludeCounter/ResultGenerator.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtilities.h"

#include <iostream>

namespace IncludeCounter {
    ArgParse::ArgParse(int argc, char* argv[]) {
        auto sortingOptions = ResultGenerator::GetSortOrderStrings();
        std::stringstream sortHelpLine;
        sortHelpLine << "Order to sort results by.  " << StrUtil::JoinVec(" ", sortingOptions);

        m_SortOption.UpdateHelpText(sortHelpLine.str());

        m_Options.Add(m_RecurseFlag).Add(m_HelpFlag).Add(m_FilePathOption).Add(m_OutFile).Add(m_SortOption).Add(m_Descending);
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

        FileNameCollector fnc{targets, std::vector<std::string>{".h", ".cpp", ".hpp"}, m_RecurseFlag.IsPopulated()};
        return fnc.GetAllFullyQualifiedPaths();
    }
} // namespace IncludeCounter