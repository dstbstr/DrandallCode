#pragma once

#include "Core/Platform/Types.h"

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

namespace Extractor {
    struct DefineData;

    class IfDefExtractor {
    public:
        IfDefExtractor(const DefineData& knownDefines, std::istream& stream) : m_KnownDefines(&knownDefines), m_Stream(&stream) {}
        IfDefExtractor(const IfDefExtractor& other) = delete;
        IfDefExtractor operator=(const IfDefExtractor& other) = delete;

        bool CanExtract(const std::string& line) const;
        void Extract(std::string& line);

    private:
        const DefineData* m_KnownDefines;
        std::set<u8> m_MatchedLevels{};
        u8 m_DefineDepth{0};
        std::istream* m_Stream{nullptr};

        void SkipBody(std::string& line);
        void SkipToEndif();
    };
} // namespace Extractor