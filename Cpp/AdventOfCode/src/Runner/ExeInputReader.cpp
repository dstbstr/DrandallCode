#include "Runner/ExeInputReader.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <libloaderapi.h>
#include <filesystem>
#include <memory>
#include <fstream>

std::vector<std::string> ExeInputReader::ReadInput(size_t year, size_t day) const {
    std::vector<std::string> result{};
    auto buffer = std::make_unique<char*>(new char[1024]);
    GetModuleFileNameA(nullptr, *buffer, MAX_PATH);
    auto exePath = std::string(*buffer);
    exePath = exePath.substr(0, exePath.find_last_of("/\\"));
    auto targetFile = std::filesystem::path(exePath) / "Inputs" / std::to_string(year) / ("d" + std::to_string(day) + ".txt");
    auto stream = std::ifstream(targetFile.c_str());
    if (!stream.is_open()) {
        return {};
    }

    std::string line;
    while (std::getline(stream, line)) {
        result.push_back(line);
    }

    return result;
}