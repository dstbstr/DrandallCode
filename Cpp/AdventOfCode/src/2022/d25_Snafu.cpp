#include "2022/d25_Snafu.h"

SOLUTION(2022, 25) {
    static constexpr std::array<s64, 21> Powers = {
    95'367'431'640'625,         // 20
    19'073'486'328'125,         // 19
    3'814'697'265'625,          // 18
    762'939'453'125,            // 17
    152'587'890'625,            // 16
    30'517'578'125,             // 15
    6'103'515'625,              // 14
    1'220'703'125,              // 13
    244'140'625,                // 12
    48'828'125,                 // 11
    9'765'625,                  // 10
    1'953'125,                  // 9
    390'625,                    // 8
    78'125,                     // 7
    15'625,                     // 6
    3'125,                      // 5
    625,                        // 4
    125,                        // 3
    25,                         // 2
    5,                          // 1
    1                           // 0
    };

    constexpr s64 FromSnafu(const std::string & value) {
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

    static_assert(FromSnafu("1=-0-2") == 1747);
    static_assert(FromSnafu("1121-1110-1=0") == 314159265);

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


    auto Part1(const std::vector<std::string>&lines) {
        u64 result = 0;
        for (const auto& line : lines) {
            result += FromSnafu(line);
        }
        return ToSnafu(result);
    }

    auto Part2(const std::vector<std::string>&lines) {
        return lines.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        return Part1(lines);
    }

    bool RunTests() {
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

        if (Part1(lines) != "2=-1=0") return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}