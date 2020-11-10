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
        bool HasBaseClass;
        std::vector<FunctionData> Functions;
        std::vector<TypeData> InnerTypes;

        u16 PrivateDataMemberCount;
        u16 ProtectedDataMemberCount;
        u16 PublicDataMemberCount;
    };
} // namespace IncludeCounter
#endif // __TYPEDATA_H__