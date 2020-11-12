#include "IncludeReport.h"

#include "Extractor/Data/FileData.h"
#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <algorithm>

constexpr char LineFormat[]{"%11u | %10u | %5u | %11u | %s\n"};
constexpr char LineHeading[]{"Direct Deps | Total Deps | Lines | Total Lines | File"};

namespace IncludeCounter {
    IncludeReport::SortOrder IncludeReport::GetSortOrder(u8 index) {
        Require::True(index < SortOrder::_COUNT, StrUtil::Format("Invalid index %u.  Values are between 0 and %u", index, SortOrder::_COUNT - 1));
        return static_cast<SortOrder>(index);
    }

    std::vector<std::string> IncludeReport::GetSortOrderStrings() {
        return {StrUtil::Format("%u FileName", SortOrder::FILE_NAME),
                StrUtil::Format("%u Direct Dependencies", SortOrder::DIRECT_DEPENDENCIES),
                StrUtil::Format("%u Total Dependencies", SortOrder::TOTAL_DEPENDENCIES),
                StrUtil::Format("%u Included By", SortOrder::BLAST_RADIUS)};
    }

    void IncludeReport::PrintResultToStream(std::ostream& targetStream, SortOrder sortBy, bool descending) const {
        using Extractor::FileData;

        std::vector<Extractor::FileData> working = m_Results; // make a copy, don't change the original
        switch(sortBy) {
        case SortOrder::FILE_NAME:
            std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.FileName.compare(rhs.FileName) < 0; });
            break;
        case SortOrder::TOTAL_DEPENDENCIES:
            std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.TotalIncludeCount < rhs.TotalIncludeCount; });
            break;
        case SortOrder::DIRECT_DEPENDENCIES:
            std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.IncludeFiles.size() < rhs.IncludeFiles.size(); });
            break;
        case SortOrder::BLAST_RADIUS:
            std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.IncludedByCount < rhs.IncludedByCount; });
            break;
        default: ASSERT_FAIL("Unexpected sortBy"); break;
        }

        if(descending) {
            std::reverse(working.begin(), working.end());
        }
        targetStream << LineHeading << std::endl;
        for(auto&& row: working) {
            targetStream << StrUtil::Format(
                LineFormat, row.IncludeFiles.size(), row.TotalIncludeCount, row.LineCount, row.TotalLineCount, row.FilePath);
        }
    }
} // namespace IncludeCounter