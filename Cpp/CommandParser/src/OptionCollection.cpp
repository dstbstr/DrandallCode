#include "CommandParser/OptionCollection.h"

#include "CommandParser/Option.h"
#include "Utilities/Format.h"

namespace CommandParser {
    OptionCollection::OptionCollection(std::string description) : m_Description(description){};

    OptionCollection& OptionCollection::Add(BaseOption& option) {
        m_Options.push_back(&option);
        return *this;
    }

    void OptionCollection::PrintUsage(std::ostream& stream) const {
        stream << m_Description << std::endl;
        stream << "Options" << std::endl;
        stream << "Short Name | Long Name | Required | Flag | Help Text" << std::endl;
        stream << "----------------------------------------------------" << std::endl;

        for(auto&& option: m_Options) {
            stream << option->ToString() << std::endl;
        }
    }

    std::vector<BaseOption*> OptionCollection::Apply(std::vector<OptionValuePair> pairs) {
        std::vector<BaseOption*> result;

        for(auto&& option: m_Options) {
            bool found = false;
            for(auto&& pair: pairs) {
                if(option->Matches(pair.Name)) {
                    if(pair.HasValue) {
                        option->Populate(pair.Value);
                    }
                    result.push_back(option);
                    found = true;
                    break;
                }
            }
            if(!found && option->IsRequired()) {
                throw std::exception(StrUtil::Format("%s option is required, but not provided", option->GetLongName()).c_str());
            }
        }

        return result;
    }
} // namespace CommandParser