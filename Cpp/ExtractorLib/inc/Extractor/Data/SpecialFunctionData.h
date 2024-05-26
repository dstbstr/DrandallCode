#pragma once

#include "Extractor/Data/SpecialFunctionType.h"
#include "Extractor/Data/Visibility.h"

#include "Core/Platform/Types.h"

namespace Extractor {
    struct SpecialFunctionData {
        std::string Namespace;
        std::string ClassName;
        Visibility Visibility;
        SpecialFunctionType Kind;

        u64 LineCount{0};
        u8 Airity{0};
        u8 DefaultParameterCount{0};

        bool IsVirtual{false};
        bool IsDeleted{false};
        bool IsDefaulted{false};
        bool IsExplicit{false};
    };
}