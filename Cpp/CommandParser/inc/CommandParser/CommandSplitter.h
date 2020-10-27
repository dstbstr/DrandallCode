#ifndef __COMMANDSPLITTER_H__
#define __COMMANDSPLITTER_H__
#include "OptionValuePair.h"

#include <string>
#include <vector>

namespace CommandParser {
    class CommandSplitter {
    public:
        CommandSplitter(int argc, char* argv[]);

        bool HasMoreArguments() const;

        OptionValuePair GetNext();

    private:
        std::vector<std::string> m_Args;
        std::vector<OptionValuePair> m_ParsedArgs;
        bool m_HasParsed{false};
    };
} // namespace CommandParser
#endif // __COMMANDSPLITTER_H__