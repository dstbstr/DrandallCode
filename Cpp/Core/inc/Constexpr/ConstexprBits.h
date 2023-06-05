#pragma once

#include "Platform/Types.h"
#include <typeinfo>

namespace Constexpr {
    template<typename T, typename U>
    constexpr bool IsBitSet(T bits, U targetBit) {
        static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>);

        return bits & (T(1) << targetBit);
    }
    static_assert(IsBitSet(1, 0));
    static_assert(IsBitSet(2, 1));
    static_assert(IsBitSet(3, 0));
    static_assert(IsBitSet(3, 1));
    static_assert(!IsBitSet(2, 0));
    static_assert(!IsBitSet(4, 0));
    static_assert(!IsBitSet(4, 1));

    template<typename T, typename U>
    constexpr void SetBit(T& bits, U targetBit) {
        static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>);
        bits |= (T(1) << targetBit);
    }
    template<typename T, typename U>
    constexpr T WithBit(T bits, U targetBit) {
        T result = bits;
        SetBit(result, targetBit);
        return result;
    }

    static_assert(WithBit(0, 0) == 1);
    static_assert(WithBit(0, 1) == 2);
    static_assert(WithBit(0, 2) == 4);
    static_assert(WithBit(2, 1) == 2);
    static_assert(WithBit(4, 2) == 4);

    template<typename T, typename U>
    constexpr void UnsetBit(T& bits, U targetBit) {
        static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>);
        T mask = ~(T(1) << targetBit);
        bits &= mask;
    }
    template<typename T, typename U>
    constexpr T WithoutBit(T bits, U targetBit) {
        static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>);
        T result = bits;
        UnsetBit(result, targetBit);
        return result;
    }

    static_assert(WithoutBit(4, 2) == 0);
    static_assert(WithoutBit(2, 1) == 0);
    static_assert(WithoutBit(1, 0) == 0);
    static_assert(WithoutBit(3, 2) == 3);
    static_assert(WithoutBit(4, 1) == 4);
    static_assert(WithoutBit(15, 0) == 14);

    template<typename T, typename U>
    constexpr void ToggleBit(T& bits, U targetBit) {
        static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>);
        bits ^= (T(1) << targetBit);
    }
    template<typename T, typename U>
    constexpr T WithToggle(T bits, U targetBit) {
        T result = bits;
        ToggleBit(result, targetBit);
        return result;
    }

    static_assert(WithToggle(0, 0) == 1);
    static_assert(WithToggle(1, 0) == 0);
    static_assert(WithToggle(0, 1) == 2);
    static_assert(WithToggle(2, 1) == 0);

    template<typename T, char TrueVal = '1', char FalseVal = '0'>
    constexpr T FromBinary(const std::string& binary) {
        T mul = 1;
        T result = 0;
        for (int i = static_cast<int>(binary.size() - 1); i >= 0; i--) {
            result += mul * (binary[i] == TrueVal);
            mul *= 2;
        }
        return result;
    }

    static_assert(FromBinary<u32>("1011") == 11);
    static_assert(FromBinary<size_t, '#', '.'>("#.##") == 11);
}