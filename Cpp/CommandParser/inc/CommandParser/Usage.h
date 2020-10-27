#ifndef __USAGE_H__
#define __USAGE_H__

#include "CommandParser/OptionCollection.h"

#include <iostream>
#include <string>


namespace CommandParser {
    class Usage {
    public:
        explicit Usage(std::string description, OptionCollection optionCollection) : m_Description(description), m_Options(optionCollection) {}

        void Print(std::ostream targetStream) const;

    private:
        std::string m_Description;
        OptionCollection m_Options;
    };
} // namespace CommandParser
#endif //__USAGE_H__