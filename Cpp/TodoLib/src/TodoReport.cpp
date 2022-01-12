#include "Todo/TodoReport.h"

#include "Platform/Types.h"
#include "Utilities/StringUtils.h"

#include <OpenXLSX.hpp>

namespace {
    void WriteHeader(OpenXLSX::XLWorksheet& sheet) {
        sheet.row(1).values() = std::vector<std::string>{"FilePath", "FileName", "Line Number", "Classification", "Status", "Notes", "RelevantLine"};
    }

    void AppendTodo(OpenXLSX::XLWorksheet& sheet, const Todo& todo) {
        static u32 rowNum = 2;
        std::vector<OpenXLSX::XLCellValue> rowValues;

        rowValues.emplace_back(todo.FilePath);
        rowValues.emplace_back(todo.FileName);
        rowValues.emplace_back(todo.LineNumber);
        rowValues.emplace_back(todo.Classification);
        rowValues.emplace_back(todo.Status);
        rowValues.emplace_back(todo.Notes);
        rowValues.emplace_back(todo.RelevantLine);

        sheet.row(rowNum++).values() = rowValues;
    }
} // namespace

namespace TodoReportGenerator {
    void WriteToExcel(const std::vector<Todo>& todos, std::string fileName) {
        using namespace OpenXLSX;

        XLDocument doc;
        if(StrUtil::EndsWith(fileName, ".xlsx")) {
            doc.create(fileName);
        } else {
            doc.create(fileName + ".xlsx");
        }

        auto wb = doc.workbook();
        auto sheet = wb.worksheet("Sheet1");
        sheet.setName("Todos");

        WriteHeader(sheet);
        for(const auto& todo: todos) {
            AppendTodo(sheet, todo);
        }

        doc.save();
        doc.close();
    }
}; // namespace TodoReportGenerator
