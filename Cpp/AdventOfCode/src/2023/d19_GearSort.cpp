#include "2023/d19_GearSort.h"

SOLUTION(2023, 19) {
    struct Gear {
        std::array<u32, 4> Xmas;
    };

    struct GearRange {
        std::array<std::pair<size_t, size_t>, 4> Xmas;

        constexpr size_t GetCount() const {
            size_t result = 1;
            for (auto [start, end] : Xmas) {
                result *= (end - start);
            }
            return result;
        }
    };

    struct Inst {
        size_t XmasIndex;
        bool Gt;
        u32 Value;
        std::string Dest;
    };

    constexpr Gear ParseGear(const std::string& line) {
        auto s = Constexpr::Split(line, ",");
        Gear result{};
        size_t XmasIndex = 0;
        for (auto part : s) {
            auto b = Constexpr::Split(part, "=");
            if (b[1].back() == '}') {
                b[1] = b[1].substr(0, b[1].size() - 1);
            }

            Constexpr::ParseNumber(b[1], result.Xmas[XmasIndex++]);
        }
        return result;
    }

    static_assert(ParseGear("{x=787,m=2655,a=1222,s=2876}").Xmas == std::array<u32, 4>{787, 2655, 1222, 2876});

    constexpr Inst ParseInst(std::string_view sv) {
        Inst inst;
        if (sv.find(":") == sv.npos) {
            inst.Dest = std::string(sv);
            inst.Gt = true;
            inst.Value = 0;
            inst.XmasIndex = 99;
        }
        else {
            auto c = Constexpr::Split(sv, ":");
            switch (c[0][0]) {
            case 'x': inst.XmasIndex = 0; break;
            case 'm': inst.XmasIndex = 1; break;
            case 'a': inst.XmasIndex = 2; break;
            default: inst.XmasIndex = 3; break;
            }

            inst.Gt = c[0][1] == '>';
            auto valStr = c[0].substr(2);
            Constexpr::ParseNumber(valStr, inst.Value);
            inst.Dest = c[1];
        }

        Constexpr::RemoveAllOf(inst.Dest, "}");
        return inst;
    }

    constexpr auto ParseFunc(const std::string& line, std::string& outLabel) {
        auto a = Constexpr::Split(line, "{");
        outLabel = std::string(a[0]);
        auto b = Constexpr::Split(a[1], ",");

        auto parameters = ParseLines(b, ParseInst);

        return [parameters](const Gear& gear) {
            for (auto p : parameters) {
                if (p.XmasIndex == 99) return p.Dest;
                auto val = gear.Xmas[p.XmasIndex];
                if (p.Gt && val > p.Value) return p.Dest;
                else if (!p.Gt && val < p.Value) return p.Dest;
            }
            throw "Wat?";
            };
    }

    constexpr auto ParseFunc2(const std::string& line, std::string& outLabel) {
        auto a = Constexpr::Split(line, "{");
        outLabel = std::string(a[0]);
        auto b = Constexpr::Split(a[1], ",");

        auto parameters = ParseLines(b, ParseInst);

        return [parameters](const GearRange& range) {
            std::vector<std::pair<std::string, GearRange>> result;
            GearRange running = range;
            for (auto p : parameters) {
                auto match = running;
                if (p.XmasIndex == 99) {
                }
                else if (p.Gt) {
                    match.Xmas[p.XmasIndex].first = p.Value;
                    running.Xmas[p.XmasIndex].second = p.Value;
                }
                else {
                    match.Xmas[p.XmasIndex].second = p.Value - 1;
                    running.Xmas[p.XmasIndex].first = p.Value - 1;
                }

                result.push_back(std::make_pair(p.Dest, match));
            }

            return result;
            };
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);
        Constexpr::BigMap<std::string, std::function<std::string(const Gear&)>> funcs{};
        for (const auto& line : groups[0]) {
            std::string label;
            auto func = ParseFunc(line, label);

            funcs.emplace(label, func);
        }

        auto gears = ParseLines(groups[1], ParseGear);
        std::vector<Gear> accepted;

        for (const auto& gear : gears) {
            std::string next = "in";
            while (true) {
                next = funcs.at(next)(gear);
                if (next == "A") {
                    accepted.push_back(gear);
                    break;
                }
                else if (next == "R") {
                    break;
                }
            }
        }

        size_t result = 0;
        for (const auto& gear : accepted) {
            result += gear.Xmas[0] + gear.Xmas[1] + gear.Xmas[2] + gear.Xmas[3];
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);
        Constexpr::BigMap<std::string, std::function<std::vector<std::pair<std::string, GearRange>>(const GearRange&)>> funcs{};
        for (const auto& line : groups[0]) {
            std::string label;
            auto func = ParseFunc2(line, label);

            funcs.emplace(label, func);
        }

        GearRange initial{};
        for (auto& [start, end] : initial.Xmas) {
            start = 0;
            end = 4000;
        }

        size_t accepted = 0;

        std::vector<std::pair<std::string, GearRange>> q {std::make_pair("in", initial)};
        while (!q.empty()) {
            auto [label, range] = q.back();
            q.pop_back();

            auto next = funcs.at(label)(range);
            for (auto [nextLabel, nextRange] : next) {
                if (nextLabel == "A") accepted += nextRange.GetCount();
                else if (nextLabel != "R") q.push_back(std::make_pair(nextLabel, nextRange));
            }
        }

        return Constexpr::ToString(accepted);
    }

    TESTS() {
        std::vector<std::string> example = {
        "px{a<2006:qkq,m>2090:A,rfg}",
        "pv{a>1716:R,A}",
        "lnx{m>1548:A,A}",
        "rfg{s<537:gd,x>2440:R,A}",
        "qs{s>3448:A,lnx}",
        "qkq{x<1416:A,crn}",
        "crn{x>2662:A,R}",
        "in{s<1351:px,qqz}",
        "qqz{s>2770:qs,m<1801:hdj,R}",
        "gd{a>3333:R,R}",
        "hdj{m>838:A,pv}",
        "",
        "{x=787,m=2655,a=1222,s=2876}",
        "{x=1679,m=44,a=2067,s=496}",
        "{x=2036,m=264,a=79,s=2244}",
        "{x=2461,m=1339,a=466,s=291}",
        "{x=2127,m=1623,a=2188,s=1013}"
        };

        if (PartOne(example) != "19114") return false;
        if (PartTwo(example) != "167409079868000") return false;

        return true;
    }
}