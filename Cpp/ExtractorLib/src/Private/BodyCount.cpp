#include "Extractor/Private/BodyCount.h"

#include "Extractor/Private/LineFetcher.h"

namespace Extractor {
    namespace BodyCount {
        u32 GetBodyCount(const std::string& line, std::istream& stream) {
            auto nestingDepth = std::count(line.begin(), line.end(), '{');
            nestingDepth -= std::count(line.begin(), line.end(), '}');
            u32 lineCount = 0;
            std::string nextLine;
            while(nestingDepth > 0 && LineFetcher::GetNextLine(stream, nextLine)) {
                lineCount++;
                nestingDepth += std::count(nextLine.begin(), nextLine.end(), '{');
                nestingDepth -= std::count(nextLine.begin(), nextLine.end(), '}');
            }

            return lineCount;
        }
    } // namespace BodyCount
} // namespace Extractor
