#include "FunctionReport.h"

#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <algorithm>

constexpr char LineFormat[]{"%s,%s,%s,%u,%s,%s,%s,%s,%s,%u\n"};
constexpr char LineHeading[]{"Function Name,Class Name,Visibility,Airity,Const,Static,Virtual,Abstract,Templated,Default Parameters"};

namespace TypeCounter {
    FunctionReport::ReportLine::ReportLine(Extractor::FunctionData data) : Data(data) {
        Data.Airity;
        Data.ClassName;
        Data.DefaultParameterCount;
        Data.FunctionName;
        Data.IsAbstract;
        Data.IsConst;
        Data.IsStatic;
        Data.IsTemplated;
        Data.IsVirtual;
        Data.Visibility;
    }

    std::string FunctionReport::ReportLine::ToString() const {
        return StrUtil::Format(LineFormat,
                               Data.FunctionName,
                               Data.ClassName,
                               Extractor::ToString(Data.Visibility),
                               Data.Airity,
                               Data.IsConst ? "true" : "false",
                               Data.IsStatic ? "true" : "false",
                               Data.IsVirtual ? "true" : "false",
                               Data.IsAbstract ? "true" : "false",
                               Data.IsTemplated ? "true" : "false",
                               Data.DefaultParameterCount);
    }

    FunctionReport::FunctionReport(const std::vector<Extractor::FileData>& files) {
        m_Lines.reserve(files.size());
        for(auto&& file: files) {
            for(auto func: file.FreeFunctions) {
                m_Lines.push_back(ReportLine(func));
            }
            for(auto type: file.Types) {
                for(auto func: type.Functions) {
                    m_Lines.push_back(ReportLine(func));
                }
            }
        }
    }

    void FunctionReport::PrintResultToStream(std::ostream& targetStream) const {
        targetStream << LineHeading << std::endl;
        for(auto&& line: m_Lines) {
            targetStream << line.ToString();
        }
    }
} // namespace TypeCounter