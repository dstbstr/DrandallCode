#include "Extractor/Private/DefineExtractor.h"

#include "Utilities/Format.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtils.h"

#include <regex>

// todo: handle undef?

namespace {
    constexpr char DefinePrefix[] = "#define ";
    static const std::regex DefinitionRegex("^#define (\\w+(?:\\([^\\)]*\\))?) ?(.+)?");
} // namespace

namespace Extractor {
    namespace DefineExtractor {
        bool CanExtract(const std::string& line) {
            return StrUtil::StartsWith(line, DefinePrefix);
        }

        std::pair<std::string, std::string> Extract(const std::string& line) {
            std::smatch match;
            Require::True(std::regex_search(line, match, DefinitionRegex), StrUtil::Format("%s did not match the regex.  Was CanExtract run?", line));

            return std::make_pair(match[1].str(), match[2].str());
        }
    } // namespace DefineExtractor
} // namespace Extractor