#ifndef __OPTIONCOLLECTION_H__
#define __OPTIONCOLLECTION_H__
#include "CommandParser/Option.h"
#include "CommandParser/OptionValuePair.h"
#include "Utilities/Format.h"
#include <vector>

namespace CommandParser {
    class OptionCollection {
    public:
        OptionCollection& Add(BaseOption& option) {
            m_Options.push_back(std::move(&option));
            return *this;
        }

        std::vector<BaseOption*> Apply(std::vector<OptionValuePair> pairs) {
            std::vector<BaseOption*> result;

            for(auto&& option : m_Options) {
                bool found = false;
                for(auto&& pair : pairs) {
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
    private:
        
        std::vector<BaseOption*> m_Options;
    };
} // namespace CommandParser
#endif // __OPTIONCOLLECTION_H__