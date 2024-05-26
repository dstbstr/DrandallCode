#include "Report/DotToSvg.h"

#include "Core/Platform/ExecuteCommand.h"
#include "Core/Utilities/FileUtils.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

namespace {
    constexpr std::string GetOutputFileName(const std::string absolutePath) {
        auto copy = absolutePath;
        return copy.replace(absolutePath.length() - 4, 4, ".svg");
    }

    static constexpr char CommandName[] = "dot";
} // namespace
namespace Report {
    void ConvertDotsToSvg(const std::string& directory) {
        const auto dotFiles = FileUtils::GetFullyQualifiedFilesWithExtension(directory, ".dot");
        for(const auto& file: dotFiles) {
            const std::string outputFileName = GetOutputFileName(file);
            const std::string args = "-Tsvg " + file + " > " + outputFileName;

            std::string errorMessage;
            if(!Platform::ExecuteCommand(CommandName, args, errorMessage)) {
                std::cout << errorMessage;
            }
        }
    }

    void DeleteDotFiles(const std::string& directory) {
        FileUtils::DeleteFiles(directory, ".dot");
    }
} // namespace Report