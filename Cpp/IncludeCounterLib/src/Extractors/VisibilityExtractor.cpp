#include "IncludeCounter/Extractors/VisibilityExtractor.h"

#include "Utilities/Require.h"

#include <regex>

namespace {
    std::regex VisibilityRegex("^((private)|(public)|(protected)):");
}

namespace IncludeCounter {
    namespace Extractors {
        namespace VisibilityExtractor {
            bool HasVisibility(const std::string& line) {
                return std::regex_search(line, VisibilityRegex);
            }

            Visibility ExtractVisibility(std::string& line) {
                std::smatch match;
                std::regex_search(line, match, VisibilityRegex);
                Visibility result = Visibility::PUBLIC;
                auto name = match[1];
                Require::False(name == "", "Expected a match, but didn't get one.  Was HasVisibility been called first?");
                if(name == "private") {
                    result = Visibility::PRIVATE;
                } else if(name == "public") {
                    result = Visibility::PUBLIC;
                } else if(name == "protected") {
                    result = Visibility::PROTECTED;
                }

                line = line.substr(match[0].length());
                return result;
            }
        } // namespace VisibilityExtractor
    } // namespace Extractors
} // namespace IncludeCounter
