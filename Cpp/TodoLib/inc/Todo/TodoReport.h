#ifndef __TODOREPORT_H__
#define __TODOREPORT_H__
#include "Platform/Types.h"
#include "Todo/Todo.h"

#include <vector>

namespace TodoReportGenerator {
    void WriteToExcel(const std::vector<Todo>& todos, std::string fileName);
};
#endif // __TODOREPORT_H__