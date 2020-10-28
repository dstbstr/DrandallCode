#ifndef __OPTIONCOLLECTION_H__
#define __OPTIONCOLLECTION_H__
#include "CommandParser/OptionValuePair.h"
#include <vector>
#include <ostream>

namespace CommandParser {
    class BaseOption;

    class OptionCollection {
    public:
        OptionCollection(std::string description);
        OptionCollection& Add(BaseOption& option);
        std::vector<BaseOption*> Apply(std::vector<OptionValuePair> pairs);
        void PrintUsage(std::ostream& target) const;
    private:
        std::vector<BaseOption*> m_Options;
        std::string m_Description;
    };
} // namespace CommandParser
#endif // __OPTIONCOLLECTION_H__