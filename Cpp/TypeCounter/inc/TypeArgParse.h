#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__

#include "CommandParser/Option.h"
#include "CommandParser/OptionCollection.h"

#include <string>
#include <vector>

namespace TypeCounter {
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

        bool RunFunctionReport() const {
            return m_FunctionFlag.IsPopulated();
        }

    private:
        CommandParser::BoolOption m_HelpFlag{"h", "help", false, "Prints the usage"};
        CommandParser::BoolOption m_RecurseFlag{"r", "recurse", false, "Whether or not to search the provided directory/directories recursively"};
        CommandParser::BoolOption m_FunctionFlag{"", "function", false, "Runs the function report.  If not set, defaults to type report"};
        CommandParser::StringOption m_OutFile{"f", "file", false, "File name to save the results to"};
        CommandParser::VecStringOption m_FilePathOption{false, "File paths and directories"};
        CommandParser::OptionCollection m_Options{"Produces a report about the types and their functions"};
    };
} // namespace TypeCounter

#endif // __ARGPARSE_H__