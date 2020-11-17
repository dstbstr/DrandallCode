#ifndef __FUNCTIONDATA_H__
#define __FUNCTIONDATA_H__

#include "Extractor/Data/Visibility.h"
#include "Platform/Types.h"

#include <string>

namespace Extractor {
    struct FunctionData {
        std::string ClassName;
        std::string FunctionName;
        Visibility Visibility;
        u8 Airity{0};
        u8 DefaultParameterCount{0};
        bool IsAbstract{false};
        bool IsConst{false};
        bool IsVirtual{false};
        bool IsInline{false};
        bool IsStatic{false};
        bool IsTemplated{false};
    };
} // namespace Extractor

#endif // __FUNCTIONDATA_H__