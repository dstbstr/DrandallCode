#include "2016/d20_Firewall.h"

SOLUTION(2016, 20) {
    constexpr u32 MaxIp = 4294967295;

    constexpr std::vector<UCoord> GetRanges(const std::vector<std::string>& lines) {
        std::vector<UCoord> ranges;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, "-");
            UCoord range;
            Constexpr::ParseNumber(split[0], range.X);
            Constexpr::ParseNumber(split[1], range.Y);
            ranges.push_back(range);
        }

        return ranges;
    }

    constexpr u64 FindLowest(const std::vector<std::string>& lines) {
        auto ranges = GetRanges(lines);
        std::ranges::sort(ranges, {}, &UCoord::X);

        auto bottomRange = ranges[0];
        for (size_t i = 1; i < ranges.size(); i++) {
            auto next = ranges[i];
            if (bottomRange.Y >= next.X - 1) {
                bottomRange.Y = std::max(bottomRange.Y, next.Y);
            }
            else {
                return bottomRange.Y + 1;
            }
        }

        return 0;
    }

    constexpr u64 CountAvailable(const std::vector<std::string> &lines) {
        auto ranges = GetRanges(lines);
        std::ranges::sort(ranges, {}, &UCoord::X);

        u64 result = 0;
        auto current = ranges[0];
        for (size_t i = 1; i < ranges.size(); i++) {
            auto next = ranges[i];
            if (current.Y >= next.X - 1) {
                current.Y = std::max(current.Y, next.Y);
            }
            else {
                result += next.X - current.Y - 1;
                current.Y = next.Y;
            }
        }

        if (current.Y < MaxIp) {
            result += MaxIp - current.Y;
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(FindLowest(lines));
    }
    PART_TWO() {
        return Constexpr::ToString(CountAvailable(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "5-8",
            "0-2",
            "4-7"
        };

        if (FindLowest(lines) != 3) return false;

        return true;
    }
}

