#ifndef __NAMESPACEEXTRACTOR_H__
#define __NAMESPACEEXTRACTOR_H__

#include "Platform/Types.h"

#include <string>
#include <vector>


namespace Extractor {
    class NamespaceExtractor {
    public:
        const std::string Separator = "::";

        NamespaceExtractor() = default;
        NamespaceExtractor(const NamespaceExtractor&) = delete;
        NamespaceExtractor& operator=(const NamespaceExtractor&) = delete;

        bool IsNamespace(const std::string& line) const;
        void ExtractNamespace(std::string& line);
        void PushNestedCurly();
        void PopNamespace();

        std::string GetNamespace() const;

    private:
        std::vector<std::string> m_Namespace;
        u8 m_NestedCurlyCount{0};
    };
} // namespace Extractor
#endif // __NAMESPACEEXTRACTOR_H__