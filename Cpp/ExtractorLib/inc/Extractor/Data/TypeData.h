#pragma once

#include "Extractor/Data/FunctionData.h"
#include "Extractor/Data/OperatorOverloadData.h"
#include "Extractor/Data/SpecialFunctionData.h"
#include "Extractor/Data/TypeKeyword.h"

#include "Core/Platform/Types.h"

#include <string>
#include <vector>

namespace Extractor {
    struct TypeData {
        std::string Namespace;
        std::string ClassName;
        std::string FileName;

        TypeKeyword TypeKind;
        bool HasBaseClass{false};
        bool IsTemplated{false};
        std::vector<FunctionData> Functions;
        std::vector<SpecialFunctionData> SpecialFunctions;
        std::vector<OperatorOverloadData> OperatorOverloads;
        std::vector<TypeData> InnerTypes;

        u64 LineCount{0};
        u64 IfDefCount{0};
        u16 PrivateDataMemberCount{0};
        u16 ProtectedDataMemberCount{0};
        u16 PublicDataMemberCount{0};
    };
} // namespace Extractor