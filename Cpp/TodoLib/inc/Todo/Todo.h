#pragma once

#include "Core/Platform/Types.h"

#include <string>

struct Todo {
    std::string FileName = "Unset";
    std::string FilePath = "Unset";
    u64 LineNumber = 0;
    std::string RelevantLine = "";

    std::string Classification = "Unclassified";
    std::string Status = "Unreviewed";
    std::string Notes = "";
};