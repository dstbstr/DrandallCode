#ifndef __DEFINEFILEPARSER_H__
#define __DEFINEFILEPARSER_H__

#include <string>
#include <unordered_map>

namespace Extractor {
    namespace DefineFileParser {
        std::unordered_map<std::string, std::string> Parse(std::istream& fileStream);
    }
} // namespace Extractor

#endif // __DEFINEFILEPARSER_H__