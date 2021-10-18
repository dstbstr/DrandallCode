#include "Extractor/TypeDataExtractor.h"

#include "Extractor/Data/DefineData.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/BodyCount.h"
#include "Extractor/Private/CommentExtractor.h"
#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Extractor/Private/VisibilityExtractor.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtils.h"

#include <regex>

namespace {
    //^(template[\w <>=,:]+?> ?)?(class|enum|struct|union|interface) (?:\[\[\w+\]\] )? ?(?:[\w\(\)]+ )??([\w<>]+) ?(final ?)?:?( ?,?((?:public|protected|private|virtual) ?)? [\w:<>]+){0,10} ?(?:\{[^}]*\}?;?|$)
    std::regex TypeRegex("^" // start of the string
                         "(template[\\w <>=,:]+?> ?)?" // optionally start with a template
                         "(class|enum|struct|union|interface) " // required type
                         "(?:\\[\\[\\w+\\]\\] )?" // optional attributes
                         "(?:[\\w\\(\\)]+ )??" // optional declspec (macro or not)
                         "([\\w<>]+) ?" // required identifier
                         "(?:final ?)?" // optionally final
                         ":?( ?,?((?:public|protected|private|virtual) ?)? [\\w:<>]+){0,10} ?" // up to 5 base classes (the {0,10} is to avoid using * which causes infinite matches)
                         "(?:\\{[^}]*\\}?;?|$)", // Can't end with a semicolon unless it's preceeded by a close curly
                         std::regex_constants::optimize);

    std::regex SimpleTypeRegex("(?:class|enum|struct|union|interface).+(?:\\{|\\};)$");

    constexpr size_t TemplateIndex = 1;
    constexpr size_t TypeIndex = 2;
    constexpr size_t NameIndex = 3;
    constexpr size_t BaseClassIndex = 4;
    constexpr size_t InheritenceScopeIndex = 5;

    Extractor::TypeData GetTypeData(std::smatch match, const std::string& fileName) {
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
        bool IsAType(const std::string& line, std::smatch& outMatch) {
            // ScopedTimer timer("IsAType: " + line, ScopedTimer::TimeUnit::SECOND);
            try {
                if(!std::regex_search(line, SimpleTypeRegex)) {
                    return false;
                }
                return std::regex_search(line, outMatch, TypeRegex);
            } catch(std::exception& ex) {
                LOG_WARN(StrUtil::Format("Failed to determine if line is a type: %s.  Error: %s", line, ex.what()));
                return false;
            }
        }

        // Should extract a type (class, struct, union, enum) from the provided stream (and initial line)
        // Would almost certainly break with something like class Foo{struct Bar{union Baz{};};}; (in a single line)
        TypeData Extract(const std::smatch& match, const std::string& fileName, const std::string& ns, const DefineData& knownDefines, std::istream& stream) {
            // ScopedTimer timer("ExtractType: " + initialLine, ScopedTimer::TimeUnit::SECOND);

            auto result = GetTypeData(match, fileName);
            result.Namespace = ns;
            result.LineCount = 1;
            auto initialLine = match[0].str();
            if(initialLine.find('}') != initialLine.npos) {
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
            std::smatch nextMatch;

            // if some monster adds defines inside of a type, they don't deserve to have them preprocessed correctly...
            IfDefExtractor ifDefExtractor(knownDefines, stream);

            while(LineFetcher::GetNextLine(stream, line)) {
                if(ifDefExtractor.CanExtract(line)) {
                    result.IfDefCount++;
                    ifDefExtractor.Extract(line);
                    continue;
                }

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

                if(TypeDataExtractor::IsAType(line, nextMatch)) {
                    // if using Egyptian braces, need to remove the open curly from above
                    if(std::find(line.begin(), line.end(), '{') != line.end() && std::find(line.begin(), line.end(), '}') == line.end()) {
                        nestingDepth--;
                    }
                    auto innerType = TypeDataExtractor::Extract(nextMatch, fileName, ns, knownDefines, stream);
                    result.InnerTypes.push_back(innerType);
                    lineCount += innerType.LineCount - 1;
                } else if(FunctionDataExtractor::IsSpecialFunction(line, nextMatch)) {
                    auto function = FunctionDataExtractor::ExtractSpecialFunction(line, nextMatch, stream, ns, currentVisibility);
                    result.SpecialFunctions.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(FunctionDataExtractor::IsAFunction(line, nextMatch)) {
                    auto function = FunctionDataExtractor::ExtractFunction(line, nextMatch, stream, ns, result.ClassName, currentVisibility);
                    result.Functions.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(FunctionDataExtractor::IsOperatorOverload(line, nextMatch)) {
                    auto function = FunctionDataExtractor::ExtractOperatorOverload(line, nextMatch, stream, ns, result.ClassName, currentVisibility);
                    result.OperatorOverloads.push_back(function);
                    lineCount += function.LineCount - 1;
                } else if(line[line.length() - 1] == '{') {
                    lineCount += BodyCount::GetBodyCount(line, stream);
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