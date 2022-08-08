#pragma once
#include <string>
#include <iostream>

namespace Extractor {
    struct IncludeMap;
}

namespace Report {
    class DotReport {
    public:
        void WriteReport(const Extractor::IncludeMap& includeMap, std::string directory) const;
        void WriteReportToStream(const Extractor::IncludeMap& includeMap, std::ostream& targetStream) const;
    };
} // namespace Report
