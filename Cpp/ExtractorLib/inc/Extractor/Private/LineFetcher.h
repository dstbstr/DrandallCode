#ifndef __LINEFETCHER_H__
#define __LINEFETCHER_H__

#include <string>
#include <iostream>

namespace Extractor {
    namespace LineFetcher {
        bool JoinLines(std::string& line, std::istream& stream);
        bool GetNextLine(std::istream& stream, std::string& outString);
    }
}
#endif // __LINEFETCHER_H__