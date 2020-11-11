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
        u8 Airity{0};
        u8 DefaultParameterCount{0};
        bool IsAbstract{false};
        bool IsConst{false};
        bool IsVirtual{false};
        bool IsStatic{false};
        bool IsTemplated{false};
    };
} // namespace IncludeCounter

#endif // __FUNCTIONDATA_H__