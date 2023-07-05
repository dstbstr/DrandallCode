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

    static_assert(CountAnyInGroup(std::vector<std::string>{ "a" }) == 1);
    static_assert(CountAnyInGroup(std::vector<std::string>{"a", "b", "c"}) == 3);
    static_assert(CountAnyInGroup(std::vector<std::string>{"a", "a", "a"}) == 1);

    constexpr size_t CountAllInGroup(const std::vector<std::string>&lines) {
        std::array<size_t, 26> seen{};
        seen.fill(0);

        for (const auto& line : lines) {
            for (auto c : line) {
                seen[c - 'a']++;
            }
        }

        return std::count(seen.cbegin(), seen.cend(), lines.size());
    }

    static_assert(CountAllInGroup(std::vector<std::string>{"abc"}) == 3);
    static_assert(CountAllInGroup(std::vector<std::string>{"a", "b", "c"}) == 0);
    static_assert(CountAllInGroup(std::vector<std::string>{"a", "a", "a"}) == 1);

    template<typename CountFunc>
    constexpr size_t Run(const std::vector<std::string>&lines, CountFunc countFunc) {
        std::vector<std::string> gLines;
        size_t sum = 0;
        for (const auto& line : lines) {
            if (line.empty()) {
                sum += countFunc(gLines);
                gLines.clear();
            }
            else {
                gLines.push_back(line);
            }
        }
        sum += countFunc(gLines);
        return sum;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Run(lines, CountAnyInGroup));
        return Constexpr::ToString(Run(lines, CountAllInGroup));
    }

    bool RunTests() {
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