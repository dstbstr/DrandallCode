#include "CommandParser/OptionCollection.h"

#include "CommandParser/Option.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <unordered_set>

namespace CommandParser {
    OptionCollection::OptionCollection(std::string description) : m_Description(description){};

    OptionCollection& OptionCollection::Add(BaseOption& option) {
        if(option.IsRestOption()) {
            Require::Null(m_RestOption, "Received multiple options which match rest args");
            m_RestOption = &option;
        } else {
            m_Options.push_back(&option);
        }
        return *this;
    }

    void OptionCollection::Validate() {
        std::unordered_set<std::string> shortNames;
        std::unordered_set<std::string> longNames;

        for(auto&& option: m_Options) {
            Require::False(shortNames.insert(option->GetShortName()).second, "Duplicate short name: " + option->GetShortName());
            Require::False(longNames.insert(option->GetLongName()).second, "Duplicate long name: " + option->GetLongName());
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
            Require::True(found || !option->IsRequired(), option->GetLongName() + " option is required, but was not provided");
        }

        if(m_RestOption) {
            for(auto&& param: restParams) {
                m_RestOption->Populate(param.Value);
            }
        }
    }
} // namespace CommandParser