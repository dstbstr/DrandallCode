#include "Extractor/TypeDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Extractor/VisibilityExtractor.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtils.h"

#include <regex>


namespace {
    std::regex TypeRegex("^(template *<[^>]*> *)?" // optionally start with a template
                         "((?:class)|(?:enum)|(?:struct)|(?:union)|(?:interface))" // keyword
                         "\\s+(\\w+)\\s*" // identifier
                         "(:\\s*" // optional base class
                         "((?:public)|(?:protected)|(?:private))?\\s*" // optional scope of inheritence
                         "(?:virtual)?" // optional virtual inheritence
                         "\\s*[\\w:<>]+)?" // base class name
                         "\\s*((\\{[^\\}]*\\}?;?)|$)"); // can't end with a semicolon (may not have curly brace based on style)

    constexpr size_t TemplateIndex = 1;
    constexpr size_t TypeIndex = 2;
    constexpr size_t NameIndex = 3;
    constexpr size_t BaseClassIndex = 4;
    constexpr size_t InheritenceScopeIndex = 5;

    Extractor::TypeData GetTypeData(std::smatch match, std::string fileName) {
        Extractor::TypeData result;
        result.ClassName = match[NameIndex];
        result.FileName = fileName;
        result.HasBaseClass = match[BaseClassIndex].length() > 0 || match[InheritenceScopeIndex].length() > 0;
        result.IsTemplated = match[TemplateIndex].length() > 0;

        auto typeStr = match[TypeIndex];
        if(typeStr == "class") {
            result.TypeKind = Extractor::TypeKeyword::CLASS;
        } else if(typeStr == "struct" || typeStr == "interface") {
            result.TypeKind = Extractor::TypeKeyword::STRUCT;
        } else if(typeStr == "union") {
            result.TypeKind = Extractor::TypeKeyword::UNION;
        } else if(typeStr == "enum") {
            result.TypeKind = Extractor::TypeKeyword::ENUM;
        } else {
            Require::True(false, "Unknown type kind: " + typeStr.str());
        }

        return result;
    }
} // namespace

namespace Extractor {
    namespace TypeDataExtractor {
        bool IsAType(const std::string& line) {
            return std::regex_search(line, TypeRegex);
        }

        // Should extract a type (class, struct, union, enum) from the provided stream (and initial line)
        // Would almost certainly break with something like class Foo{struct Bar{union Baz{};};}; (in a single line)
        TypeData Extract(const std::string& initialLine, const std::string& fileName, const std::string& ns, std::istream& stream) {
            std::smatch match;
            Require::True(std::regex_search(initialLine, match, TypeRegex), "Failed to parse type.  Was IsAType run?");
            auto result = GetTypeData(match, fileName);
            result.Namespace = ns;
            result.LineCount = 1;

            if(std::find(initialLine.begin(), initialLine.end(), '}') != initialLine.end()) {
                // single line declaration
                // TODO: should probably find members
                return result;
            }

            Visibility currentVisibility = Visibility::PUBLIC;
            if(result.TypeKind == TypeKeyword::CLASS) {
                currentVisibility = Visibility::PRIVATE;
            }
            u64 nestingDepth = std::count(initialLine.begin(), initialLine.end(), '{');
            u64 lineCount = 0;
            std::string line;
            while(LineFetcher::GetNextLine(stream, line)) {
                lineCount++;
                nestingDepth += std::count(line.begin(), line.end(), '{');
                nestingDepth -= std::count(line.begin(), line.end(), '}');
                if(nestingDepth <= 0) {
                    // TODO: If this is the first line, remove the class bits
                    break; // Yay, we found the closing curly
                }

                if(VisibilityExtractor::HasVisibility(line)) {
                    currentVisibility = VisibilityExtractor::ExtractVisibility(line);
                    line = StrUtil::Trim(line);
                    if(line == "") {
                        continue;
                    }
                }

                if(TypeDataExtractor::IsAType(line)) {
                    // if using Egyptian braces, need to remove the open curly from above
                    if(std::find(line.begin(), line.end(), '{') != line.end() && std::find(line.begin(), line.end(), '}') == line.end()) {
                        nestingDepth--;
                    }
                    auto innerType = TypeDataExtractor::Extract(line, fileName, ns, stream);
                    result.InnerTypes.push_back(innerType);
                    lineCount += innerType.LineCount - 1;
                } else if(FunctionDataExtractor::IsAFunction(line)) {
                    auto function = FunctionDataExtractor::ExtractFunction(line, stream, ns, result.ClassName, currentVisibility);
                    result.Functions.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(FunctionDataExtractor::IsSpecialFunction(line)) {
                    auto function = FunctionDataExtractor::ExtractSpecialFunction(line, stream, ns, currentVisibility);
                    result.SpecialFunctions.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(FunctionDataExtractor::IsOperatorOverload(line)) {
                    auto function = FunctionDataExtractor::ExtractOperatorOverload(line, stream, ns, result.ClassName, currentVisibility);
                    result.OperatorOverloads.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(line[line.length() - 1] == ';') {
                    // can we assume that this is a member variable?  What else is left?
                    switch(currentVisibility) {
                    case Visibility::PUBLIC: result.PublicDataMemberCount++; break;
                    case Visibility::PRIVATE: result.PrivateDataMemberCount++; break;
                    case Visibility::PROTECTED: result.ProtectedDataMemberCount++; break;
                    }
                }
            }

            result.LineCount += lineCount;
            return result;
        }

    } // namespace TypeDataExtractor
} // namespace Extractor