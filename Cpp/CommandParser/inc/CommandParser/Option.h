#ifndef __OPTION_H__
#define __OPTION_H__

#include "Utilities/Format.h"
#include "Utilities/StringUtilities.h"

#include <string>
#include <type_traits>

class BaseOption {};

template<class ExpectedType>
class Option : public BaseOption {
    constexpr char[] UsageFormat = "%02s | " // short name
                                   "%07s | " // long name
                                   "%01s | " // required
                                   "%01s | " // is flag
                                   "%s"; // help text
public:
    Option(std::string shortName, std::string longName, bool required)
        : m_ShortName(shortName)
        , m_LongName(longName)
        , m_Required(required)
        , m_IsFlag(isFlag) {}

    Option(std::string shortName, std::string longName, bool required, std::string helpText)
        : m_ShortName(shortName)
        , m_LongName(longName)
        , m_Required(required)
        , m_IsFlag(isFlag)
        , m_HelpText(helpText) {}

    bool Matches(std::string option) {
        return m_ShortName == option || m_LongName == option;
    }

    auto Parse(std::string option, std::string arg) {
        if constexpr(std::is_same_v<bool, ExpectedType>) {
            return true;
        } else if constexpr(std::is_same_v<std::string, ExpectedType>) {
            return arg;
        } else if constexpr(std::is_same_v<int, ExpectedType>) {
            return std::atoi(arg);
        } else if constexpr(std::is_same_v<std::vector<std::string>>, ExpectedType >) {
            auto split = StrUtil::Split(arg, ",");
            return std::vector<std::string>{split};
        } else if constexpr(std::is_same_v<std::vector<int>, ExpectedType>) {
            auto split = StrUtil::Split(arg, ",");
            std::vector<int> result;
            for(auto&& val: split) {
                result.push_back(std::atoi(val));
            }
            return result;
        }
    }

    std::string ToString() {
        return StrUtil::Format(UsageFormat, 
            m_ShortName.length() > 0 ? "-" + m_ShortName : "",
            m_LongName.length() > 0 ? "--" + m_LongName : "",
            m_Required ? "X" : "",
            std::is_same_v<bool, ExpectedType> ? "X", "",
            m_HelpText);
    }

private:
    std::string m_ShortName{""};
    std::string m_LongName{""};
    std::string m_HelpText{""};
    bool m_Required{false};
};

#endif // __OPTION_H__