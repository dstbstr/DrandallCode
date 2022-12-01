#pragma once

#include "Platform/Types.h"
#include <string_view>

constexpr u32 CalculateWrappingPaper(u32 h, u32 w, u32 l)
{
    auto d1 = h * w;
    auto d2 = h * l;
    auto d3 = w * l;

    if (d1 <= d2 && d1 <= d3) {
        return (d1 * 3) + (d2 * 2) + (d3 * 2);
    } else if (d2 <= d1 && d2 <= d3) {
        return (d1 * 2) + (d2 * 3) + (d3 * 2);
    } else {
        return (d1 * 2) + (d2 * 2) + (d3 * 3);
    }
}

static_assert(CalculateWrappingPaper(2, 3, 4) == 58);
static_assert(CalculateWrappingPaper(1, 1, 10) == 43);

constexpr u64 CalculateRibbon(u32 h, u32 w, u32 l)
{
    u64 area = h * w * l;
    if(h >= w && h >= l) {
        return area + w + w + l + l;
    } else if(w >= h && w >= l) {
        return area + h + h + l + l;
    } else if(l >= w && l >= h) {
        return area + w + w + h + h;
    }
    return 0;
}

static_assert(CalculateRibbon(2, 3, 4) == 34);
static_assert(CalculateRibbon(1, 1, 10) == 14);
static_assert(CalculateRibbon(2, 2, 2) == 16);