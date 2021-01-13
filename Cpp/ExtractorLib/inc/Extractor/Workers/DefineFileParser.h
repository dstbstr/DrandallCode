#ifndef __DEFINEFILEPARSER_H__
#define __DEFINEFILEPARSER_H__

#include <string>
#include <vector>

namespace Extractor {
    namespace DefineFileParser {
        std::vector<std::string> Parse(std::istream& fileStream);
    }
} // namespace Extractor

#endif // __DEFINEFILEPARSER_H__