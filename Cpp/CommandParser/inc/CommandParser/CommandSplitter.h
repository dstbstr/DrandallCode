#pragma once

#include "OptionValuePair.h"

#include <string>
#include <vector>

namespace CommandParser {
    class CommandSplitter {
    public:
        CommandSplitter(int argc, char* argv[]);
        CommandSplitter(std::vector<std::string> args);

        bool HasMoreArguments() const;

        OptionValuePair GetNext();
        std::vector<OptionValuePair> GetAll();

    private:
        std::vector<std::string> m_Args;
        std::vector<OptionValuePair> m_ParsedArgs;
        bool m_HasParsed{false};
    };
} // namespace CommandParser