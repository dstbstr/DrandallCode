#pragma once

#include "Core/Platform/Types.h"

#include <array>
#include <bit>


constexpr u32 BytesToU32Big(u8 b1, u8 b2, u8 b3, u8 b4) {
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}
constexpr u32 BytesToU32Little(u8 b1, u8 b2, u8 b3, u8 b4) {
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}

constexpr u32 BytesToU32(u8 b1, u8 b2, u8 b3, u8 b4) {
    if constexpr (std::endian::native == std::endian::big) {
        return BytesToU32Big(b1, b2, b3, b4);
    }
    else {
        return BytesToU32Little(b1, b2, b3, b4);
    }
}

constexpr u16 BytesToU16Big(u8 b1, u8 b2) {
    return (b1 << 8) | b2;
}

constexpr u16 BytesToU16Little(u8 b1, u8 b2) {
    return (b2 << 8) | b1;
}

constexpr u16 BytesToU16(u8 b1, u8 b2) {
    if constexpr (std::endian::native == std::endian::big) {
        return BytesToU16Big(b1, b2);
    }
    else {
        return BytesToU16Little(b1, b2);
    }
}

constexpr std::array<u8, 4> U32ToBytesLittle(u32 input) {
    std::array<u8, 4> result;
    result[3] = static_cast<u8>((input & 0xff000000U) >> 24u);
    result[2] = static_cast<u8>((input & 0x00ff0000U) >> 16u);
    result[1] = static_cast<u8>((input & 0x0000ff00U) >> 8u);
    result[0] = static_cast<u8>((input & 0x000000ffU));

    return result;
}

constexpr std::array<u8, 4> U32ToBytesBig(u32 input) {
    std::array<u8, 4> result;
    result[0] = static_cast<u8>((input & 0xff000000U) >> 24u);
    result[1] = static_cast<u8>((input & 0x00ff0000U) >> 16u);
    result[2] = static_cast<u8>((input & 0x0000ff00U) >> 8u);
    result[3] = static_cast<u8>((input & 0x000000ffU));

    return result;
}

constexpr std::array<u8, 4> U32ToBytes(u32 input) {
    if constexpr (std::endian::native == std::endian::big) {
        return U32ToBytesBig(input);
    }
    else {
        return U32ToBytesLittle(input);
    }
}

constexpr std::array<u8, 2> U16ToBytesLittle(u16 input) {
    std::array<u8, 2> result;
    result[1] = (input & 0xFF00u) >> 8u;
    result[0] = (input & 0x00FFu);

    return result;
}

constexpr std::array<u8, 2> U16ToBytesBig(u16 input) {
    std::array<u8, 2> result;
    result[0] = (input & 0xFF00u) >> 8u;
    result[1] = (input & 0x00FFu);

    return result;
}

constexpr std::array<u8, 2> U16ToBytes(u16 input) {
    if constexpr (std::endian::native == std::endian::big) {
        return U16ToBytesBig(input);
    }
    else {
        return U16ToBytesLittle(input);
    }
}

//   0        0       1  2    3    4
//00000000 00000000 00010010 00110100
//AND
//00000000 00000000 00000000 11111111

//00000000 00000000 00010010 00110100
//AND
//11111111 11111111 11111111 00000000


//00000000 00000000 00000000 11111111
//IS
//100000000 - 1

//11111111 11111111 11111111 00000000
//IS
//11111111 11111111 11111111 << 8

//00000000 00000000 00000000 11111111
//IS
//11111111 11111111 11111111 11111111
//Minus
//11111111 11111111 11111111 00000000

namespace {
    constexpr std::array<u32, 2> SplitValImpl(u32 input, u32 bits)
    {
        std::array<u32, 2> result;

        auto leftMask = (1u << bits) - 1;
        auto rightMask = (~0u) - leftMask;

        result[0] = input & leftMask;
        result[1] = (input & rightMask) >> bits;
        return result;
    }

    constexpr std::array<u16, 2> SplitValImpl(u16 input, u16 bits)
    {
        std::array<u16, 2> result;

        auto leftMask = (u16(1) << bits) - 1;
        auto rightMask = (~(u16(0))) - leftMask;

        result[0] = input & leftMask;
        result[1] = (input & rightMask) >> bits;
        return result;
    }
}

constexpr std::array<u32, 2> SplitValBig(u32 input, u32 bits) {
    return SplitValImpl(input, bits);
}

constexpr std::array<u32, 2> SplitValLittle(u32 input, u32 bits) {
    return SplitValImpl(input, bits);
}

constexpr std::array<u32, 2> SplitVal(u32 input, u32 bits) {
    if constexpr (std::endian::native == std::endian::big) {
        return SplitValBig(input, bits);
    }
    else {
        return SplitValLittle(input, bits);
    }
}

constexpr std::array<u16, 2> SplitVal(u16 input, u16 bits) {
    if constexpr (std::endian::native == std::endian::big) {
        return SplitValImpl(input, bits);
    }
    else {
        return SplitValImpl(input, bits);
    }
}
