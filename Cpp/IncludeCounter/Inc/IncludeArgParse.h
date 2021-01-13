#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__
#include "CommandParser/Option.h"
#include "CommandParser/OptionCollection.h"

namespace IncludeCounter {
    class ArgParse {
    public:
        ArgParse(int argc, char* argv[]);
        ArgParse(const ArgParse&) = delete;
        const ArgParse& operator=(const ArgParse&) = delete;

        bool ShouldParse() const;

        std::vector<std::string> GetFileNames() const;
        std::vector<std::string> GetDefines() const;

        std::string GetTargetFile() const {
            return m_OutFile.IsPopulated() ? m_OutFile.GetValue() : "";
        }

    private:
        CommandParser::BoolOption m_RecurseFlag{"r", "recurse", false, "Whether or not to search the provided directory/directories recursively"};
        CommandParser::BoolOption m_HelpFlag{"h", "help", false, "Prints the usage"};
        CommandParser::StringOption m_OutFile{"f", "file", false, "File name to save the results to"};
        CommandParser::VecStringOption m_Defines{"d", "define", false, "Preprocess defines to set"};
        CommandParser::StringOption m_DefineFile{"", "defineFile", false, "File with space separated defines"};
        CommandParser::VecStringOption m_FilePathOption{false, "File paths and directories"};
        CommandParser::OptionCollection m_Options{"Produces a report about the number of includes"};
    };
} // namespace IncludeCounter
#endif // __ARGPARSE_H__