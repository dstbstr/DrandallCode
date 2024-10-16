#pragma once

#include <string>
#include <iostream>

namespace Extractor {
    namespace LineFetcher {
        bool JoinLines(std::string& line, std::istream& stream);
        bool GetNextLine(std::istream& stream, std::string& outString);
    }
}
