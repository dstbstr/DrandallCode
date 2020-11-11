#include "IncludeCounter/Extractors/TypeDataExtractor.h"

#include "IncludeCounter/Extractors/CommentExtractor.h"
#include "IncludeCounter/Extractors/FunctionDataExtractor.h"
#include "IncludeCounter/Extractors/VisibilityExtractor.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtilities.h"

#include <regex>

namespace {
    std::regex TypeRegex("^(template<[^>]*> *)?" // optionally start with a template
                         "((class)|(enum)|(struct)|(union))" // keyword
                         "\\s+(\\w+)\\s*" // identifier
                         "(:\\s*" // optional base class
                         "((public)|(protected)|(private))?\\s*" // optional scope of inheritence
                         "(virtual)?" // optional virtual inheritence
                         "\\s*[\\w:]+)?" // base class name
                         "\\s*((\\{[^\\}]*\\}?;?)|$)"); // can't end with a semicolon (may not have curly brace based on style)

    /* Matches
     [0] = Full string
     [1] = template
     [2] = type
     [7] = class name
     [8] = base class
     [9] = scope of inheritence
    [13] = Is inheritence virtual
    [14] = body
    */
    IncludeCounter::TypeData GetTypeData(std::smatch match, std::string fileName) {
        IncludeCounter::TypeData result;
        result.ClassName = match[7];
        result.FileName = fileName;
        result.HasBaseClass = match[8].length() > 0 || match[9].length() > 0;
        result.IsTemplated = match[1].length() > 0;

        auto typeStr = match[2];
        if(typeStr == "class") {
            result.TypeKind = IncludeCounter::TypeKeyword::CLASS;
        } else if(typeStr == "struct") {
            result.TypeKind = IncludeCounter::TypeKeyword::STRUCT;
        } else if(typeStr == "union") {
            result.TypeKind = IncludeCounter::TypeKeyword::UNION;
        } else if(typeStr == "enum") {
            result.TypeKind = IncludeCounter::TypeKeyword::ENUM;
        } else {
            Require::True(false, "Unknown type kind: " + typeStr.str());
        }

        return result;
    }
} // namespace

namespace IncludeCounter {
    namespace Extractors {
        namespace TypeDataExtractor {
            bool IsAType(const std::string& line) {
                return std::regex_search(line, TypeRegex);
            }

            // Should extract a type (class, struct, union, enum) from the provided stream (and initial line)
            // Would almost certainly break with something like class Foo{struct Bar{union Baz{};};}; (in a single line)
            TypeData Extract(const std::string& initialLine, const std::string& fileName, std::istream& stream) {
                std::smatch match;
                Require::True(std::regex_search(initialLine, match, TypeRegex), "Failed to parse type.  Was IsAType run?");
                auto result = GetTypeData(match, fileName);

                if(std::find(initialLine.begin(), initialLine.end(), '}') != initialLine.end()) {
                    // single line declaration
                    // TODO: should probably find members
                    return result;
                }

                Visibility currentVisibility = Visibility::PUBLIC;
                if(result.TypeKind == TypeKeyword::CLASS) {
                    currentVisibility = Visibility::PRIVATE;
                }
                bool isInBlockComment = false;
                u64 nestingDepth = std::count(initialLine.begin(), initialLine.end(), '{');
                std::string line;
                while(std::getline(stream, line)) {
                    CommentExtractor::StripComments(line, isInBlockComment);
                    auto trimmed = StrUtil::Trim(line);
                    if(trimmed == "") {
                        continue;
                    }

                    nestingDepth += std::count(trimmed.begin(), trimmed.end(), '{');
                    nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');
                    if(nestingDepth <= 0) {
                        // TODO: If this is the first line, remove the class bits
                        break; // Yay, we found the closing curly
                    }

                    if(VisibilityExtractor::HasVisibility(trimmed)) {
                        currentVisibility = VisibilityExtractor::ExtractVisibility(trimmed);
                        trimmed = StrUtil::Trim(trimmed);
                        if(trimmed == "") {
                            continue;
                        }
                    }

                    if(TypeDataExtractor::IsAType(trimmed)) {
                        // if using Egyptian braces, need to remove the open curly from above
                        if(std::find(trimmed.begin(), trimmed.end(), '{') != trimmed.end() &&
                           std::find(trimmed.begin(), trimmed.end(), '}') == trimmed.end()) {
                            nestingDepth--;
                        }
                        result.InnerTypes.push_back(TypeDataExtractor::Extract(trimmed, fileName, stream));
                    } else if(FunctionDataExtractor::IsAFunction(trimmed)) {
                        result.Functions.push_back(FunctionDataExtractor::Extract(trimmed, stream, result.ClassName, currentVisibility));
                    } else if(trimmed[trimmed.length() - 1] == ';') {
                        // can we assume that this is a member variable?  What else is left?
                        switch(currentVisibility) {
                        case Visibility::PUBLIC: result.PublicDataMemberCount++; break;
                        case Visibility::PRIVATE: result.PrivateDataMemberCount++; break;
                        case Visibility::PROTECTED: result.ProtectedDataMemberCount++; break;
                        }
                    }
                }

                return result;
            }

        } // namespace TypeDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter