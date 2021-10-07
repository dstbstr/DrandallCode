#include "Report/ExcelReport.h"

#include "Report/Data/TypeWithMethodCounts.h"
#include "Utilities/StringUtils.h"

#include <OpenXLSX.hpp>

namespace {
    using namespace OpenXLSX;

    bool IsDetailed(const std::vector<Extractor::FileData>& files) {
        for(const auto& file: files) {
            if(file.Types.size() > 0) {
                return true;
            }
            if(file.FreeFunctions.size() > 0) {
                return true;
            }
            if(file.FreeOperatorOverloads.size() > 0) {
                return true;
            }
        }

        return false;
    }
    static const std::vector<std::string> FunctionHeaders =
        {"FilePath", "FileName", "Line Count", "Namespace", "Class Name", "Function Name", "Visibility", "Airity", "Default Parameters", "Abstract", "Const", "Explicit", "Inline", "Static", "Templated", "Virtual", "Friend"};
    static bool FunctionHeaderAdded = false;
    static u32 CurrentFunctionRow = 2;

    void AppendIncludeData(XLWorksheet& includeSheet, const Extractor::FileData data) {
        const std::vector<std::string> headers = {"File Path", "File Name", "Direct Dependencies", "Total Dependencies", "Lines In File", "Total Lines", "Included By", "Coupling Impact", "Size Impact"};
        static bool headerAdded = false;
        if(!headerAdded) {
            includeSheet.row(1).values() = headers;
            headerAdded = true;
        }

        static u32 currentRow = 2;

        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(data.FilePath.c_str());
        rowValues.emplace_back(data.FileName.c_str());
        rowValues.emplace_back(data.IncludeFiles.size());
        rowValues.emplace_back(data.TotalIncludeCount);
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.TotalLineCount);
        rowValues.emplace_back(data.IncludedByCount);
        rowValues.emplace_back((data.TotalIncludeCount * data.IncludedByCount));
        rowValues.emplace_back((data.TotalLineCount * data.IncludedByCount));

        includeSheet.row(currentRow++).values() = rowValues;
    }

    void AppendFileData(XLWorksheet& fileSheet, const Extractor::FileData data) {
        const std::vector<std::string> headers = {"FilePath", "FileName", "LineCount", "PreProcessorCount"};
        static bool headerAdded = false;
        if(!headerAdded) {
            fileSheet.row(1).values() = headers;
            headerAdded = true;
        }

        static u32 currentRow = 2;
        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(data.FilePath.c_str());
        rowValues.emplace_back(data.FileName.c_str());
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.PreProcessorCount);

        fileSheet.row(currentRow++).values() = rowValues;
    }

    void AppendTypeData(XLWorksheet& typeSheet, const Extractor::TypeData& type, const std::string& filePath) {
        Report::TypeWithMethodCounts data{type};

        static bool headerAdded = false;
        const std::vector<std::string> headers = {
            "FilePath", "FileName", "Specific Type", "Line Count", "Namespace", "Name", "Inner Types", "Public Data", "Protected Data", "Private Data", "Public Methods", "Protected Methods", "Private Methods", "Is Templated", "Has Base Class"};

        if(!headerAdded) {
            typeSheet.row(1).values() = headers;
            headerAdded = true;
        }

        static u32 currentRow = 2;
        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(filePath.c_str());
        rowValues.emplace_back(data.FileName.c_str());
        rowValues.emplace_back(ToString(data.TypeKind).c_str());
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.Namespace.c_str());
        rowValues.emplace_back(data.ClassName.c_str());
        rowValues.emplace_back(data.InnerTypes.size());
        rowValues.emplace_back(data.PublicDataMemberCount);
        rowValues.emplace_back(data.ProtectedDataMemberCount);
        rowValues.emplace_back(data.PrivateDataMemberCount);
        rowValues.emplace_back(data.PublicMethodCount);
        rowValues.emplace_back(data.ProtectedMethodCount);
        rowValues.emplace_back(data.PrivateMethodCount);
        rowValues.emplace_back(data.IsTemplated);
        rowValues.emplace_back(data.HasBaseClass);

        typeSheet.row(currentRow++).values() = rowValues;
    }

    void AppendFunctionData(XLWorksheet& functionSheet, const Extractor::FunctionData& data, const std::string& filePath, const std::string& fileName) {
        if(!FunctionHeaderAdded) {
            functionSheet.row(1).values() = FunctionHeaders;
            FunctionHeaderAdded = true;
        }

        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(filePath.c_str());
        rowValues.emplace_back(fileName.c_str());
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.Namespace.c_str());
        rowValues.emplace_back(data.ClassName.c_str());
        rowValues.emplace_back(data.FunctionName.c_str());
        rowValues.emplace_back(ToString(data.Visibility).c_str());
        rowValues.emplace_back(data.Airity);
        rowValues.emplace_back(data.DefaultParameterCount);
        rowValues.emplace_back(data.IsAbstract);
        rowValues.emplace_back(data.IsConst);
        rowValues.emplace_back("");
        rowValues.emplace_back(data.IsInline);
        rowValues.emplace_back(data.IsStatic);
        rowValues.emplace_back(data.IsTemplated);
        rowValues.emplace_back(data.IsVirtual);
        rowValues.emplace_back("");

        functionSheet.row(CurrentFunctionRow++).values() = rowValues;
    }

    void AppendFunctionData(XLWorksheet& functionSheet, const Extractor::SpecialFunctionData& data, const std::string& filePath, const std::string& fileName) {
        if(!FunctionHeaderAdded) {
            functionSheet.row(1).values() = FunctionHeaders;
            FunctionHeaderAdded = true;
        }

        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(filePath.c_str());
        rowValues.emplace_back(fileName.c_str());
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.Namespace.c_str());
        rowValues.emplace_back(data.ClassName.c_str());
        rowValues.emplace_back(ToString(data.Kind).c_str());
        rowValues.emplace_back(ToString(data.Visibility).c_str());
        rowValues.emplace_back(data.Airity);
        rowValues.emplace_back(data.DefaultParameterCount);
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back(data.IsExplicit);
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back(data.IsVirtual);
        rowValues.emplace_back("");

        functionSheet.row(CurrentFunctionRow++).values() = rowValues;
    }

    void AppendFunctionData(XLWorksheet& functionSheet, const Extractor::OperatorOverloadData& data, const std::string& filePath, const std::string& fileName) {
        if(!FunctionHeaderAdded) {
            functionSheet.row(1).values() = FunctionHeaders;
            FunctionHeaderAdded = true;
        }

        std::vector<XLCellValue> rowValues;

        rowValues.emplace_back(filePath.c_str());
        rowValues.emplace_back(fileName.c_str());
        rowValues.emplace_back(data.LineCount);
        rowValues.emplace_back(data.Namespace.c_str());
        rowValues.emplace_back(data.ClassName.c_str());
        rowValues.emplace_back(data.Operator.c_str());
        rowValues.emplace_back(ToString(data.Visibility).c_str());
        rowValues.emplace_back(data.Airity);
        rowValues.emplace_back(0);
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back(data.IsExplicit);
        rowValues.emplace_back(data.IsInline);
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back("");
        rowValues.emplace_back(data.IsFriend);

        functionSheet.row(CurrentFunctionRow++).values() = rowValues;
    }

    void AppendTypeData(XLWorksheet& typeSheet, XLWorksheet& functionSheet, const Extractor::TypeData& type, const std::string filePath) {
        AppendTypeData(typeSheet, type, filePath);

        for(const auto& func: type.Functions) {
            AppendFunctionData(functionSheet, func, filePath, type.FileName);
        }
        for(const auto& opOverload: type.OperatorOverloads) {
            AppendFunctionData(functionSheet, opOverload, filePath, type.FileName);
        }
        for(const auto& specialFunc: type.SpecialFunctions) {
            AppendFunctionData(functionSheet, specialFunc, filePath, type.FileName);
        }
        for(const auto& innerType: type.InnerTypes) {
            AppendTypeData(typeSheet, functionSheet, innerType, filePath);
        }
    }

    OpenXLSX::XLWorksheet AddWorksheet(OpenXLSX::XLWorkbook wb, std::string name) {
        wb.addWorksheet(name);
        return wb.worksheet(name);
    }
} // namespace
namespace Report {
    void ExcelReport::WriteReport(std::string fileName) const {
        using namespace OpenXLSX;

        XLDocument doc;
        if(StrUtil::EndsWith(fileName, ".xlsx")) {
            doc.create(fileName);
        } else {
            doc.create(fileName + ".xlsx");
        }

        auto wb = doc.workbook();
        auto includes = wb.worksheet("Sheet1");
        includes.setName("Includes");
        auto files = AddWorksheet(wb, "Files");
        std::unique_ptr<XLWorksheet> types = nullptr;
        std::unique_ptr<XLWorksheet> functions = nullptr;
        if(IsDetailed(m_FileData)) {
            types = std::make_unique<XLWorksheet>(AddWorksheet(wb, "Types"));
            functions = std::make_unique<XLWorksheet>(AddWorksheet(wb, "Functions"));
        }

        for(const auto& file: m_FileData) {
            AppendIncludeData(includes, file);
            AppendFileData(files, file);

            for(const auto& freeFunc: file.FreeFunctions) {
                AppendFunctionData(*functions, freeFunc, file.FilePath, file.FileName);
            }
            for(const auto& freeOp: file.FreeOperatorOverloads) {
                AppendFunctionData(*functions, freeOp, file.FilePath, file.FileName);
            }
            for(const auto& type: file.Types) {
                AppendTypeData(*types, *functions, type, file.FilePath);
            }
        }

        doc.save();
        doc.close();
    }
} // namespace Report