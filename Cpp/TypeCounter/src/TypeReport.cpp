#include "TypeReport.h"

#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <algorithm>

constexpr char LineFormat[]{"%s,%s,%s,%u,%u,%u,%u,%u,%u,%u\n"};
constexpr char LineHeading[]{
    "File Name,Type Name,Type,Public Data,Protected Data,Private Data,Public Methods,Protected Methods,Private Methods,Inner Types"};

namespace TypeCounter {
    TypeReport::ReportLine::ReportLine(Extractor::TypeData data) : Data(data) {
        using namespace Extractor;

        for(auto&& method: data.Functions)
            switch(method.Visibility) {
            case PUBLIC: PublicMethodCount++; break;
            case PROTECTED: ProtectedMethodCount++; break;
            case PRIVATE: PrivateMethodCount++; break;
            default: break;
            }
    }

    std::string TypeReport::ReportLine::ToString() const {
        return StrUtil::Format(LineFormat,
                               Data.FileName,
                               Data.ClassName,
                               Extractor::ToString(Data.TypeKind),
                               Data.PublicDataMemberCount,
                               Data.ProtectedDataMemberCount,
                               Data.PrivateDataMemberCount,
                               PublicMethodCount,
                               ProtectedMethodCount,
                               PrivateMethodCount,
                               Data.InnerTypes.size());
    }

    TypeReport::TypeReport(const std::vector<Extractor::FileData>& files) {
        m_Lines.reserve(files.size());
        for(auto&& file: files) {
            for(auto type: file.Types) {
                m_Lines.push_back(ReportLine(type));
            }
        }
    }

    void TypeReport::PrintResultToStream(std::ostream& targetStream) const {
        targetStream << LineHeading << std::endl;
        for(auto&& line: m_Lines) {
            targetStream << line.ToString();
        }
    }
} // namespace TypeCounter