#ifndef __OPTION_H__
#define __OPTION_H__

#include "Instrumentation/Assert.h"
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

    class BaseOption {
    public:
        enum class InnerType { INT, STRING, BOOL, VEC_STRING, VEC_INT };

        BaseOption(std::string shortName, std::string longName, bool required) : m_ShortName(shortName), m_LongName(longName), m_Required(required) {}

        BaseOption(std::string shortName, std::string longName, bool required, std::string helpText)
            : m_ShortName(shortName)
            , m_LongName(longName)
            , m_Required(required)
            , m_HelpText(helpText) {}

        virtual ~BaseOption() = default;

        bool Matches(std::string option) const {
            return m_ShortName == option || m_LongName == option;
        }

        bool IsRequired() const {
            return m_Required;
        }

        std::string GetLongName() const {
            return m_LongName;
        }
        virtual InnerType GetInnerType() const = 0;
        virtual void Populate(std::string) {
            ASSERT_MSG(false, "Called Populate on BaseOption");
        }

        std::string ToString() {
            return StrUtil::Format(UsageFormat,
                                   m_ShortName.length() > 0 ? "-" + m_ShortName : "",
                                   m_LongName.length() > 0 ? "--" + m_LongName : "",
                                   m_Required ? "X" : "",
                                   GetInnerType() == InnerType::BOOL ? "X" : "",
                                   m_HelpText);
        }

    private:
        std::string m_ShortName{""};
        std::string m_LongName{""};
        std::string m_HelpText{""};
        bool m_Required{false};
    };

    class IntOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final {
            return InnerType::INT;
        }

        void Populate(std::string input) override {
            m_Value = std::atoi(input.c_str());
        }

        int GetValue() const {
            return m_Value;
        }

    private:
        int m_Value;
    };

    class StringOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final {
            return InnerType::STRING;
        }

        void Populate(std::string input) override {
            m_Value = input;
        }

        std::string GetValue() const {
            return m_Value;
        }

    private:
        std::string m_Value;
    };

    class BoolOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final {
            return InnerType::BOOL;
        }

        void Populate(std::string) override {};

        bool GetValue() const {
            return true;
        }
    };

    class VecStringOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final {
            return InnerType::VEC_STRING;
        }

        void Populate(std::string input) override {
            auto split = StrUtil::Split(input, ",");
            for(auto&& val: split) {
                m_Value.push_back(std::string(val));
            }
        }

        std::vector<std::string> GetValue() const {
            return m_Value;
        }
    private:
        std::vector<std::string> m_Value;
    };

    class VecIntOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final {
            return InnerType::VEC_INT;
        }

        void Populate(std::string input) override {
            auto split = StrUtil::Split(input, ",");
            for(auto&& val: split) {
                m_Value.push_back(std::atoi(std::string(val).c_str()));
            }
        }

        std::vector<int> GetValue() const {
            return m_Value;
        }
    private:
        std::vector<int> m_Value;
    };

} // namespace CommandParser
#endif // __OPTION_H__