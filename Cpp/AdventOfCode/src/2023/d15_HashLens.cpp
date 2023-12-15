#include "2023/d15_HashLens.h"

SOLUTION(2023, 15) {
    constexpr u32 Hash(std::string_view input) {
        u32 current = 0;
        for (auto c : input) {
            current = ((current + c) * 17) % 256;
        }
        return current;
    }

    PART_ONE() {
        auto v = Constexpr::Split(lines[0], ",");
        auto result = std::accumulate(v.begin(), v.end(), 0ull, [](size_t running, std::string_view sv) {
            return running + Hash(sv);
            });

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto v = Constexpr::Split(lines[0], ",");
        std::array<std::vector<std::pair<std::string_view, u32>>, 256> map{};
        for (auto value : v) {
            std::string_view label;
            u32 num = 0;
            if (value.back() == '-') {
                label = value.substr(0, value.size() - 1);
            }
            else {
                auto s = Constexpr::Split(value, "=");
                label = s[0];
                Constexpr::ParseNumber(s[1], num);
            }

            auto pos = Hash(label);
            auto& box = map[pos];
            size_t match = 0;

            for (auto [lens, _] : box) {
                if (lens == label) {
                    break;
                }
                match++;
            }

            if (num == 0) {
                if (match < box.size()) {
                    std::rotate(box.begin(), box.begin() + match + 1, box.end());
                    box.pop_back();
                    std::rotate(box.rbegin(), box.rbegin() + match, box.rend());
                }
            }
            else {
                if (match < box.size()) {
                    box[match].second = num;
                }
                else {
                    box.push_back(std::make_pair(label, num));
                }
            }
        }

        size_t result = 0;
        for (auto box = 0; box < map.size(); box++) {
            for (auto slot = 0; slot < map[box].size(); slot++) {
                auto [label, focus] = map[box][slot];
                result += (box + 1) * (slot + 1) * focus;
            }
        }
        return Constexpr::ToString(result);
    }

    TESTS() {
        static_assert(Hash("HASH") == 52);

        std::vector<std::string> example = { "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7" };
        if (PartTwo(example) != "145") return false;

        return true;
    }
}