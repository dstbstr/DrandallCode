#pragma once

#include "Extractor/Data/TypeData.h"

namespace Report {
    struct TypeWithMethodCounts : public Extractor::TypeData {
        TypeWithMethodCounts(Extractor::TypeData data) {
            using namespace Extractor;
            for(auto&& func: data.Functions) {
                switch(func.Visibility) {
                case PUBLIC: PublicMethodCount++; break;
                case PROTECTED: ProtectedMethodCount++; break;
                case PRIVATE: PrivateMethodCount++; break;
                default: break;
                }
            }
        }
        u32 PublicMethodCount{0};
        u32 ProtectedMethodCount{0};
        u32 PrivateMethodCount{0};
    };
} // namespace Report