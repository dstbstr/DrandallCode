#include "Report/Data/DetailedData.h"

#include "Extractor/Data/FileData.h"
#include "Report/Data/TypeWithMethodCounts.h"

#include <string>

namespace {

    std::string BoolToString(bool val) {
        return val ? "X" : "";
    }
} // namespace

namespace Report {
    DetailedData DetailedData::Create(const Extractor::FileData& data, std::string filePath, std::string fileName) {
        DetailedData result{};
        result.FileName = fileName;
        result.FilePath = filePath;
        result.LineCount = std::to_string(data.LineCount);
        result.PreProcessorCount = std::to_string(data.PreProcessorCount);

        return result;
    }

    DetailedData DetailedData::Create(const Extractor::TypeData& type, std::string filePath, std::string fileName) {
        DetailedData result{};
        TypeWithMethodCounts data{type};

        result.FileName = fileName;
        result.FilePath = filePath;
        result.Category = "UserDefinedType";
        result.SpecificType = Extractor::ToString(data.TypeKind);
        result.LineCount = std::to_string(data.LineCount);
        result.Namespace = data.Namespace;
        result.ClassName = data.ClassName;

        result.HasBaseClass = BoolToString(data.HasBaseClass);
        result.InnerTypeCount = std::to_string(data.InnerTypes.size());
        result.IsTemplated = BoolToString(data.IsTemplated);
        result.PublicDataCount = std::to_string(data.PublicDataMemberCount);
        result.ProtectedDataCount = std::to_string(data.ProtectedDataMemberCount);
        result.PrivateDataCount = std::to_string(data.PrivateDataMemberCount);
        result.PublicMethodCount = std::to_string(data.PublicMethodCount);
        result.ProtectedMethodCount = std::to_string(data.ProtectedMethodCount);
        result.PrivateMethodCount = std::to_string(data.PrivateMethodCount);

        return result;
    }

    DetailedData DetailedData::Create(const Extractor::FunctionData& data, std::string filePath, std::string fileName) {
        DetailedData result{};

        result.FileName = fileName;
        result.FilePath = filePath;
        result.Category = "Function";
        result.SpecificType = "Normal";
        result.LineCount = std::to_string(data.LineCount);
        result.Namespace = data.Namespace;
        result.ClassName = data.ClassName;
        result.Visibility = Extractor::ToString(data.Visibility);

        result.FunctionAirity = std::to_string(data.Airity);
        result.DefaultParameterCount = data.DefaultParameterCount;
        result.FunctionName = data.FunctionName;
        result.IsAbstract = BoolToString(data.IsAbstract);
        result.IsConst = BoolToString(data.IsConst);
        result.IsInline = BoolToString(data.IsInline);
        result.IsStatic = BoolToString(data.IsStatic);
        result.IsTemplated = BoolToString(data.IsTemplated);
        result.IsVirtual = BoolToString(data.IsVirtual);

        return result;
    }

    DetailedData DetailedData::Create(const Extractor::SpecialFunctionData& data, std::string filePath, std::string fileName) {
        DetailedData result{};
        result.FileName = fileName;
        result.FilePath = filePath;
        result.Category = "Special Function";
        result.SpecificType = Extractor::ToString(data.Kind);
        result.LineCount = std::to_string(data.LineCount);
        result.Namespace = data.Namespace;
        result.ClassName = data.ClassName;
        result.Visibility = Extractor::ToString(data.Visibility);

        result.FunctionAirity = std::to_string(data.Airity);
        result.DefaultParameterCount = std::to_string(data.DefaultParameterCount);
        result.IsDefaulted = BoolToString(data.IsDefaulted);
        result.IsDeleted = BoolToString(data.IsDeleted);
        result.IsExplicit = BoolToString(data.IsExplicit);
        result.IsVirtual = BoolToString(data.IsVirtual);

        return result;
    }

    DetailedData DetailedData::Create(const Extractor::OperatorOverloadData& data, std::string filePath, std::string fileName) {
        DetailedData result{};
        result.FileName = fileName;
        result.FilePath = filePath;
        result.Category = "Operator Overload";
        result.SpecificType = data.Operator;
        result.LineCount = std::to_string(data.LineCount);
        result.Namespace = data.Namespace;
        result.ClassName = data.ClassName;
        result.Visibility = Extractor::ToString(data.Visibility);

        result.FunctionAirity = std::to_string(data.Airity);
        result.IsExplicit = BoolToString(data.IsExplicit);
        result.IsFriend = BoolToString(data.IsFriend);
        result.IsInline = BoolToString(data.IsInline);

        return result;
    }
}; // namespace Report
