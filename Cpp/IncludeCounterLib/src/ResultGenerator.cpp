#include "IncludeCounter/ResultGenerator.h"

#include "IncludeCounter/Data/FileData.h"
#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <algorithm>

constexpr char LineFormat[]{"%32s | %19u | %20u | %5u"};
constexpr char LineHeading[]{"           File Name             | Direct Dependencies |  Total Dependencies  |  Included By Count "};

namespace IncludeCounter {
    ResultGenerator::SortOrder ResultGenerator::GetSortOrder(u8 index) {
        Require::True(index < SortOrder::_COUNT, StrUtil::Format("Invalid index %u.  Values are between 0 and %u", index, SortOrder::_COUNT - 1));
        return static_cast<SortOrder>(index);
    }

    std::vector<std::string> ResultGenerator::GetSortOrderStrings() {
        return {StrUtil::Format("%u FileName", SortOrder::FILE_NAME),
                StrUtil::Format("%u Direct Dependencies", SortOrder::DIRECT_DEPENDENCIES),
                StrUtil::Format("%u Total Dependencies", SortOrder::TOTAL_DEPENDENCIES),
                StrUtil::Format("%u Included By", SortOrder::BLAST_RADIUS)};
    }

    void ResultGenerator::PrintResultToStream(std::ostream& targetStream, SortOrder sortBy, bool descending) const {
        std::vector<FileData> working = m_Results; // make a copy, don't change the original
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
            targetStream << StrUtil::Format(LineFormat, row.FileName, row.IncludeFiles.size(), row.TotalIncludeCount, row.IncludedByCount)
                         << std::endl;
        }
    }
} // namespace IncludeCounterLib