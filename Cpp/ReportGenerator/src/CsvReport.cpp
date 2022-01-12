#include "Report/CsvReport.h"

#include "Report/Data/DetailedData.h"
#include "Report/Data/SimpleData.h"
#include "Utilities/Format.h"

#include <filesystem>
#include <fstream>
#include <memory>

namespace {
    struct CsvLine {
        virtual ~CsvLine() = default;

        virtual std::string ToString() const = 0;
    };

    class TypeAndFuncLine : public CsvLine {
    public:
        static std::string GenerateHeaderLine() {
            static const std::string format = "FilePath,FileName,Namespace,Category,SpecificType,LineCount,PreProcessorCount,"
                                              "ClassName,PublicMethodCount,ProtectedMethodCount,PrivateMethodCount,PublicDataCount,ProtectedDataCount,PrivateDataCount,InnerTypeCount,"
                                              "Visibility,FunctionName,Airity,DefaultParameterCount,"
                                              "HasBaseClass,IsTemplated,IsVirtual,IsInline,IsConst,IsExplicit,IsAbstract,IsStatic,IsFriend,IsDeleted,IsDefaulted\n";
            return format;
        }

        TypeAndFuncLine(const Report::DetailedData& data) : m_Data(&data) {}

        std::string ToString() const override {
            static const std::string format = "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n";

            return StrUtil::Format(format,
                                   m_Data->FilePath,
                                   m_Data->FileName,
                                   m_Data->Namespace,
                                   m_Data->Category,
                                   m_Data->SpecificType,
                                   m_Data->LineCount,
                                   m_Data->PreProcessorCount,
                                   m_Data->ClassName,
                                   m_Data->PublicMethodCount,
                                   m_Data->ProtectedMethodCount,
                                   m_Data->PrivateMethodCount,
                                   m_Data->PublicDataCount,
                                   m_Data->ProtectedDataCount,
                                   m_Data->PrivateDataCount,
                                   m_Data->InnerTypeCount,
                                   m_Data->Visibility,
                                   m_Data->FunctionName,
                                   m_Data->FunctionAirity,
                                   m_Data->DefaultParameterCount,
                                   m_Data->HasBaseClass,
                                   m_Data->IsTemplated,
                                   m_Data->IsVirtual,
                                   m_Data->IsInline,
                                   m_Data->IsConst,
                                   m_Data->IsExplicit,
                                   m_Data->IsAbstract,
                                   m_Data->IsStatic,
                                   m_Data->IsFriend,
                                   m_Data->IsDeleted,
                                   m_Data->IsDefaulted);
        }

        const Report::DetailedData* m_Data;
    };

    class FileLine : public CsvLine {
    public:
        static std::string GenerateHeaderLine() {
            static const std::string format = "FilePath,FileName,Direct Dependencies,Total Dependencies,Lines In File,Total Lines, Included By, Coupling Impact, Size Impact\n";
            return format;
        }

#pragma warning(suppress : 4702) // Don't know why it feels m_Data(&data) is unreachable code in release build...
        FileLine(const Report::SimpleData& data) : m_Data(&data) {}

        std::string ToString() const override {
            static const std::string format = "%s,%s,%s,%s,%s,%s,%s,%s,%s\n";

            return StrUtil::Format(format, m_Data->FilePath, m_Data->FileName, m_Data->DirectDependencies, m_Data->TotalDependencies, m_Data->LinesInFile, m_Data->TotalLines, m_Data->IncludedBy, m_Data->CouplingImpact, m_Data->SizeImpact);
        }

    private:
        const Report::SimpleData* m_Data;
    };

    class CsvLineFactory {
    public:
        CsvLineFactory(Extractor::FileData file, bool detailed) : m_FileName(file.FileName), m_FilePath(file.FilePath), m_Detailed(detailed) {}

        std::string GetHeaderString() {
            if(m_Detailed) {
                return TypeAndFuncLine::GenerateHeaderLine();
            } else {
                return FileLine::GenerateHeaderLine();
            }
        }

        template<typename T>
        std::string Create(const T data) const {
            if(m_Detailed) {
                return TypeAndFuncLine{Report::DetailedData::Create(data, m_FilePath, m_FileName)}.ToString();
            } else {
                return FileLine{Report::SimpleData::Create(data, m_FilePath, m_FileName)}.ToString();
            }
        }

    private:
        std::string m_FileName;
        std::string m_FilePath;
        bool m_Detailed;
    };

    bool IsDetailedReport(const std::vector<Extractor::FileData>& data) {
        if(data.size() == 0) {
            return false;
        }

        for(const auto& file: data) {
            if(file.Types.size() > 0) {
                return true;
            }
        }

        return false;
    }

    void PrintTypeData(std::ostream& targetStream, const CsvLineFactory& factory, const Extractor::TypeData& type) {
        targetStream << factory.Create(type);

        for(const auto& func: type.Functions) {
            targetStream << factory.Create(func);
        }
        for(const auto& opOverload: type.OperatorOverloads) {
            targetStream << factory.Create(opOverload);
        }
        for(const auto& specialFunc: type.SpecialFunctions) {
            targetStream << factory.Create(specialFunc);
        }
        for(const auto& innerType: type.InnerTypes) {
            PrintTypeData(targetStream, factory, innerType);
        }
    }
} // namespace

namespace Report {
    void CsvReport::WriteReport(std::string fileName) const {
        auto path = std::filesystem::path(fileName + ".csv");
        std::filesystem::remove(path); // clear out old results
        auto stream = std::ofstream(path);
        WriteReportToStream(stream);
    }

    void CsvReport::WriteReportToStream(std::ostream& targetStream) const {
        bool isDetailedReport = IsDetailedReport(m_FileData);
        bool headerPrinted = false;
        for(const auto& file: m_FileData) {
            auto factory = CsvLineFactory{file, isDetailedReport};
            if(!headerPrinted) {
                targetStream << factory.GetHeaderString();
                headerPrinted = true;
            }
            targetStream << factory.Create(file);
            for(const auto& freeFunc: file.FreeFunctions) {
                targetStream << factory.Create(freeFunc);
            }
            for(const auto& freeOp: file.FreeOperatorOverloads) {
                targetStream << factory.Create(freeOp);
            }
            for(const auto& type: file.Types) {
                PrintTypeData(targetStream, factory, type);
            }
        }
    }
} // namespace Report