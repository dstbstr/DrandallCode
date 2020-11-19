#include "Extractor/NamespaceExtractor.h"

#include "Utilities/Require.h"
#include "Utilities/StringUtilities.h"

#include <regex>

namespace {
    std::regex NamespaceRegex("^namespace\\s*([\\w:]+)? *\\{?");
} // namespace

namespace Extractor {
    bool NamespaceExtractor::IsNamespace(const std::string& line) const {
        return std::regex_search(line, NamespaceRegex);
    }

    void NamespaceExtractor::ExtractNamespace(std::string& line) {
        std::smatch match;
        Require::True(std::regex_search(line, match, NamespaceRegex), "Expected line to match namespace regex.  Was 'IsNamespace' executed?");

        while(std::regex_search(line, match, NamespaceRegex)) {
            if(match[1].length() > 0) {
                m_Namespace.push_back(match[1]);
            } else {
                m_Namespace.push_back("Anonymous");
            }

            line = StrUtil::TrimStart(line.substr(match[0].length()));
        }
    }

    void NamespaceExtractor::PopNamespace() {
        Require::NotEmpty(m_Namespace, "Popped empty namespace");
        m_Namespace.pop_back();
    }

    std::string NamespaceExtractor::GetNamespace() const {
        return StrUtil::JoinVec(Separator.c_str(), m_Namespace);
    }
} // namespace Extractor