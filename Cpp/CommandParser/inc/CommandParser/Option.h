#ifndef __OPTION_H__
#define __OPTION_H__

#include "Utilities/Format.h"
#include "Utilities/StringUtilities.h"

#include <string>
#include <type_traits>

template<typename T>
struct is_vector : public std::false_type {};

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : public std::true_type {};

namespace CommandParser {
    constexpr char UsageFormat[]{"%2s | " // short name
                                 "%7s | " // long name
                                 "%1s | " // required
                                 "%1s | " // is flag
                                 "%s"}; // help text

    class BaseOption {};

    template<class T>
    class Option : public BaseOption {
    public:
        Option(std::string shortName, std::string longName, bool required) : m_ShortName(shortName), m_LongName(longName), m_Required(required) {}

        Option(std::string shortName, std::string longName, bool required, std::string helpText)
            : m_ShortName(shortName)
            , m_LongName(longName)
            , m_Required(required)
            , m_HelpText(helpText) {}

        bool Matches(std::string option) {
            return m_ShortName == option || m_LongName == option;
        }

        T Parse(std::string arg) {
            if constexpr(std::is_same_v<bool, T>) {
                return true;
            } else if constexpr(std::is_same_v<std::string, T>) {
                return arg;
            } else if constexpr(std::is_same_v<int, T>) {
                return std::atoi(arg.c_str());
            } else if constexpr(std::is_same_v<std::vector<std::string>, T>) {
                auto split = StrUtil::Split(arg, ",");
                std::vector<std::string> result;
                for(auto&& val: split) {
                    result.push_back(std::string(val));
                }
                return result;
            } else if constexpr(std::is_same_v<std::vector<int, std::allocator<int>>, T>) {
                auto split = StrUtil::Split(arg, ",");
                std::vector<int> result;
                for(auto&& val: split) {
                    result.push_back(std::atoi(std::string(val).c_str()));
                }
                return result;
            }
        }

        std::string ToString() {
            return StrUtil::Format(UsageFormat,
                                   m_ShortName.length() > 0 ? "-" + m_ShortName : "",
                                   m_LongName.length() > 0 ? "--" + m_LongName : "",
                                   m_Required ? "X" : "",
                                   std::is_same_v<bool, T> ? "X" : "",
                                   m_HelpText);
        }

    private:
        std::string m_ShortName{""};
        std::string m_LongName{""};
        std::string m_HelpText{""};
        bool m_Required{false};
    };
} // namespace CommandParser
#endif // __OPTION_H__