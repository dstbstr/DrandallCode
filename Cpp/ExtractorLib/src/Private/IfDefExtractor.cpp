#include "Extractor/Private/IfDefExtractor.h"

#include "Extractor/Private/LineFetcher.h"
#include "Utilities/StringUtils.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"

#include <regex>

static const std::string endIf = "#endif";
static const std::string elseStr = "#else";
static const std::regex defineRegex(R"(defined\(([^\)]+)\))");
static const std::regex ifdefRegex(R"(#(?:el)?if(?:def)?))");
static const std::regex ifndefRegex("#ifndef");
static const std::regex spaceRegex(" +");

//#ifdef A && ( !defined(B) || defined(C))
//#else
//#endif

namespace {
    bool ContainsInnerExpression(const std::string& line) {
        return line.find('(') != line.npos;
    }

    std::string ExtractInnerExpression(std::string& line) {
        auto closeParen = line.find_first_of(')');
        std::string substr = line.substr(0, closeParen);
        auto openParen = substr.find_last_of('(');
        auto prefix = line.substr(0, openParen);
        auto suffix = line.substr(closeParen);
        
        line = prefix + suffix;
        return line.substr(openParen + 1, closeParen - openParen);
    }

    bool EvaluateValue(const std::string& value, const std::vector<std::string>& knownDefines) {
        if(value[0] == '!') {
            return std::find(knownDefines.cbegin(), knownDefines.cend(), value.substr(1)) == knownDefines.cend();
        }
        return std::find(knownDefines.cbegin(), knownDefines.cend(), value) != knownDefines.cend();
    }

    enum Symbol {AND, OR, XOR};

    bool EvaluateExpression(const std::string& line, const std::vector<std::string>& knownDefines) {
        auto symbolIndex = line.find_first_of("&|");
        if(symbolIndex == line.npos) {
            return EvaluateValue(line, knownDefines);
        }

        std::string value = line.substr(0, symbolIndex);
        bool result = EvaluateValue(value, knownDefines);
        while(symbolIndex != line.npos) {
            Symbol symbol;
            switch(line[symbolIndex]) {
                case '&' : symbol = Symbol::AND; break;
                case '|' : symbol = Symbol::OR; break;
                default: LOG_ERROR(StrUtil::Format("Unrecognized symbol: %s", line[symbolIndex])); return false;
            }

            auto nextSymbolIndex = line.find_first_of("&|", symbolIndex + 2);
            if(nextSymbolIndex == line.npos) {
                value = line.substr(symbolIndex + 1);
            } else {
                value = line.substr(symbolIndex + 1, nextSymbolIndex - symbolIndex + 1);
            }
            bool rhs = EvaluateValue(value, knownDefines);

            switch(line[symbolIndex]) {
                case '&' : result = result && rhs; break;
                case '|' : result = result || rhs; break;
            }
            symbolIndex = nextSymbolIndex;
        }

        return result;
    }
}

namespace Extractor {
    bool IfDefExtractor::CanExtract(const std::string& line) {
        return StrUtil::StartsWith(line, "#if") || StrUtil::StartsWith(line, endIf) || StrUtil::StartsWith(line, elseStr) || StrUtil::StartsWith(line, "#elif");
    }

    void IfDefExtractor::Extract(std::string& line) {
        if(StrUtil::StartsWith(line, endIf)) {
            m_DefineDepth--;
            m_MatchedIfDef = false;
            line = line.substr(endIf.length());
            return;
        }

        if(m_MatchedIfDef) {
            SkipBody(line);
            return;
        }

        if(StrUtil::StartsWith(line, elseStr)) {
            m_MatchedIfDef = true;
            line = line.substr(elseStr.length());
            return;
        }

        line = std::regex_replace(line, defineRegex, "$1"); // turn #if defined(X) into X
        line = std::regex_replace(line, ifdefRegex, ""); // turn #ifdef X into X
        line = std::regex_replace(line, ifndefRegex, "!"); // turn #ifndef X into !X
        line = std::regex_replace(line, spaceRegex, ""); //remove all the spaces

        if(line.find('(') != line.npos) {

        }

        // remove all if/ifdef/elif
        // look for parens
        // split on && and ||
        // do something with !
    }

    //private
    void IfDefExtractor::SkipBody(std::string& line) {
        while(Extractor::LineFetcher::GetNextLine(*m_Stream, line) && !CanExtract(line)) {
            // intentionally blank
        }
    }
} // namespace Extractor