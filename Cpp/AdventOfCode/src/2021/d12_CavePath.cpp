#include "2021/d12_CavePath.h"

SOLUTION(2021, 12) {
    //rooms are unique by first letter
    //s and e are special (start and end)
    constexpr std::array<std::string, 'z' + 1> Parse(const std::vector<std::string>&lines) {
        std::array<std::string, 'z' + 1> result{};
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, "-");
            auto lhs = s[0][0];
            auto rhs = s[1][0];
            if (lhs != 'e' && rhs != 's') {
                result[lhs].push_back(rhs);
            }
            if (lhs != 's' && rhs != 'e') {
                result[rhs].push_back(lhs);
            }
        }

        return result;
    }

    template<bool AllowDuplicate>
    constexpr size_t CountPaths(const std::array<std::string, 'z' + 1>& neighbors) {
        struct State { u32 Taken{ 0 }; std::string Path; };
        std::vector<State> current;
        current.push_back({ 0, "s"});

        Constexpr::BigSet<std::string> seen;
        size_t result = 0;
        u8 dupBit = 27;

        State next;
        while (!current.empty()) {
            State c = current.back();
            current.pop_back();
            if (c.Path.back() == 'e') {
                result++;
                continue;
            }
            for (auto n : neighbors[c.Path.back()]) {
                auto nextPath = c.Path + n;
                if (n < 'a') {
                    next = { c.Taken, nextPath };
                    current.push_back(next);
                    continue;
                }
                auto bit = n - 'a';

                if (Constexpr::IsBitSet(c.Taken, bit)) {
                    if (Constexpr::IsBitSet(c.Taken, dupBit) || !AllowDuplicate) continue;
                    next = { Constexpr::WithBit(c.Taken, dupBit), nextPath };
                }
                else {
                    next = { Constexpr::WithBit(c.Taken, bit), nextPath};
                }
                if (seen.insert(next.Path)) {
                    current.push_back(next);
                }
            }
        }

        return result;
    }
    
    PART_ONE() {
        auto n = Parse(lines);
        return Constexpr::ToString(CountPaths<false>(n));
    }

    PART_TWO() {
        auto n = Parse(lines);
        return Constexpr::ToString(CountPaths<true>(n));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "start-A",
            "start-b",
            "A-c",
            "A-b",
            "b-d",
            "A-end",
            "b-end"
        };

        auto n = Parse(lines);
        if (CountPaths<false>(n) != 10) return false;
        if (CountPaths<true>(n) != 36) return false;

        return true;
    }
}