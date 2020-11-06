#include "ResultGenerator.h"

#include "FileData.h"
#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"

#include <algorithm>

constexpr char LineFormat[]{"%32s | %21u | %5u"};
constexpr char LineHeading[]{"           File Name             |  Total Include Count  |  Included By Count "};

void ResultGenerator::PrintResultToStream(std::ostream& targetStream, SortOrder sortBy, bool descending) const {
    std::vector<FileData> working = m_Results; // make a copy, don't change the original
    switch(sortBy) {
    case SortOrder::FILE_NAME:
        std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.FileName.compare(rhs.FileName); });
        break;
    case SortOrder::DEPENDENCIES:
        std::sort(working.begin(), working.end(), [](FileData lhs, FileData rhs) { return lhs.TotalIncludeCount < rhs.TotalIncludeCount; });
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
        targetStream << StrUtil::Format(LineFormat, row.FileName, row.TotalIncludeCount, row.IncludedByCount) << std::endl;
    }
}