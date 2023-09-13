#pragma once
#include "2015/d7_Circuit.h"

SOLUTION(2015, 7) {
    using Map = Constexpr::SmallMap<std::string, std::string>;

    constexpr void Resolve(const std::string& expr, Map& mapping);

    constexpr u16 GetValue(const std::string& str, Map& mapping) {
        u16 result;
        if (mapping.contains(str)) {
            Resolve(str, mapping);
            Constexpr::ParseNumber(mapping.at(str), result);
        }
        else {
            Constexpr::ParseNumber(str, result);
        }
        return result;
    }
    constexpr void Resolve(const std::string& key, Map& mapping) {
        auto expr = mapping.at(key);
        auto s = Constexpr::Split(expr, " ");
        if (s.size() == 1) {
            mapping[key] = Constexpr::ToString(GetValue(std::string(s[0]), mapping));
        }
        else if (s.size() == 2) {
            auto valStr = std::string(s[1]);
            auto val = GetValue(valStr, mapping);
            mapping[key] = Constexpr::ToString(static_cast<u16>(~val));
        }
        else {
            auto lhsStr = std::string(s[0]);
            auto op = std::string(s[1]);
            auto rhsStr = std::string(s[2]);

            auto lhs = GetValue(lhsStr, mapping);
            auto rhs = GetValue(rhsStr, mapping);

            if (op == "AND") mapping[key] = Constexpr::ToString(lhs & rhs);
            else if (op == "OR") mapping[key] = Constexpr::ToString(lhs | rhs);
            else if (op == "LSHIFT") mapping[key] = Constexpr::ToString(lhs << rhs);
            else if (op == "RSHIFT") mapping[key] = Constexpr::ToString(lhs >> rhs);
        }
    }

    constexpr Map ParseInput(const auto& lines) {
        Map result;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " -> ");
            result[std::string(s[1])] = std::string(s[0]);
        }
        
        return result;
    }

    constexpr std::string Solve(const auto& lines) {
        auto map = ParseInput(lines);
        Resolve("a", map);
        return map.at("a");
    }

    PART_ONE() {
        return Solve(Lines);
    }

    PART_TWO() {
        auto partOneResult = Solve(Lines);

        auto copy = CopyToVector(Lines);
        for (auto& line : copy) {
            if (line.substr(line.size() - 4) == "-> b") {
                line = partOneResult + " -> b";
                break;
            }
        }

        return Solve(copy);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "123 -> x",
            "456 -> y",
            "x AND y -> d",
            "x OR y -> e",
            "x LSHIFT 2 -> f",
            "y RSHIFT 2 -> g",
            "NOT x -> h",
            "NOT y -> i"
        };

        auto map = ParseInput(lines);
        Resolve("d", map);
        Resolve("e", map);
        Resolve("f", map);
        Resolve("g", map);
        Resolve("h", map);
        Resolve("i", map);

        if (map.at("x") != "123") return false;
        if (map.at("y") != "456") return false;
        if (map.at("d") != "72") return false;
        if (map.at("e") != "507") return false;
        if (map.at("f") != "492") return false;
        if (map.at("g") != "114") return false;
        if (map.at("h") != "65412") return false;
        if (map.at("i") != "65079") return false;


        lines = {
            "a -> b",
            "b -> c",
            "c -> d",
            "42 -> a"
        };

        map = ParseInput(lines);
        Resolve("d", map);
        if (map.at("a") != "42") return false;
        if (map.at("b") != "42") return false;
        if (map.at("c") != "42") return false;
        if (map.at("d") != "42") return false;

        return true;
    }
}
