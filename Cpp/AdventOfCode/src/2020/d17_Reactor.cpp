#include "2020/d17_Reactor.h"

SOLUTION(2020, 17) {
    using Reactor = Constexpr::SmallSet<Vec3<s32>>;
    using BigReactor = Constexpr::SmallSet<Vec4<s32>>;
    using Neighbors = Constexpr::SmallMap<Vec3<s32>, size_t>;
    using BigNeighbors = Constexpr::SmallMap<Vec4<s32>, size_t>;

    constexpr auto Vec3Deltas = []() {
        std::array<Vec3<s32>, 26> result;
        size_t index = 0;
        Vec3<s32> origin = { 0, 0, 0 };
        for (s32 x = -1; x <= 1; x++) {
            for (s32 y = -1; y <= 1; y++) {
                for (s32 z = -1; z <= 1; z++) {
                    Vec3<s32> pos = { x, y, z };
                    if (pos == origin) continue;
                    result[index++] = pos;
                }
            }
        }
        return result;
    }();

    constexpr auto Vec4Deltas = []() {
        std::array<Vec4<s32>, 80> result{};
        size_t index = 0;
        Vec4<s32> origin = { 0, 0, 0, 0 };
        for (s32 x = -1; x <= 1; x++) {
            for (s32 y = -1; y <= 1; y++) {
                for (s32 z = -1; z <= 1; z++) {
                    for (s32 w = -1; w <= 1; w++) {
                        Vec4<s32> pos = { x, y, z, w };
                        if (pos == origin) continue;
                        result[index++] = pos;
                    }
                }
            }
        }
        return result;
    }();

    constexpr Reactor ParseReactor(const auto& lines) {
        Reactor result;
        for (s32 row = 0; row < lines.size(); row++) {
            for (s32 col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.insert({ col, row, 0 });
                }
            }
        }

        return result;
    }

    constexpr BigReactor ParseBigReactor(const auto& lines) {
        BigReactor result;
        for (s32 row = 0; row < lines.size(); row++) {
            for (s32 col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.insert({ col, row, 0, 0 });
                }
            }
        }

        return result;
    }

    constexpr void UpdateNeighbors(const Reactor& reactor, Neighbors& neighbors) {
        for (const auto& pos : reactor) {
            for (const auto& delta: Vec3Deltas) {
                neighbors[pos + delta]++;
            }
        }
    }
    
    constexpr void UpdateNeighbors(const BigReactor& reactor, BigNeighbors& neighbors) {
        for (const auto& pos : reactor) {
            for (const auto& delta : Vec4Deltas) {
                neighbors[pos + delta]++;
            }
        }
    }

    constexpr Reactor Next(const Reactor& current) {
        Neighbors neighbors;
        UpdateNeighbors(current, neighbors);
        Reactor next;
        for (const auto& [pos, count] : neighbors) {
            if (count == 3 || (count == 2 && current.contains(pos))) {
                next.insert(pos);
            }
        }

        return next;
    }

    constexpr BigReactor Next(const BigReactor& current) {
        BigNeighbors neighbors;
        UpdateNeighbors(current, neighbors);
        BigReactor next;
        for (const auto& [pos, count] : neighbors) {
            if (count == 3 || (count == 2 && current.contains(pos))) {
                next.insert(pos);
            }
        }

        return next;
    }

    constexpr size_t Solve(auto reactor) {
        for (auto i = 0; i < 6; i++) {
            reactor = Next(reactor);
        }
        return reactor.size();
    }

    PART_ONE() {
        auto reactor = ParseReactor(Lines);
        return Constexpr::ToString(Solve(reactor));
    }

    PART_TWO() {
        auto reactor = ParseBigReactor(Lines);
        return Constexpr::ToString(Solve(reactor));
    }

    TESTS() {
        std::vector<std::string> lines = {
           ".#.",
           "..#",
           "###"
        };

        if (Solve(ParseReactor(lines)) != 112) return false;
        if (Solve(ParseBigReactor(lines)) != 848) return false;
        return true;
    }
}