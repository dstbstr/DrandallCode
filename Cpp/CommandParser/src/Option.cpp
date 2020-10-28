#include "CommandParser/Option.h"
namespace CommandParser {
    BaseOption::BaseOption(std::string shortName, std::string longName, bool required) : m_Required(required) {
        m_ShortName = CleanName(shortName);
        m_LongName = CleanName(longName);
        ValidateName();
    }

    BaseOption::BaseOption(std::string shortName, std::string longName, bool required, std::string helpText)
        : m_Required(required)
        , m_HelpText(helpText) {
        m_ShortName = CleanName(shortName);
        m_LongName = CleanName(longName);
        ValidateName();
    }

    BaseOption::BaseOption(bool required) : m_Required(required) {
        m_ShortName = "";
        m_LongName = BaseOption::BLANK;
    }

    BaseOption::BaseOption(bool required, std::string helpText) : m_Required(required), m_HelpText(helpText) {
        m_ShortName = "";
        m_LongName = BaseOption::BLANK;
    }

    bool BaseOption::Matches(std::string option) const {
        if(option == "") {
            return m_ShortName == BLANK || m_LongName == BLANK;
        }
        return option == m_ShortName || option == m_LongName;
    }

    bool BaseOption::IsRequired() const {
        return m_Required;
    }

    std::string BaseOption::GetShortName() const {
        return m_ShortName;
    }

    std::string BaseOption::GetLongName() const {
        return m_LongName;
    }
    void BaseOption::Populate(std::string) {
        ASSERT_MSG(false, "Called Populate on BaseOption");
    }

    bool BaseOption::IsPopulated() const {
        return m_Populated;
    }

    std::string BaseOption::ToString() {
        return StrUtil::Format(UsageFormat,
                               m_ShortName.length() > 0 ? "-" + m_ShortName : "",
                               m_LongName.length() > 0 ? "--" + m_LongName : "",
                               m_Required ? "X" : "",
                               GetInnerType() == InnerType::BOOL ? "X" : "",
                               m_HelpText);
    }

    std::string BaseOption::CleanName(std::string input) const {
        if(input == "" || input == BaseOption::BLANK) {
            return input; // blank is a valid if command doesn't have a short/long version
        }
        static std::regex cleanRegex(R"(\w+)");
        std::smatch match;
        std::regex_match(input, match, cleanRegex);
        if(!std::regex_search(input, match, cleanRegex)) {
            throw std::exception(("Invalid option name " + input).c_str());
        }
        return match[0];
    }

    void BaseOption::ValidateName() const {
        if(m_ShortName == "" && m_LongName == "") {
            throw std::exception("Must provide at least identifier.  To use blank, use BaseOption::BLANK");
        }
        if(m_ShortName.length() > 1 && m_ShortName != BaseOption::BLANK) {
            throw std::exception("Short name must be a single character");
        }
    }

    BaseOption::InnerType IntOption::GetInnerType() const {
        return InnerType::INT;
    }

    void IntOption::Populate(std::string input) {
        m_Value = std::atoi(input.c_str());
        m_Populated = true;
    }

    int IntOption::GetValue() const {
        return m_Value;
    }

    BaseOption::InnerType StringOption::GetInnerType() const {
        return InnerType::STRING;
    }

    void StringOption::Populate(std::string input) {
        m_Value = input;
        m_Populated = true;
    }

    std::string StringOption::GetValue() const {
        return m_Value;
    }

    BaseOption::InnerType BoolOption::GetInnerType() const {
        return InnerType::BOOL;
    }

    void BoolOption::Populate(std::string) {
        m_Populated = true;
    };

    bool BoolOption::GetValue() const {
        return true;
    }
    BaseOption::InnerType VecStringOption::GetInnerType() const {
        return InnerType::VEC_STRING;
    }

    void VecStringOption::Populate(std::string input) {
        auto split = StrUtil::Split(input, ",");
        for(auto&& val: split) {
            m_Value.push_back(std::string(val));
        }
        m_Populated = true;
    }

    std::vector<std::string> VecStringOption::GetValue() const {
        return m_Value;
    }

    BaseOption::InnerType VecIntOption::GetInnerType() const {
        return InnerType::VEC_INT;
    }

    void VecIntOption::Populate(std::string input) {
        auto split = StrUtil::Split(input, ",");
        for(auto&& val: split) {
            m_Value.push_back(std::atoi(std::string(val).c_str()));
        }
        m_Populated = true;
    }

    std::vector<int> VecIntOption::GetValue() const {
        return m_Value;
    }
} // namespace CommandParser