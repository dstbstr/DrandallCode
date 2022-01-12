#ifndef __TODOSETTINGS_H__
#define __TODOSETTINGS_H__
#include <regex>
#include <string>
#include <vector>

struct TodoSettings {
    std::string TodoStart = "";
    std::string TodoSeparator = "+-";
};
#endif // __TODOSETTINGS_H__