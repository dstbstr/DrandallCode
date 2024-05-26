#pragma once

#include "Core/Platform/Types.h"
#include "Todo/Todo.h"

#include <vector>

namespace TodoReportGenerator {
    void WriteToExcel(const std::vector<Todo>& todos, std::string fileName);
};