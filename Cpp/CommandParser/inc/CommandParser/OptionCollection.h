#ifndef __OPTIONCOLLECTION_H__
#define __OPTIONCOLLECTION_H__
#include "CommandParser/OptionValuePair.h"

#include <ostream>
#include <vector>

namespace CommandParser {
    class BaseOption;

    class OptionCollection {
    public:
        OptionCollection(std::string description);
        OptionCollection& Add(BaseOption& option);
        void Validate();
        void Apply(std::vector<OptionValuePair> pairs);
        void PrintUsage(std::ostream& target) const;

    private:
        std::vector<BaseOption*> m_Options;
        std::string m_Description;
        BaseOption* m_RestOption {nullptr};
    };
} // namespace CommandParser
#endif // __OPTIONCOLLECTION_H__