#include "2017/d13_Firewall.h"

SOLUTION(2017, 13) {
    using Scanner = std::pair<u32, u32>;

    constexpr Scanner ParseScanner(const std::string& line) {
        u32 layer, depth;
        auto s = Constexpr::Split(line, ": ");
        Constexpr::ParseNumber(s[0], layer);
        Constexpr::ParseNumber(s[1], depth);
        return { layer, depth };
    }

    constexpr bool Caught(const Scanner& scanner, u32 delay = 0) {
        auto [layer, depth] = scanner;
        return (delay + layer) % ((depth - 1) * 2) == 0;
    }

    PART_ONE() {
        u32 total = 0;
        auto scanners = ParseLines<Scanner>(lines, ParseScanner);
        for (const auto& scanner : scanners) {
            if (Caught(scanner)) total += scanner.first * scanner.second;
        }
        return Constexpr::ToString(total);
    }

    PART_TWO() {
        auto scanners = ParseLines<Scanner>(lines, ParseScanner);
        u32 delay = 0;
        while (std::any_of(scanners.begin(), scanners.end(), [delay](auto const& scanner) {
            return Caught(scanner, delay);
        })) {
            ++delay;
        }

        return Constexpr::ToString(delay);
    }

    TESTS() {
        return true;
    }
}