#pragma once

#include "Extractor/Data/Visibility.h"

#include "Core/Platform/Types.h"

#include <string>

namespace Extractor {
    struct OperatorOverloadData {
        std::string Namespace;
        std::string ClassName;
        std::string Operator;
        Visibility Visibility;
        u8 Airity{0};
        u64 LineCount{0};

        bool IsExplicit{false};
        bool IsFriend{false};
        bool IsInline{false};
    };
} // namespace Extractor