#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__
#include "CommandParser/CommandSplitter.h"
#include "CommandParser/Option.h"
#include "CommandParser/OptionCollection.h"
#include "FileNameCollector.h"
#include "Utilities/ScopedTimer.h"

#include <filesystem>
#include <iostream>

class ArgParse {
public:
    ArgParse(int argc, char* argv[]) {
        m_Options.Add(m_RecurseFlag).Add(m_HelpFlag).Add(m_FilePathOption).Add(m_OutFile).Add(m_Descending);
        CommandParser::CommandSplitter splitter(argc, argv);
        m_Options.Apply(splitter.GetAll());
    }

    bool ShouldParse() {
        if(m_HelpFlag.IsPopulated()) {
            m_Options.PrintUsage(std::cout);
            return false;
        }
        return true;
    }

    std::vector<std::string> GetFileNames() {
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

    bool IsDescending() {
        return m_Descending.IsPopulated();
    }

    std::string GetTargetFile() {
        return m_OutFile.IsPopulated() ? m_OutFile.GetValue() : "";
    }

private:
    CommandParser::BoolOption m_RecurseFlag{"r", "recurse", false, "Whether or not to search the provided directory/directories recursively"};
    CommandParser::BoolOption m_HelpFlag{"h", "help", false, "Prints the usage"};
    CommandParser::BoolOption m_Descending{"", "desc", false, "Sort the results in descending order.  Defaults to ascending"};
    CommandParser::StringOption m_OutFile{"f", "file", false, "File name to save the results to"};
    CommandParser::VecStringOption m_FilePathOption{false, "File paths and directories"};
    CommandParser::OptionCollection m_Options{"Produces a report about the number of includes"};
};

#endif // __ARGPARSE_H__