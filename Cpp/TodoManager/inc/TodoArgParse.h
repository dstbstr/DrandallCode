#pragma once

#include "CommandParser/Option.h"
#include "CommandParser/OptionCollection.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace TodoManager {
    class ArgParse {
    public:
        ArgParse(int argc, char* argv[]);
        ArgParse(const ArgParse&) = delete;
        const ArgParse& operator=(const ArgParse&) = delete;

        bool ShouldParse() const;

        std::vector<std::string> GetFileNames() const;

        std::string GetTargetFile() const {
            return m_OutFile.IsPopulated() ? m_OutFile.GetValue() : "";
        }

    private:
        CommandParser::BoolOption m_HelpFlag{"h", "help", false, "Prints the usage"};
        CommandParser::BoolOption m_RecurseFlag{"r", "recurse", false, "Whether or not to search the provided directory(ies) recursively"};
        CommandParser::StringOption m_OutFile{"f", "file", false, "Output File name prefix"};
        CommandParser::VecStringOption m_FilePathOption{false, "File paths and directories"};
        CommandParser::OptionCollection m_Options{"Finds TODOs in code"};
    };
} // namespace TodoManager