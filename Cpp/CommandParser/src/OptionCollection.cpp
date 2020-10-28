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
        stream << " S |  LongName | R | F | Help Text" << std::endl;
        stream << "----------------------------------------------------" << std::endl;

        for(auto&& option: m_Options) {
            stream << option->ToString() << std::endl;
        }
    }

    void OptionCollection::Apply(std::vector<OptionValuePair> pairs) {
        for(auto&& option: m_Options) {
            bool found = false;
            for(auto&& pair: pairs) {
                if(option->Matches(pair.Name)) {
                    if(pair.HasValue) {
                        option->Populate(pair.Value);
                    } else {
                        option->Populate("");
                    }
                    found = true;
                }
            }
            if(!found && option->IsRequired()) {
                throw std::exception(StrUtil::Format("%s option is required, but not provided", option->GetLongName()).c_str());
            }
        }
    }
} // namespace CommandParser