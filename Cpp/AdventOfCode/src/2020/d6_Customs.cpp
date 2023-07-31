#include "2020/d6_Customs.h"

SOLUTION(2020, 6) {
    constexpr size_t CountAnyInGroup(const std::vector<std::string>&lines) {
        std::array<bool, 26> seen{};
        for (const auto& line : lines) {
            for (const auto& c : line) {
                seen[c - 'a'] = true;
            }
        }

        return std::count(seen.cbegin(), seen.cend(), true);
    }

    constexpr size_t CountAllInGroup(const std::vector<std::string>&lines) {
        std::array<size_t, 26> seen{};

        for (const auto& line : lines) {
            for (auto c : line) {
                seen[c - 'a']++;
            }
        }

        return std::count(seen.cbegin(), seen.cend(), lines.size());
    }

    constexpr size_t Run(const std::vector<std::string>&lines, auto countFunc) {
        auto groups = SplitInputIntoGroups(lines);
        return std::accumulate(groups.begin(), groups.end(), 0ull, [&](size_t previous, const auto& group) {
            return previous + countFunc(group);
            });
    }

    PART_ONE() {
        return Constexpr::ToString(Run(lines, CountAnyInGroup));
    }

    PART_TWO() {
        return Constexpr::ToString(Run(lines, CountAllInGroup));
    }

    TESTS() {
        static_assert(CountAnyInGroup(std::vector<std::string>{ "a" }) == 1);
        static_assert(CountAnyInGroup(std::vector<std::string>{"a", "b", "c"}) == 3);
        static_assert(CountAnyInGroup(std::vector<std::string>{"a", "a", "a"}) == 1);

        static_assert(CountAllInGroup(std::vector<std::string>{"abc"}) == 3);
        static_assert(CountAllInGroup(std::vector<std::string>{"a", "b", "c"}) == 0);
        static_assert(CountAllInGroup(std::vector<std::string>{"a", "a", "a"}) == 1);

        return true;
    }
}