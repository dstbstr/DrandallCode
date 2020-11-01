#include "CommandParser/OptionCollection.h"

#include "CommandParser/Option.h"
#include "Utilities/Format.h"

#include <unordered_set>

namespace CommandParser {
    OptionCollection::OptionCollection(std::string description) : m_Description(description){};

    OptionCollection& OptionCollection::Add(BaseOption& option) {
        if(option.IsRestOption()) {
            if(m_RestOption) {
                throw std::exception("Received multiple options which match rest args");
            } else {
                m_RestOption = &option;
            }
        } else {
            m_Options.push_back(&option);
        }
        return *this;
    }

    void OptionCollection::Validate() {
        std::unordered_set<std::string> shortNames;
        std::unordered_set<std::string> longNames;

        for(auto&& option: m_Options) {
            if(shortNames.insert(option->GetShortName()).second == false) {
                throw std::exception(StrUtil::Format("Duplicate short option detected: %s", option->GetShortName()).c_str());
            }
            if(longNames.insert(option->GetLongName()).second == false) {
                throw std::exception(StrUtil::Format("Duplicate long option detected: %s", option->GetLongName()).c_str());
            }
        }
    }
    void OptionCollection::PrintUsage(std::ostream& stream) const {
        stream << m_Description << std::endl;
        stream << "Options" << std::endl;
        stream << " S |  LongName | R | F | Help Text" << std::endl;
        stream << "----------------------------------------------------" << std::endl;

        for(auto&& option: m_Options) {
            stream << option->ToString() << std::endl;
        }
    }

    void OptionCollection::Apply(std::vector<OptionValuePair> pairs) {
        std::vector<OptionValuePair> restParams;

        for(auto&& option: m_Options) {
            bool found = false;
            std::string optionToAdd = "";

            for(auto&& pair: pairs) {
                if(pair.Name == "") {
                    restParams.push_back(pair);
                    continue;
                }
                if(option->Matches(pair.Name)) {
                    if(pair.HasValue) {
                        if(option->GetInnerType() == BaseOption::InnerType::BOOL) {
                            // strip out the value.  Bools don't get values, this was a rest parameter
                            optionToAdd = pair.Value;
                        }
                        option->Populate(pair.Value);
                    } else {
                        option->Populate("");
                    }
                    found = true;
                }
                if(optionToAdd.length() > 0) {
                    restParams.push_back(OptionValuePair("", optionToAdd));
                }
            }
            if(!found && option->IsRequired()) {
                throw std::exception(StrUtil::Format("%s option is required, but not provided", option->GetLongName()).c_str());
            }
        }

        if(m_RestOption) {
            for(auto&& param: restParams) {
                m_RestOption->Populate(param.Value);
            }
        }
    }
} // namespace CommandParser