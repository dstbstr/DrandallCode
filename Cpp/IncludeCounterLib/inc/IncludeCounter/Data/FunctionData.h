#ifndef __FUNCTIONDATA_H__
#define __FUNCTIONDATA_H__

#include "IncludeCounter/Data/Visibility.h"
#include "Platform/Types.h"

#include <string>

namespace IncludeCounter {
    struct FunctionData {
        std::string ClassName;
        std::string FunctionName;
        Visibility Visibility;
        u8 Airity;
        u8 DefaultParameterCount;
        bool IsAbstract;
        bool IsConst;
        bool IsVirtual;
        bool IsStatic;
        bool IsTemplated;
    };
} // namespace IncludeCounter

#endif // __FUNCTIONDATA_H__