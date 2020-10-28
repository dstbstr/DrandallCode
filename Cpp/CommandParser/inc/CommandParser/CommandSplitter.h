#ifndef __COMMANDSPLITTER_H__
#define __COMMANDSPLITTER_H__
#include "OptionValuePair.h"

#include <string>
#include <vector>

namespace CommandParser {
    // TODO: Command Splitter can't tell the difference between an option which expects a value
    // And simply having the last option.  So passing "-r foo" is interpreted as an argument to "-r"
    // But if "-r" takes no arguments, the "foo" should have been a free floating option.
    // Fixing this would require that the splitter knew about which options took values.
    // Work Arounds: place a - between the last flag and the free floating argument. "-r - foo"
    // Place the free floating arguments first.  "foo -r"
    class CommandSplitter {
    public:
        CommandSplitter(int argc, char* argv[]);

        bool HasMoreArguments() const;

        OptionValuePair GetNext();
        std::vector<OptionValuePair> GetAll();

    private:
        std::vector<std::string> m_Args;
        std::vector<OptionValuePair> m_ParsedArgs;
        bool m_HasParsed{false};
    };
} // namespace CommandParser
#endif // __COMMANDSPLITTER_H__