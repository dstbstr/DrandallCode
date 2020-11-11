#include "IncludeCounter/Extractors/FunctionDataExtractor.h"

#include "Utilities/Require.h"

#include <regex>

namespace {
    std::regex FunctionRegex("^" // start of string
                             "(template<[^>]*>\\s*)?" // optional template
                             "(virtual *)?(static *)?(const *)?\\s*" // function prefixes and return type modifiers
                             "[\\w&\\*:\\[\\]]+\\s+" // return type with potential qualifification or reference
                             "([\\w:]+)" // Function name
                             "\\(" // Start of parameters
                             "([^\\)]*)" // parameters
                             "\\)\\s*" // end of parameters
                             "(const *)?(final *)?(override *)?\\s*" // optional function modifiers
                             "(\\s*=\\s*0)?" // optional pure virtual indicator
                             ";?"); // optional declaration (instead of definition)

    /*
    match[1] = template
    match[2] = virtual
    match[3] = static
    match[4] = const return type
    match[5] = function name
    match[6] = arguments
    match[7] = const function
    match[8] = final
    match[9] = override
    match[10] = abstract
     */
    IncludeCounter::FunctionData GetFunctionData(std::string line, std::smatch match, std::string className, IncludeCounter::Visibility visibility) {
        IncludeCounter::FunctionData result;
        result.ClassName = className;
        result.Visibility = visibility;
        result.IsTemplated = match[1].length() > 0;
        result.IsStatic = match[3].length() > 0;
        result.FunctionName = match[5];
        result.IsConst = match[7].length() > 0;
        result.IsAbstract = match[10].length() > 0;

        auto parameters = match[6].str();
        if(parameters.empty()) {
            result.Airity = 0;
            result.DefaultParameterCount = 0;
        } else {
            result.Airity = std::count(parameters.begin(), parameters.end(), ',');
            // TODO: remove commas from template parameters
            // std::unordered_map<std::string, int> - remove one
            // std::unordered_set<std::string> - don't remove one

            result.DefaultParameterCount = std::count(parameters.begin(), parameters.end(), '=');
        }

        return result;
    }
} // namespace

namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsAFunction(const std::string& line) {
                return std::regex_search(line, FunctionRegex);
            }

            FunctionData Extract(std::string line, std::istream& stream, std::string className, Visibility visibility) {
                std::smatch match;
                Require::True(std::regex_search(line, match, FunctionRegex), "Failed to parse type.  Was IsAFunction run?");
                auto result = GetFunctionData(line, match, className, visibility);

                auto trimmed = StrUtil::Trim(line.substr(match[0].length()));
                auto nestingDepth = std::count(trimmed.begin(), trimmed.end(), '{');
                nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');

                std::string nextLine;
                while(nestingDepth > 0 && std::getline(stream, nextLine)) {
                    trimmed = StrUtil::Trim(nextLine);
                    nestingDepth += std::count(trimmed.begin(), trimmed.end(), '{');
                    nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');
                }
                return result;
            }
        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter