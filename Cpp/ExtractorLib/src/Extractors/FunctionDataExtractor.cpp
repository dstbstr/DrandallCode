#include "Extractor/FunctionDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Utilities/Require.h"

#include <regex>

namespace {
    std::regex TemplateRegex("<[^>]+>");
    std::regex FunctionRegex("^" // start of string
                             "(template<[^>]*>\\s*)?" // optional template
                             "((?:(?:virtual *)|(?:(?:__(force)?)?inline *)|(?:static *))*)?" // function prefixes
                             "(?:const *)?" // return type const
                             "[\\w&\\*:\\[\\]<>]+\\s+" // return type with potential qualifification or reference
                             "([\\w:]+)\\s*" // Function name
                             "\\(" // Start of parameters
                             "([^\\)]*)" // parameters
                             "\\)?\\s*" // optional end of parameters (may split parameters on multiple lines)
                             "((?:(?:const *)|(?:final *)|(?:override *))*)?\\s*" // optional function modifiers
                             "(\\s*=\\s*0)?\\s*" // optional pure virtual indicator
                             ";?"); // optional declaration (instead of definition)
    std::regex VirtualRegex("virtual");
    std::regex InlineRegex("(__(force)?)?inline");
    std::regex StaticRegex("static");

    std::regex ConstRegex("const");

    constexpr size_t TemplateIndex = 1;
    constexpr size_t PrefixIndex = 2;
    constexpr size_t FunctionNameIndex = 4;
    constexpr size_t ArgIndex = 5;
    constexpr size_t PostfixIndex = 6;
    constexpr size_t AbstractIndex = 7;

    Extractor::FunctionData GetFunctionData(std::smatch match, std::string className, Extractor::Visibility visibility) {
        Extractor::FunctionData result;
        result.ClassName = className;
        result.Visibility = visibility;

        result.IsTemplated = match[TemplateIndex].length() > 0;

        if(match[PrefixIndex].length() > 0) {
            auto prefixes = match[PrefixIndex].str();
            result.IsVirtual = std::regex_search(prefixes, VirtualRegex);
            result.IsInline = std::regex_search(prefixes, InlineRegex);
            result.IsStatic = std::regex_search(prefixes, StaticRegex);
        }
        result.FunctionName = match[FunctionNameIndex];

        if(match[PostfixIndex].length() > 0) {
            auto postfixes = match[PostfixIndex].str();
            result.IsConst = std::regex_search(postfixes, ConstRegex);
        }
        result.IsAbstract = match[AbstractIndex].length() > 0;

        auto parameters = match[ArgIndex].str();
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