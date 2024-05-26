#pragma once
#include <string>

namespace Platform {
    void ExecuteCommand(const std::string& command, const std::string& arguments);
    bool ExecuteCommand(const std::string& command, const std::string& arguments, std::string& outErrorMessage);
    void ExecuteCommandKeepWindow(const std::string& command, const std::string& arguments);
    bool ExecuteCommandKeepWindow(const std::string& command, const std::string& arguments, std::string& outErrorMessage);
}