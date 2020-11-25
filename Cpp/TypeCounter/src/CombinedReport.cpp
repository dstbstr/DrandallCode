#include "CombinedReport.h"

#include "Utilities/Format.h"

namespace {

    constexpr char FileLineFormat[]{"%s,%s,,%s,,%u,,,,,,,,,,,,,,,,,,,,\n"};
    constexpr char TypeLineFormat[]{",%s,%s,%s,%s,%u,"
                                    "%s,%u,%u,%u,%u,%u,%u,%u"
                                    ",,,,"
                                    "%s,%s,,,,,,,,,\n"};
    constexpr char FunctionLineFormat[]{",%s,%s,%s,%s,%u,"
                                        "%s,,,,,,,,"
                                        "%s,%s,%u,%u,"
                                        ",%s,%s,%s,%s,%s,%s,,,\n"};
    constexpr char SpecialFunctionFormat[]{",%s,%s,%s,%s,%u,"
                                           "%s,,,,,,,,"
                                           "%s,,%u,%u,"
                                           ",,%s,,,%s,,,%s,%s\n"};
    constexpr char OpOverloadFormat[]{",%s,%s,%s,%s,%u,"
                                      "%s,,,,,,,,"
                                      "%s,,%u,,"
                                      ",,,%s,,%s,,,%s,,\n"};

    constexpr char LineHeading[]{"FilePath,FileName,Namespace,Category,SpecificType,LineCount,"
                                 "ClassName,PublicMethodCount,ProtectedMethodCount,PrivateMethodCount,PublicDataCount,ProtectedDataCount,PrivateDataCount,InnerTypeCount,"
                                 "Visibility,FunctionName,Airity,DefaultParameterCount,"
                                 "HasBaseClass,IsTemplated,IsVirtual,IsInline,IsConst,IsExplicit,IsAbstract,IsStatic,IsFriend,IsDeleted,IsDefaulted"};

    std::string PrintBool(bool val) {
        return val ? "x" : "";
    }

    std::string PrintFileLine(Extractor::FileData data) {
        return StrUtil::Format(FileLineFormat, data.FilePath, data.FileName, "File", data.LineCount);
    }
    std::string PrintTypeLine(Extractor::TypeData data) {
        using namespace Extractor;
        u32 publicMethodCount = 0;
        u32 protectedMethodCount = 0;
        u32 privateMethodCount = 0;
        for(auto&& func: data.Functions) {
            switch(func.Visibility) {
            case PUBLIC: publicMethodCount++; break;
            case PROTECTED: protectedMethodCount++; break;
            case PRIVATE: privateMethodCount++; break;
            default: break;
            }
        }
        return StrUtil::Format(TypeLineFormat,
                               data.FileName,
                               data.Namespace,
                               "Class",
                               ToString(data.TypeKind),
                               data.LineCount,
                               data.ClassName,
                               publicMethodCount,
                               protectedMethodCount,
                               privateMethodCount,
                               data.PublicDataMemberCount,
                               data.ProtectedDataMemberCount,
                               data.PrivateDataMemberCount,
                               data.InnerTypes.size(),
                               PrintBool(data.HasBaseClass),
                               PrintBool(data.IsTemplated));
    }

    std::string PrintFunctionLine(std::string fileName, Extractor::FunctionData data) {
        return StrUtil::Format(FunctionLineFormat,
                               fileName,
                               data.Namespace,
                               "Function",
                               "Normal",
                               data.LineCount,
                               data.ClassName,
                               ToString(data.Visibility),
                               data.FunctionName,
                               data.Airity,
                               data.DefaultParameterCount,
                               PrintBool(data.IsTemplated),
                               PrintBool(data.IsVirtual),
                               PrintBool(data.IsInline),
                               PrintBool(data.IsConst),
                               PrintBool(data.IsAbstract),
                               PrintBool(data.IsStatic));
    }

    std::string PrintSpecialFunctionLine(std::string fileName, Extractor::SpecialFunctionData data) {
        return StrUtil::Format(SpecialFunctionFormat,
                               fileName,
                               data.Namespace,
                               "SpecialFunction",
                               ToString(data.Kind),
                               data.LineCount,
                               data.ClassName,
                               ToString(data.Visibility),
                               data.Airity,
                               data.DefaultParameterCount,
                               PrintBool(data.IsVirtual),
                               PrintBool(data.IsExplicit),
                               PrintBool(data.IsDeleted),
                               PrintBool(data.IsDefaulted));
    }

    std::string PrintOpOverloadLine(std::string fileName, Extractor::OperatorOverloadData data) {
        return StrUtil::Format(
            OpOverloadFormat, fileName, data.Namespace, "Operator Overload", data.Operator, data.LineCount, data.ClassName, ToString(data.Visibility), data.Airity, PrintBool(data.IsInline), PrintBool(data.IsExplicit), PrintBool(data.IsFriend));
    }

} // namespace
namespace TypeCounter {
    CombinedReport::CombinedReport(const std::vector<Extractor::FileData>& files) : m_Files(files) {}

    void CombinedReport::PrintTypeData(std::ostream& targetStream, Extractor::TypeData type) const {
        auto fileName = type.FileName;
        targetStream << PrintTypeLine(type);

        for(auto func: type.Functions) {
            targetStream << PrintFunctionLine(fileName, func);
        }
        for(auto opOverload: type.OperatorOverloads) {
            targetStream << PrintOpOverloadLine(fileName, opOverload);
        }
        for(auto specialFunction: type.SpecialFunctions) {
            targetStream << PrintSpecialFunctionLine(fileName, specialFunction);
        }
        for(auto innerType: type.InnerTypes) {
            PrintTypeData(targetStream, innerType);
        }
    }

    void CombinedReport::PrintResultToStream(std::ostream& targetStream) const {
        targetStream << LineHeading << std::endl;

        for(auto&& file: m_Files) {
            auto fileName = file.FileName;
            targetStream << PrintFileLine(file);
            for(auto freeFunction: file.FreeFunctions) {
                targetStream << PrintFunctionLine(fileName, freeFunction);
            }
            for(auto freeOperator: file.FreeOperatorOverloads) {
                targetStream << PrintOpOverloadLine(fileName, freeOperator);
            }
            for(auto type: file.Types) {
                PrintTypeData(targetStream, type);
            }
        }
    }
} // namespace TypeCounter