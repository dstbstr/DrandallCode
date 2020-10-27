#ifndef __OPTIONVALUEPAIR_H__
#define __OPTIONVALUEPAIR_H__

#include <string>

namespace CommandParser {
    struct OptionValuePair {
        OptionValuePair(std::string name) : Name(name), HasValue(false) {}
        OptionValuePair(std::string name, std::string value) : Name(name), Value(value), HasValue(true) {}

        std::string Name;
        std::string Value;
        bool HasValue;
    };
} // namespace CommandParser
#endif // __OPTIONVALUEPAIR_H__