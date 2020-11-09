#ifndef __FUNCTIONDATA_H__
#define __FUNCTIONDATA_H__

#include "Platform/Types.h"

#include <string>

namespace IncludeCounter {
    struct FunctionData {
        enum Visibility {PRIVATE, PROTECTED, PUBLIC};
        std::string ClassName;
        std::string FunctionName;
        Visibility Visibility;
        u8 Airity;
        u8 DefaultParameterCount;
        bool IsAbstract;
        bool IsConst;
        bool IsVirtual;
        bool IsStatic;
    };
}

#endif // __FUNCTIONDATA_H__