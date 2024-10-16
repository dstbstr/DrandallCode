#pragma once

#include <string>

namespace Extractor {
    struct FileData;
    struct TypeData;
    struct FunctionData;
    struct SpecialFunctionData;
    struct OperatorOverloadData;
} // namespace Extractor

namespace Report {
    struct DetailedData {
    private:
        DetailedData() = default;

    public:
        static DetailedData Create(const Extractor::FileData& data, std::string filePath, std::string fileName);
        static DetailedData Create(const Extractor::TypeData& type, std::string filePath, std::string fileName);
        static DetailedData Create(const Extractor::FunctionData& data, std::string filePath, std::string fileName);
        static DetailedData Create(const Extractor::SpecialFunctionData& data, std::string filePath, std::string fileName);
        static DetailedData Create(const Extractor::OperatorOverloadData& data, std::string filePath, std::string fileName);

        std::string FilePath;
        std::string FileName;
        std::string Namespace;
        std::string Category;
        std::string SpecificType;
        std::string LineCount;
        std::string PreProcessorCount;
        std::string ClassName;
        std::string PublicMethodCount;
        std::string ProtectedMethodCount;
        std::string PrivateMethodCount;
        std::string PublicDataCount;
        std::string ProtectedDataCount;
        std::string PrivateDataCount;
        std::string InnerTypeCount;
        std::string Visibility;
        std::string FunctionName;
        std::string FunctionAirity;
        std::string DefaultParameterCount;
        std::string HasBaseClass;
        std::string IsTemplated;
        std::string IsVirtual;
        std::string IsInline;
        std::string IsConst;
        std::string IsExplicit;
        std::string IsAbstract;
        std::string IsStatic;
        std::string IsFriend;
        std::string IsDeleted;
        std::string IsDefaulted;
    };
} // namespace Report