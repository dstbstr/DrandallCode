#ifndef __TYPEDATA_H__
#define __TYPEDATA_H__

#include "Extractor/Data/FunctionData.h"
#include "Extractor/Data/TypeKeyword.h"
#include "Platform/Types.h"

#include <string>
#include <vector>

namespace Extractor {
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
} // namespace Extractor
#endif // __TYPEDATA_H__