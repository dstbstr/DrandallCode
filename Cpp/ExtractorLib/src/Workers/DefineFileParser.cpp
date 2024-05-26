#include "Extractor/Workers/DefineFileParser.h"

#include "Core/Utilities/Format.h"
#include "Core/Utilities/Require.h"

#include <filesystem>
#include <fstream>

namespace Extractor {
    namespace DefineFileParser {
        std::unordered_map<std::string, std::string> Parse(std::istream& stream) {
            std::string line;
            std::unordered_map<std::string, std::string> result;
            while(std::getline(stream, line)) {
                for(const auto& token: StrUtil::Split(line, " ")) {
                    if(!token.empty()) {
                        auto keyValue = StrUtil::Split(token, "=");
                        if(keyValue.size() > 1) {
                            result[std::string(keyValue[0])] = keyValue[1];
                        } else {
                            result[std::string(token)] = "";
                        }
                    }
                }
            }

            return result;
        }
    } // namespace DefineFileParser
} // namespace Extractor