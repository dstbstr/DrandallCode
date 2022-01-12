#ifndef __TODOWRITER_H__
#define __TODOWRITER_H__
#include "Todo/Todo.h"

namespace TodoWriter {
    bool UpdateTodo(Todo before, Todo after, bool clearReadonlyFlag);
}

#endif // __TODOWRITER_H__