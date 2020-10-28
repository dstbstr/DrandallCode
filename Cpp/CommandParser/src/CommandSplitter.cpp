#include "CommandParser/CommandSplitter.h"

#include "Instrumentation/Assert.h"
#include "Utilities/StringUtilities.h"

#include <algorithm>


namespace CommandParser {
    static std::string currentArg;
    // private forward declarations
    static void HandleOption(std::string, std::vector<OptionValuePair>&);
    static void HandleValue(std::string, std::vector<OptionValuePair>&);
    static void SplitShortOption(std::string, std::vector<OptionValuePair>&);
    static void ParseOptions(std::vector<std::string>, std::vector<OptionValuePair>&);
    static bool IsOption(std::string);
    static bool IsLongOption(std::string);

    CommandSplitter::CommandSplitter(int argc, char* argv[]) {
        ASSERT_MSG(argc > 0, "Received 0 args");
        // First arg is the program name
        for(int i = 1; i < argc; i++) {
            m_Args.push_back(argv[i]);
        }
    }

    bool CommandSplitter::HasMoreArguments() const {
        return !(m_ParsedArgs.empty() && m_Args.empty());
    }

    OptionValuePair CommandSplitter::GetNext() {
        if(!m_HasParsed) {
            ParseOptions(m_Args, m_ParsedArgs);
            std::reverse(m_ParsedArgs.begin(),
                         m_ParsedArgs.end()); // reverse to allow the rest of the application to receive args in the order provided
            m_Args.clear();
            m_HasParsed = true;
        }
        ASSERT_MSG(m_ParsedArgs.size() > 0, "GetNext called without HasMoreArguments call (or after it returned false)");
        auto result = m_ParsedArgs.back();
        m_ParsedArgs.pop_back();
        return result;
    }

    std::vector<OptionValuePair> CommandSplitter::GetAll() {
        if(!m_HasParsed) {
            ParseOptions(m_Args, m_ParsedArgs);
            m_Args.clear();
            m_HasParsed = true;
        }
        return m_ParsedArgs;
    }
    // Private helper functions

    static void FlushCurrentArg(std::vector<OptionValuePair>& result) {
        result.push_back(OptionValuePair(currentArg));
        currentArg = "";
    }
    static void FlushCurrentArg(std::vector<OptionValuePair>& result, std::string value) {
        result.push_back(OptionValuePair(currentArg, value));
        currentArg = "";
    }

    static void ParseOptions(std::vector<std::string> args, std::vector<OptionValuePair>& result) {
        for(auto&& arg: args) {
            if(IsOption(arg)) {
                HandleOption(arg, result);
            } else {
                HandleValue(arg, result);
            }
        }

        if(currentArg.length() > 0) {
            FlushCurrentArg(result);
        }
    }

    static void HandleOption(std::string arg, std::vector<OptionValuePair>& result) {
        if(currentArg.length() > 0) {
            FlushCurrentArg(result);
        }
        if(IsLongOption(arg)) {
            if(arg[0] == '/') {
                currentArg = arg.substr(1);
            } else {
                currentArg = arg.substr(2);
            }
        } else {
            if(arg.length() > 2) {
                SplitShortOption(arg, result);
            } else {
                currentArg = arg.substr(1);
            }
        }
    }

    static void HandleValue(std::string arg, std::vector<OptionValuePair>& result) {
        FlushCurrentArg(result, arg);
    }

    static bool IsOption(std::string arg) {
        return arg[0] == '-' || arg[0] == '/';
    }

    static bool IsLongOption(std::string arg) {
        return arg[0] == '/' || arg[1] == '-';
    }

    static void SplitShortOption(std::string arg, std::vector<OptionValuePair>& result) {
        for(int i = 1; i < arg.length(); i++) {
            result.push_back(OptionValuePair(std::string{arg[i]}));
        }
    }

} // namespace CommandParser