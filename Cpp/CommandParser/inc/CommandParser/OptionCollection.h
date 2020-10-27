#ifndef __OPTIONCOLLECTION_H__
#define __OPTIONCOLLECTION_H__
#include "CommandParser/Option.h"

#include <vector>

namespace CommandParser {
    class OptionCollection {
    public:
        OptionCollection& Add(BaseOption&& option) {
            m_Options.push_back(std::move(option));
        }

    private:
        std::vector<BaseOption> m_Options;
    };
} // namespace CommandParser
#endif // __OPTIONCOLLECTION_H__