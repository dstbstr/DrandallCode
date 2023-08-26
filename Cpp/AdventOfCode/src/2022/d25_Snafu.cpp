#include "2022/d25_Snafu.h"

SOLUTION(2022, 25) {
    template<size_t Count>
    constexpr std::array<s64, Count> GetPowers() {
        std::array<s64, Count> result{};
        result.back() = 1;
        for (int i = static_cast<int>(Count) - 2; i >= 0; i--) {
            result[i] = result[i + 1] * 5;
        }
        return result;
    }

    constexpr std::array<s64, 21> Powers = GetPowers<21>();

    constexpr s64 FromSnafu(std::string_view value) {
        s64 result = 0;
        u64 place = 1;

        for (s32 i = static_cast<s32>(value.size() - 1); i >= 0; i--) {
            switch (value[i]) {
            case '2': result += (2 * place); break;
            case '1': result += place; break;
            case '0': break;
            case '-': result -= place; break;
            case '=': result -= (2 * place); break;
            default: break;
            }

            place *= 5;
        }
        return static_cast<s64>(result);
    }

    constexpr std::string ToSnafu(u64 value) {
        s64 running = value;
        int i = 0;
        while (running < Powers[i]) {
            i++;
        }

        if (running > Powers[i] * 2) {
            i--;
        }

        std::string result = "";
        while (i < Powers.size()) {
            auto power = Powers[i];
            if (running < 0) {
                if (Constexpr::Abs(running + power) > Constexpr::Abs(running)) {
                    result += "0";
                }
                else if (Constexpr::Abs(running + power * 2) < Constexpr::Abs(running + power)) {
                    running += power * 2;
                    result += "=";
                }
                else {
                    running += power;
                    result += '-';
                }
            }
            else if (running > 0) {
                if (Constexpr::Abs(running - power) > running) {
                    result += "0";
                }
                else if (Constexpr::Abs(running - power * 2) < Constexpr::Abs(running - power)) {
                    running -= power * 2;
                    result += "2";
                }
                else {
                    running -= power;
                    result += "1";
                }
            }
            else {
                result += "0";
            }
            i++;
        }

        if (running != 0) {
            throw std::logic_error("Failed to reach 0");
        }
        return result;
    }

    constexpr std::string SolvePartOne(const auto& lines) {
        u64 result = 0;
        for (const auto& line : lines) {
            result += FromSnafu(line);
        }
        return ToSnafu(result);
    }
    PART_ONE() {
        return SolvePartOne(Lines);
    }

    PART_TWO() {
        return "Merry Christmas!";
    }

    TESTS() {
        static_assert(FromSnafu("1=-0-2") == 1747);
        static_assert(FromSnafu("1121-1110-1=0") == 314159265);

        static_assert(ToSnafu(1) == "1");
        static_assert(ToSnafu(2) == "2");
        static_assert(ToSnafu(3) == "1=");
        static_assert(ToSnafu(4) == "1-");
        static_assert(ToSnafu(5) == "10");
        static_assert(ToSnafu(6) == "11");
        static_assert(ToSnafu(7) == "12");
        static_assert(ToSnafu(8) == "2=");
        static_assert(ToSnafu(9) == "2-");
        static_assert(ToSnafu(10) == "20");
        static_assert(ToSnafu(1747) == "1=-0-2");
        static_assert(ToSnafu(314159265) == "1121-1110-1=0");

        std::vector<std::string> lines = {
            "1=-0-2",
            "12111",
            "2=0=",
            "21",
            "2=01",
            "111",
            "20012",
            "112",
            "1=-1=",
            "1-12",
            "12",
            "1=",
            "122"
        };

        if (SolvePartOne(lines) != "2=-1=0") return false;

        return true;
    }
}