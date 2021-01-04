#ifndef __IFDEFEXTRACTOR_H__
#define __IFDEFEXTRACTOR_H__

#include "Platform/Types.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace Extractor {
    class IfDefExtractor {
    public:
        IfDefExtractor(std::vector<std::string> knownDefines, std::istream& stream) : m_KnownDefines(knownDefines), m_Stream(&stream) {}
        IfDefExtractor(const IfDefExtractor& other) = delete;
        IfDefExtractor operator=(const IfDefExtractor& other) = delete;

        bool CanExtract(const std::string& line) const;
        void Extract(std::string& line);
        void AddDefine(std::string define) {
            m_KnownDefines.push_back(define);
        }

    private:
        std::vector<std::string> m_KnownDefines;
        // u32 m_MatchedIfDef{0};
        std::set<u8> m_MatchedLevels{};
        u32 m_DefineDepth{0};
        std::istream* m_Stream{nullptr};

        void SkipBody(std::string& line);
        void SkipToEndif();
    };
} // namespace Extractor
#endif // __IFDEFEXTRACTOR_H__