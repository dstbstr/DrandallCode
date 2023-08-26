#include "2021/d8_DigitDisplay.h"

SOLUTION(2021, 8) {
    /*
      a
    b   c
      d
    e   f
      g

    a = (7 - 1)
    6 = (6 segments, but missing one of 1's segments)
    c = (1 - 6)
    f = 1 - c
    9 = (6 segments with all of 4's segments)
    g = (9 - 4 - a)
    3 = (5 segment with a, c, g, f)
    d = 3 - a, c, g, f
    0 = (6 segment - d)
    5 = (6 segment - c)
    2 = (6 segment - f)
    b = 5 - a, d, f, g
    e = 2 - a, c, d, g
    */

    constexpr void FindWiresAndNumbers(const auto& input, std::array<char, 'h'>& knownWires, std::array<std::string, 10>& knownNumbers) {
        std::array<std::vector<std::string>, 8> bySegments{};
        for (const auto& val : input) {
            bySegments[val.size()].push_back(std::string(val));
        }

        knownNumbers[1] = bySegments[2][0];
        knownNumbers[4] = bySegments[4][0];
        knownNumbers[7] = bySegments[3][0];
        knownNumbers[8] = bySegments[7][0];

        knownWires['a'] = knownNumbers[7][knownNumbers[7].find_first_not_of(knownNumbers[1])];
        knownNumbers[6] = *std::find_if(bySegments[6].begin(), bySegments[6].end(), [&](const std::string& val) {
            return val.find(knownNumbers[1][0]) == val.npos || val.find(knownNumbers[1][1]) == val.npos;
            });

        knownWires['c'] = knownNumbers[1][knownNumbers[1].find_first_not_of(knownNumbers[6])];
        knownWires['f'] = knownNumbers[1][knownNumbers[1].find_first_not_of(knownWires['c'])];
        knownNumbers[9] = *std::find_if(bySegments[6].begin(), bySegments[6].end(), [&](const std::string& val) {
            return knownNumbers[4].find_first_not_of(val) == std::string::npos;
            });

        std::string g = knownNumbers[9];
        Constexpr::RemoveAllOf(g, knownNumbers[4]);
        Constexpr::RemoveAllOf(g, knownWires['a']);
        knownWires['g'] = g[0];
        knownNumbers[3] = *std::find_if(bySegments[5].begin(), bySegments[5].end(), [&](const std::string& val) {
            return val.find(knownWires['a']) != val.npos &&
                val.find(knownWires['c']) != val.npos &&
                val.find(knownWires['g']) != val.npos &&
                val.find(knownWires['f']) != val.npos;
            });
        std::string d = knownNumbers[3];
        Constexpr::RemoveAllOf(d, std::string{ knownWires['a'], knownWires['c'], knownWires['g'], knownWires['f'] });
        knownWires['d'] = d[0];
        knownNumbers[0] = *std::find_if(bySegments[6].begin(), bySegments[6].end(), [&](const std::string& val) {
            return val.find(knownWires['d']) == val.npos;
            });
        knownNumbers[5] = *std::find_if(bySegments[5].begin(), bySegments[5].end(), [&](const std::string& val) {
            return val.find(knownWires['c']) == val.npos;
            });
        knownNumbers[2] = *std::find_if(bySegments[5].begin(), bySegments[5].end(), [&](const std::string& val) {
            return val.find(knownWires['f']) == val.npos;
            });
        std::string b = knownNumbers[5];
        Constexpr::RemoveAllOf(b, std::string{ knownWires['a'], knownWires['d'], knownWires['f'], knownWires['g'] });
        knownWires['b'] = b[0];

        std::string e = knownNumbers[2];
        Constexpr::RemoveAllOf(e, std::string{ knownWires['a'], knownWires['c'], knownWires['d'], knownWires['g'] });
        knownWires['e'] = e[0];

        for (auto& num : knownNumbers) {
            std::sort(num.begin(), num.end());
        }
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        size_t result = 0;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " | ");
            auto s2 = Constexpr::Split(split[1], " ");
            result += std::count_if(s2.begin(), s2.end(), [](std::string_view sv) {
                return sv.size() < 5 || sv.size() == 7;
                });
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        size_t result = 0;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " | ");
            auto lhs = Constexpr::Split(split[0], " ");
            auto rhs = Constexpr::Split(split[1], " ");
            std::array<char, 'h'> knownWires{};
            std::array<std::string, 10> knownNumbers{};
            FindWiresAndNumbers(lhs, knownWires, knownNumbers);

            size_t number = 0;
            size_t multiplier = 1000;
            for (auto digit : rhs) {
                auto key = std::string(digit);
                std::sort(key.begin(), key.end());
                for (auto i = 0; i < knownNumbers.size(); i++) {
                    if (knownNumbers[i] == key) {
                        number += (i * multiplier);
                        multiplier /= 10;
                        break;
                    }
                }
            }
            result += number;
        }

        return result;
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(Lines));
    }

    TESTS() {
        std::vector<std::string> lines = { "abcefg cf acdeg acdfg bcdf abdfg abdefg acf abcdefg abcdfg | fc fca cf caf" };
        if (SolvePartTwo(lines) != 1717) return false;

        lines = {
            "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
            "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
            "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
            "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
            "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
            "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
            "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
            "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
            "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
            "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"
        };

        if (SolvePartOne(lines) != 26) return false;
        if (SolvePartTwo(lines) != 61229) return false;
        return true;
    }
}