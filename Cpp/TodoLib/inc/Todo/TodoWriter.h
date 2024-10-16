#pragma once

#include "Todo/Todo.h"

namespace TodoWriter {
    bool UpdateTodo(Todo before, Todo after, bool clearReadonlyFlag);
}