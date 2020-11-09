#ifndef __TYPEDATA_H__
#define __TYPEDATA_H__

#include "Platform/Types.h"
#include "IncludeCounter/Data/FunctionData.h"

#include <string>
#include <vector>

namespace IncludeCounter {
    struct TypeData {
        std::string ClassName;
        std::string FileName;

        bool IsClass; //if not a class, then it's a struct
        bool HasParent;
        std::vector<FunctionData> PublicFunctions;
        std::vector<FunctionData> ProtectedFunctions;
        std::vector<FunctionData> PrivateFunctions;

        u16 PrivateDataMemberCount;
        u16 ProtectedDataMemberCount;
        u16 PublicDataMemberCount;
    };
}
#endif // __TYPEDATA_H__