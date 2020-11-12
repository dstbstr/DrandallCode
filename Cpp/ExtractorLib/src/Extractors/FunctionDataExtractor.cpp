#include "Extractor/FunctionDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Utilities/Require.h"

#include <regex>

namespace {
    std::regex TemplateRegex("<[^>]+>");
    std::regex FunctionRegex("^" // start of string
                             "(template<[^>]*>\\s*)?" // optional template
                             "(virtual *)?(static *)?(const *)?\\s*" // function prefixes and return type modifiers
                             "[\\w&\\*:\\[\\]<>]+\\s+" // return type with potential qualifification or reference
                             "([\\w:]+)\\s*" // Function name
                             "\\(" // Start of parameters
                             "([^\\)]*)" // parameters
                             "\\)?\\s*" // optional end of parameters (may split parameters on multiple lines)
                             "(const *)?(final *)?(override *)?\\s*" // optional function modifiers
                             "(\\s*=\\s*0)?\\s*" // optional pure virtual indicator
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
    Extractor::FunctionData GetFunctionData(std::smatch match, std::string className, Extractor::Visibility visibility) {
        Extractor::FunctionData result;
        result.ClassName = className;
        result.Visibility = visibility;
        result.IsTemplated = match[1].length() > 0;
        result.IsVirtual = match[2].length() > 0;
        result.IsStatic = match[3].length() > 0;
        result.FunctionName = match[5];
        result.IsConst = match[7].length() > 0;
        result.IsAbstract = match[10].length() > 0;

        auto parameters = match[6].str();
        if(parameters.empty()) {
            result.Airity = 0;
            result.DefaultParameterCount = 0;
        } else {
            parameters = std::regex_replace(parameters, TemplateRegex, "");
            result.Airity = (u8)std::count(parameters.begin(), parameters.end(), ',') + 1;
            result.DefaultParameterCount = (u8)std::count(parameters.begin(), parameters.end(), '=');
        }

        return result;
    }

    std::string JoinFunctionLine(std::string line, std::istream& stream) {
        using namespace Extractor;

        std::string result = StrUtil::Trim(line);
        bool isInCommentBlock = false;
        CommentExtractor::StripComments(result, isInCommentBlock);
        auto lastChar = result[result.length() - 1];
        if(lastChar == ';') {
            return result;
        }

        auto nestingDepth = std::count(result.begin(), result.end(), '{');
        nestingDepth -= std::count(result.begin(), result.end(), '}');
        if(lastChar == '}') {
            return result;
        }

        std::string nextLine;
        while(lastChar != ';' && lastChar != '{' && std::getline(stream, nextLine)) {
            auto trimmed = StrUtil::TrimStart(nextLine);
            CommentExtractor::StripComments(trimmed, isInCommentBlock);
            result += trimmed;
            lastChar = result[result.length() - 1];
        }

        return result;
    }
} // namespace

namespace Extractor {
    namespace FunctionDataExtractor {
        bool IsAFunction(const std::string& line) {
            return std::regex_search(line, FunctionRegex);
        }

        FunctionData Extract(std::string line, std::istream& stream, std::string className, Visibility visibility) {
            auto combinedLine = JoinFunctionLine(line, stream);
            std::smatch match;
            Require::True(std::regex_search(combinedLine, match, FunctionRegex), "Failed to parse type.  Was IsAFunction run?");
            auto result = GetFunctionData(match, className, visibility);

            auto trimmed = StrUtil::Trim(combinedLine.substr(match[0].length()));
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
} // namespace Extractor