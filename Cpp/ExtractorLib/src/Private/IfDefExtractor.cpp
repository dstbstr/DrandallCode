#include "Extractor/Private/IfDefExtractor.h"

#include "Extractor/Private/LineFetcher.h"
#include "Instrumentation/Log.h"
#include "Utilities/ExpressionEvaluator.h"
#include "Utilities/Format.h"
#include "Utilities/StringUtils.h"

#include <regex>

static const std::string EndIf = "#endif";
static const std::string ElseStr = "#else";
static const std::regex DefineRegex("defined\\(([^\\)]+)\\)");
static const std::regex IfdefRegex("^#(?:el)?if(?:def)?");
static const std::regex IfndefRegex("^#ifndef");
static const std::regex ExtractRegex("^#(?:el|end)?(?:if|se)n?(?:def)?");

namespace Extractor {
    bool IfDefExtractor::CanExtract(const std::string& line) const {
        return std::regex_search(line, ExtractRegex);
    }

    void IfDefExtractor::Extract(std::string& line) {
        if(line == EndIf) {
            m_MatchedLevels.erase(m_DefineDepth);
            m_DefineDepth--;
            if(m_DefineDepth < 0) {
                LOG_WARN("Found unexpected endif");
                m_DefineDepth = 0;
            }
            line = "";
            return;
        }

        if(StrUtil::StartsWith(line, "#if")) {
            m_DefineDepth++;
        }

        if(m_MatchedLevels.find(m_DefineDepth) != m_MatchedLevels.end()) {
            SkipBody(line);
            return;
        }

        if(line == ElseStr) {
            m_MatchedLevels.insert(m_DefineDepth);
            line = "";
            return;
        }

        line = std::regex_replace(line, DefineRegex, "$1"); // turn #if defined(X) into X
        line = std::regex_replace(line, IfndefRegex, "!"); // turn #ifndef X into !X
        line = std::regex_replace(line, IfdefRegex, ""); // turn #ifdef X into X and #elif X into X

        bool matches = ExpressionEvaluator::Evaluate(line, [&](std::string str) -> bool {
            if(str.find("==") != str.npos) {
                auto split = StrUtil::Split(str, "==");
                auto key = std::string(split[0]);
                if(m_KnownDefines->find(key) == m_KnownDefines->end()) {
                    return false;
                }
                return m_KnownDefines->at(key).compare(split[1]) == 0;
            } else if(str.find("!=") != str.npos) {
                auto split = StrUtil::Split(str, "!=");
                auto key = std::string(split[0]);
                if(m_KnownDefines->find(key) == m_KnownDefines->end()) {
                    return true;
                }

                return m_KnownDefines->at(key).compare(split[1]) != 0;
            }

            return m_KnownDefines->find(str) != m_KnownDefines->end();
        });
        if(matches) {
            m_MatchedLevels.insert(m_DefineDepth);
            line = "";
        } else {
            SkipBody(line);
            return;
        }
    }

    // private
    void IfDefExtractor::SkipBody(std::string& line) {
        while(LineFetcher::GetNextLine(*m_Stream, line)) {
            if(CanExtract(line)) {
                if(StrUtil::StartsWith(line, "#if")) {
                    SkipToEndif();
                } else {
                    Extract(line);
                    break;
                }
            }
        }
    }

    void IfDefExtractor::SkipToEndif() {
        std::string line;
        while(LineFetcher::GetNextLine(*m_Stream, line)) {
            if(StrUtil::StartsWith(line, "#if")) {
                SkipToEndif();
            } else if(line == EndIf) {
                return;
            }
        }
    }

} // namespace Extractor