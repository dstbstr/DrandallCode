#pragma once

#include "Common.h"

DECLARE_SOLUTION(2023, 1);

namespace Year2023Day1 {
    constexpr std::string_view input =
#include "generated/2023_d1.txt"
;
    constexpr auto inputLines = SplitInputIntoLines<3>(input);
}