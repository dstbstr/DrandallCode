#ifndef __TYPEDATA_H__
#define __TYPEDATA_H__

#include "IncludeCounter/Data/FunctionData.h"
#include "IncludeCounter/Data/TypeKeyword.h"
#include "Platform/Types.h"

#include <string>
#include <vector>

namespace IncludeCounter {
    struct TypeData {
        std::string ClassName;
        std::string FileName;

        TypeKeyword TypeKind;
        bool HasBaseClass{false};
        bool IsTemplated{false};
        std::vector<FunctionData> Functions;
        std::vector<TypeData> InnerTypes;

        u16 PrivateDataMemberCount{0};
        u16 ProtectedDataMemberCount{0};
        u16 PublicDataMemberCount{0};
    };
} // namespace IncludeCounter
#endif // __TYPEDATA_H__