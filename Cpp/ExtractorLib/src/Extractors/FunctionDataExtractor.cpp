#include "Extractor/FunctionDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"

#include <regex>

namespace {
    std::regex SpecialFunctionRegex("^(?:template[\\w <>=,:]+?> ?)?(virtual )?(explicit )?(~?_*[A-Z]+[a-z][\\w:<>~]+)\\(([^\\)]*)\\)? *=? *(default)?(delete)?", std::regex_constants::optimize);
    std::regex OperatorOverloadRegex("^(explicit +)?(friend +)?(inline +)?" // optional specifier
                                     "(?:const *)?(?:.+)? *" // optional return type
                                     "operator *" // required operator keyword
                                     "(.+?) *" // specific operator being overloaded
                                     "\\(([^\\)]*)\\) *" // parameters
                                     "(?:const *)?", // optional const
                                     std::regex_constants::optimize);
    std::regex SimpleOperatorRegex("(?:operator)");

    std::regex FunctionRegex("^" // start of string
                             "(template[\\w <>=,:]+?> ?)?" // optional template
                             "((?:virtual |_?_?(?:force)?inline |static )+)?" // function prefixes
                             "(?:[\\w\\(\\)\\[\\]:<>]+? ?){0,3}" // optional declspec or attributes
                             "(?:const )?" // return type const
                             "[\\w\\[\\]&\\*:<>]{0,3}[&\\*\\w\\]>] " // return type with potential qualifification or reference
                             "(?: ?const )?[\\*&]? ?" // support RTL const
                             "([\\w:<>]+) ?" // Function name
                             "\\(" // Start of parameters
                             "([^\\)]*?)" // parameters
                             "\\) ?" // end of parameters
                             "((?:const ?|final ?|override ?){0,3})? ?" // optional function modifiers
                             "( ?= ?0)? ?" // optional pure virtual indicator
                             ";?", // optional declaration (instead of definition)
                             std::regex_constants::optimize);
    std::regex SimpleFunctionRegex("\\(");

    std::regex TemplateRegex("<[^>]+>");
    std::regex VirtualRegex("virtual");
    std::regex InlineRegex("(__(force)?)?inline");
    std::regex StaticRegex("static");
    std::regex StaticAssertRegex("^static_assert");

    std::regex ConstRegex("const");

    constexpr size_t TemplateIndex = 1;
    constexpr size_t PrefixIndex = 2;
    constexpr size_t FunctionNameIndex = 3;
    constexpr size_t ArgIndex = 4;
    constexpr size_t PostfixIndex = 5;
    constexpr size_t AbstractIndex = 6;

    Extractor::FunctionData GetFunctionData(std::smatch match, const std::string& ns, const std::string& className, Extractor::Visibility visibility) {
        Extractor::FunctionData result;
        result.Namespace = ns;
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

    // ^(?:template *<[^>]*> *)?(virtual *)?(explicit *)?([\\w:<>]+)\\(([^\\)]*)\\)? *=? *(default)?(delete)?

    Extractor::SpecialFunctionData GetSpecialFunctionData(std::smatch match, const std::string& ns, Extractor::Visibility visibility) {
        Extractor::SpecialFunctionData result;
        result.Namespace = ns;

        auto className = match[3].str();
        auto split = StrUtil::Split(className, "::");
        result.ClassName = split[split.size() - 1];

        result.Visibility = visibility;
        result.IsVirtual = match[1].length() > 0;
        result.IsExplicit = match[2].length() > 0;
        result.Kind = std::find(className.begin(), className.end(), '~') == className.end() ? Extractor::SpecialFunctionType::CONSTRUCTOR : Extractor::SpecialFunctionType::DESTRUCTOR;
        result.IsDefaulted = match[5].length() > 0;
        result.IsDeleted = match[6].length() > 0;

        auto parameters = match[4].str();
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

    /*
        ^(explicit +)?(friend +)?(inline +)?
        (?:const *)?(?:.+) *
        operator *
        (.+) *
        \\(([^\\)]*)\\) *
        (?:const *)?
    */
    Extractor::OperatorOverloadData GetOperatorOverload(std::smatch match, const std::string& ns, const std::string& className, Extractor::Visibility visibility) {
        Extractor::OperatorOverloadData result;
        result.Namespace = ns;
        result.ClassName = className;
        result.Visibility = visibility;
        result.IsExplicit = match[1].length() > 0;
        result.IsFriend = match[2].length() > 0;
        result.IsInline = match[3].length() > 0;
        result.Operator = StrUtil::Trim(match[4].str());

        auto parameters = match[5].str();
        if(parameters.empty()) {
            result.Airity = 0;
        } else {
            parameters = std::regex_replace(parameters, TemplateRegex, "");
            result.Airity = (u8)std::count(parameters.begin(), parameters.end(), ',') + 1;
        }

        return result;
    }

    u64 CountLinesInBody(std::string line, std::istream& stream) {
        auto trimmed = StrUtil::Trim(line);
        auto nestingDepth = std::count(trimmed.begin(), trimmed.end(), '{');
        nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');
        u64 lineCount = 0;
        std::string nextLine;
        while(nestingDepth > 0 && std::getline(stream, nextLine)) {
            lineCount++;
            trimmed = StrUtil::Trim(nextLine);
            nestingDepth += std::count(trimmed.begin(), trimmed.end(), '{');
            nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');
        }

        return lineCount;
    }
} // namespace

namespace Extractor {
    namespace FunctionDataExtractor {
        bool IsAFunction(const std::string& line, std::smatch& outMatch) {
            // ScopedTimer timer("IsAFunction: " + line, ScopedTimer::TimeUnit::SECOND);
            try {
                if(!std::regex_search(line, SimpleFunctionRegex)) {
                    return false;
                }
                return std::regex_search(line, outMatch, FunctionRegex);
            } catch(...) {
                LOG_ERROR(StrUtil::Format("Failed to determine if line is a function: %s", line));
                return false;
            }
        }

        bool IsSpecialFunction(const std::string& line, std::smatch& outMatch) {
            // ScopedTimer timer("IsSpecialFunction: " + line, ScopedTimer::TimeUnit::SECOND);
            try {
                if(!std::regex_search(line, SimpleFunctionRegex)) {
                    return false;
                }
                return std::regex_search(line, outMatch, SpecialFunctionRegex) && !std::regex_search(line, StaticAssertRegex);
            } catch(...) {
                LOG_ERROR(StrUtil::Format("Failed to determine if line is a special function: %s", line));
                return false;
            }
        }

        bool IsOperatorOverload(const std::string& line, std::smatch& outMatch) {
            // ScopedTimer timer("IsOperatorOverload: " + line, ScopedTimer::TimeUnit::SECOND);
            try {
                if(!std::regex_search(line, SimpleOperatorRegex)) {
                    return false;
                }
                return std::regex_search(line, outMatch, OperatorOverloadRegex);
            } catch(...) {
                LOG_ERROR(StrUtil::Format("Failed to determine if line is an operation overload: %s", line));
                return false;
            }
        }

        FunctionData ExtractFunction(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, const std::string& className, Visibility visibility) {
            // ScopedTimer timer("ExtractFunction: " + line, ScopedTimer::TimeUnit::SECOND);
            auto result = GetFunctionData(match, ns, className, visibility);

            u64 bodyLineCount = CountLinesInBody(line.substr(match[0].length()), stream);
            result.LineCount = bodyLineCount + 1;
            return result;
        }

        SpecialFunctionData ExtractSpecialFunction(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, Visibility visibility) {
            // ScopedTimer timer("ExtractSpecialFunction: " + line, ScopedTimer::TimeUnit::SECOND);
            auto result = GetSpecialFunctionData(match, ns, visibility);

            u64 bodyLineCount = CountLinesInBody(line.substr(match[0].length()), stream);
            result.LineCount = bodyLineCount + 1;
            return result;
        }

        OperatorOverloadData ExtractOperatorOverload(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, const std::string& className, Visibility visibility) {
            // ScopedTimer timer("ExtractOperatorOverload: " + line, ScopedTimer::TimeUnit::SECOND);
            auto result = GetOperatorOverload(match, ns, className, visibility);

            u64 bodyLineCount = CountLinesInBody(line.substr(match[0].length()), stream);
            result.LineCount = bodyLineCount + 1;
            return result;
        }

    } // namespace FunctionDataExtractor
} // namespace Extractor