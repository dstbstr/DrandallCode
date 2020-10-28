#ifndef __OPTION_H__
#define __OPTION_H__

#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/StringUtilities.h"

#include <regex>
#include <string>

namespace CommandParser {
    constexpr char UsageFormat[]{"%2s | " // short name
                                 "%10s | " // long name
                                 "%1s | " // required
                                 "%1s | " // is flag
                                 "%s"}; // help text

    class BaseOption {
    public:
        inline static const std::string BLANK{"<BLANK>"};

        enum class InnerType { INT, STRING, BOOL, VEC_STRING, VEC_INT };

        BaseOption(std::string shortName, std::string longName, bool required);
        BaseOption(std::string shortName, std::string longName, bool required, std::string helpText);
        // TODO: consider changing these to static factory methods
        BaseOption(bool required);
        BaseOption(bool required, std::string helpText);

        virtual ~BaseOption() = default;

        bool Matches(std::string option) const;
        bool IsRequired() const;
        std::string GetShortName() const;
        std::string GetLongName() const;

        virtual InnerType GetInnerType() const = 0;
        virtual void Populate(std::string);
        bool IsPopulated() const;

        std::string ToString();

    protected:
        bool m_Populated{false};

    private:
        std::string m_ShortName{""};
        std::string m_LongName{""};
        std::string m_HelpText{""};
        bool m_Required{false};

        std::string CleanName(std::string input) const;
        void ValidateName() const;
    };

    class IntOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final;
        void Populate(std::string input) override;
        int GetValue() const;

    private:
        int m_Value;
    };

    class StringOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final;
        void Populate(std::string input) override;
        std::string GetValue() const;

    private:
        std::string m_Value;
    };

    class BoolOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final;
        void Populate(std::string) override;
        bool GetValue() const;
    };

    class VecStringOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final;
        void Populate(std::string input) override;
        std::vector<std::string> GetValue() const;

    private:
        std::vector<std::string> m_Value;
    };

    class VecIntOption : public BaseOption {
    public:
        using BaseOption::BaseOption;
        InnerType GetInnerType() const final;
        void Populate(std::string input) override;
        std::vector<int> GetValue() const;

    private:
        std::vector<int> m_Value;
    };

} // namespace CommandParser
#endif // __OPTION_H__