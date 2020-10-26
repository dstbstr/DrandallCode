#ifndef __OPTIONCOLLECTION_H__
#define __OPTIONCOLLECTION_H__
#include "CommandParser/Option.h"

#include <vector>

class OptionCollection {
public:
    OptionCollection& Add(Option&& option) {
        m_Options.push_back(std::move(option))
    }

private:
    std::vector<Option> m_Options;
};

#endif // __OPTIONCOLLECTION_H__