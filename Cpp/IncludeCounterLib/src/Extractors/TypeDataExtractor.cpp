#include "IncludeCounter/Extractors/TypeDataExtractor.h"

#include "IncludeCounter/Extractors/CommentExtractor.h"
#include "IncludeCounter/Extractors/FunctionDataExtractor.h"
#include "IncludeCounter/Extractors/VisibilityExtractor.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtilities.h"

#include <regex>

namespace {
    std::regex TypeRegex("^(template<[^>]+> *)?" // optionally start with a template
                         "((class)|(enum)|(struct)|(union))" // keyword
                         "\\s+(\\w+)\\s*" // identifier
                         "(:\\s*" // optional base class
                         "((public)|(protected)|(private))?" // optional scope of inheritence
                         "(virtual\\s*)?" // optional virtual inheritence
                         "\\s*[\\w:]+)?" // base class name
                         "\\s*[^;]?$"); // can't end with a semicolon (may not have curly brace based on style)
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
                TypeData result;
                result.ClassName = match[3];
                result.FileName = fileName;
                Visibility currentVisibility = Visibility::PUBLIC;
                auto typeStr = match[2];
                if(typeStr == "class") {
                    result.TypeKind = TypeKeyword::CLASS;
                    currentVisibility = Visibility::PRIVATE;
                } else if(typeStr == "struct") {
                    result.TypeKind = TypeKeyword::STRUCT;
                } else if(typeStr == "union") {
                    result.TypeKind = TypeKeyword::UNION;
                } else if(typeStr == "enum") {
                    result.TypeKind = TypeKeyword::ENUM;
                } else {
                    Require::True(false, "Unknown type kind: " + typeStr.str());
                }

                std::string line = initialLine.substr(match[0].length());
                bool isInBlockComment = false;
                u64 nestingDepth = 1;
                // User may define the type in a single line
                // struct Foo{int a; int b;};
                do {
                    CommentExtractor::StripComments(line, isInBlockComment);
                    auto trimmed = StrUtil::Trim(line);
                    if(trimmed == "") {
                        continue;
                    }

                    nestingDepth += std::count(trimmed.begin(), trimmed.end(), '{');
                    nestingDepth -= std::count(trimmed.begin(), trimmed.end(), '}');
                    if(nestingDepth == 0) {
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
                        result.InnerTypes.push_back(TypeDataExtractor::Extract(trimmed, fileName, stream));
                    } else if(FunctionDataExtractor::IsLineAFunction(trimmed)) {
                        result.Functions.push_back(FunctionDataExtractor::Extract(stream, currentVisibility));
                    } else {
                        // can we assume that this is a member variable?  What else is left?
                        switch(currentVisibility) {
                        case Visibility::PUBLIC: result.PublicDataMemberCount++; break;
                        case Visibility::PRIVATE: result.PrivateDataMemberCount++; break;
                        case Visibility::PROTECTED: result.ProtectedDataMemberCount++; break;
                        }
                    }
                } while(std::getline(stream, line));

                return result;
            }

        } // namespace TypeDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter