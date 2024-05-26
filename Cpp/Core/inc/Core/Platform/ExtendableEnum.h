#pragma once

#include <vcruntime.h>

#if !_HAS_CXX20
#include "Utilities/ConstexprCounter.h"

#define EXTENDABLE_ENUM_COUNT(BaseEnum) ConstexprCounter::Count<BaseEnum>() - 1

template<typename T>
struct ExtendableEnum {
    const int id;

    constexpr bool operator==(const ExtendableEnum<T>& other) const {
        return id == other.id;
    }
    constexpr bool operator!=(const ExtendableEnum<T>& other) const {
        return !(*this == other);
    }
    constexpr bool operator<(const ExtendableEnum<T>& other) const {
        return id < other.id;
    }
    constexpr operator int() const {
        return id;
    }

    template<int N = ConstexprCounter::Next<T>()>
    constexpr ExtendableEnum() : id(N - 1) {}
};
#endif