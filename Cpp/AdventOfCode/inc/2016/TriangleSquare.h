#pragma once

#include "Common.h"

constexpr bool IsValid(u32 a, u32 b, u32 c) {
    if(a > b && a > c) {
        return b + c > a;
    } else if(b > a && b > c) {
        return a + c > b;
    } else {
        return a + b > c;
    }
}

static_assert(IsValid(5, 10, 25) == false);

u32 RunFirst(std::vector<std::string> lines) {
    u32 result = 0;
    u32 a = 0;
    u32 b = 0;
    u32 c = 0;

    for(auto line : lines) {
        auto split = StrUtil::Split(line, " ");
        ParseNumber(split[0], a);
        ParseNumber(split[1], b);
        ParseNumber(split[2], c);

        if(IsValid(a, b, c)) {
            result++;
        }
    }
    return result;
}

u32 Run(std::vector<std::string> lines) {
    u32 result = 0;
    std::array<std::array<u32, 3>, 3> tris;
    for(auto i = 0; i < lines.size(); i += 3) {
        auto split1 = StrUtil::Split(lines[i], " ");
        auto split2 = StrUtil::Split(lines[i + 1], " ");
        auto split3 = StrUtil::Split(lines[i + 2], " ");

        ParseNumber(split1[0], tris[0][0]);
        ParseNumber(split2[0], tris[0][1]);
        ParseNumber(split3[0], tris[0][2]);

        ParseNumber(split1[1], tris[1][0]);
        ParseNumber(split2[1], tris[1][1]);
        ParseNumber(split3[1], tris[1][2]);

        ParseNumber(split1[2], tris[2][0]);
        ParseNumber(split2[2], tris[2][1]);
        ParseNumber(split3[2], tris[2][2]);

        if(IsValid(tris[0][0], tris[0][1], tris[0][2])) {
            result++;
        }
        if(IsValid(tris[1][0], tris[1][1], tris[1][2])) {
            result++;
        }
        if(IsValid(tris[2][0], tris[2][1], tris[2][2])) {
            result++;
        }
    }

    return result;
}

bool RunTests() {
    auto lines = std::vector<std::string>{};
    lines.push_back("101 301 501");
    lines.push_back("102 302 502");
    lines.push_back("103 303 503");
    lines.push_back("201 401 601");
    lines.push_back("202 402 602");
    lines.push_back("203 403 603");

    if(Run(lines) != 6) return false;
    return true;
}