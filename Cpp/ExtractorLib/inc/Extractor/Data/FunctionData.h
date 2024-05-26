#pragma once

#include "Extractor/Data/Visibility.h"

#include "Core/Platform/Types.h"

#include <string>

namespace Extractor {
    struct FunctionData {
        std::string Namespace;
        std::string ClassName;
        std::string FunctionName;
        Visibility Visibility;
        u8 Airity{0};
        u8 DefaultParameterCount{0};
        u64 LineCount{0};
        bool IsAbstract{false};
        bool IsConst{false};
        bool IsVirtual{false};
        bool IsInline{false};
        bool IsStatic{false};
        bool IsTemplated{false};
    };
} // namespace Extractor