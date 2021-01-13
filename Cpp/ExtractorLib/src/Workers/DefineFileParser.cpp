#include "Extractor/Workers/DefineFileParser.h"

#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <filesystem>
#include <fstream>
#include <unordered_set>

namespace Extractor {
    namespace DefineFileParser {
        std::vector<std::string> Parse(std::istream& stream) {
            std::string line;
            std::unordered_set<std::string> result;
            while(std::getline(stream, line)) {
                for(auto&& token: StrUtil::Split(line, " ")) {
                    if(!token.empty()) {
                        result.insert(std::string(token));
                    }
                }
            }

            return std::vector<std::string>{result.begin(), result.end()};
        }
    } // namespace DefineFileParser
} // namespace Extractor