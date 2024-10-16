#pragma once

#include <stdexcept>
#include <string>

namespace Extractor {
    struct FileData;
    struct TypeData;
    struct FunctionData;
    struct SpecialFunctionData;
    struct OperatorOverloadData;
} // namespace Extractor

namespace Report {
    struct SimpleData {
    private:
        SimpleData() = default;

    public:
        static SimpleData Create(const Extractor::FileData& data, std::string filePath, std::string fileName);

        static SimpleData Create(const Extractor::TypeData&, std::string, std::string) {
            throw std::runtime_error("Invalid operation");
        }
        static SimpleData Create(const Extractor::FunctionData&, std::string, std::string) {
            throw std::runtime_error("Invalid operation");
        }
        static SimpleData Create(const Extractor::SpecialFunctionData&, std::string, std::string) {
            throw std::runtime_error("Invalid operation");
        }
        static SimpleData Create(const Extractor::OperatorOverloadData&, std::string, std::string) {
            throw std::runtime_error("Invalid operation");
        }

        std::string FilePath;
        std::string FileName;
        std::string DirectDependencies;
        std::string TotalDependencies;
        std::string LinesInFile;
        std::string TotalLines;
        std::string IncludedBy;
        std::string CouplingImpact;
        std::string SizeImpact;
    };
} // namespace Report