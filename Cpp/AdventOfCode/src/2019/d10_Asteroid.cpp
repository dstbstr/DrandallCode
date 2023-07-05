#include "2019/d10_Asteroid.h"

SOLUTION(2019, 10) {
    using Map = std::vector<Coord>;
    using VisibilityMap = std::unordered_map<Coord, std::unordered_map<Coord, bool>>;

    constexpr Map ParseMap(const std::vector<std::string>&lines) {
        Map result;
        for (s32 row = 0; row < lines.size(); row++) {
            for (s32 col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.push_back({ col, row });
                }
            }
        }
        return result;
    }

    constexpr u32 CountVisible(const Coord & pos, const Map & map) {
        u32 result = 0;
        for (const auto& end : map) {
            if (end == pos) continue;
            bool visible = true;
            for (const auto& other : map) {
                if (other == pos || other == end) continue;
                if (Constexpr::DoIntersect(pos, end, other)) {
                    visible = false;
                    break;
                }
            }
            result += visible;
        }

        return result;
    }

    VisibilityMap BuildVisibilityMap(const Map & map) {
        VisibilityMap result;

        for (const auto& start : map) {
            for (const auto& end : map) {
                if (start == end) continue;
                if (result[start].contains(end)) continue;
                bool visible = true;
                for (const auto& third : map) {
                    if (third == start || third == end) continue;
                    if (Constexpr::DoIntersect(start, end, third)) {
                        visible = false;
                        break;
                    }
                }

                result[start][end] = visible;
                result[end][start] = visible;
            }
        }

        return result;
    }

#include <iostream>
    void PrintVisibilityMap(const VisibilityMap & map) {
        Coord min, max;
        std::vector<Coord> coords;
        for (const auto& [from, to] : map) {
            coords.push_back(from);
        }
        GetLimits(coords.begin(), coords.end(), min, max);

        for (s32 row = 0; row <= max.Y; row++) {
            for (s32 col = 0; col <= max.X; col++) {
                Coord pos = { col, row };
                if (map.contains(pos)) {
                    auto count = std::count_if(map.at(pos).begin(), map.at(pos).end(), [](const auto& kvp) {
                        return kvp.second;
                        });
                    std::cout << count;
                }
                else {
                    std::cout << '.';
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    auto CountVisible(const VisibilityMap & map, const Coord & pos) {
        return static_cast<u32>(std::count_if(map.at(pos).cbegin(), map.at(pos).cend(), [](const auto& kvp) { return kvp.second; }));
    }

    auto FindBase(const Map & map, const VisibilityMap & visibility) {
        auto countVisible = [](const auto& kvp) { return kvp.second; };
        u32 best = 0;
        Coord bestPos = { 0, 0 };
        for (const auto& pos : map) {
            auto count = CountVisible(visibility, pos);
            if (best < count) {
                best = count;
                bestPos = pos;
            }
        }

        return bestPos;
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto map = ParseMap(lines);
        auto visibility = BuildVisibilityMap(map);

        auto base = FindBase(map, visibility);
        return CountVisible(visibility, base);
    }

    void PrintAsteroids(const std::vector<Coord>&coords, Coord base) {
        Coord min, max;
        GetLimits(coords.begin(), coords.end(), min, max);

        for (s32 row = 0; row <= max.Y; row++) {
            for (s32 col = 0; col <= max.X; col++) {
                Coord pos = { col, row };
                if (pos == base) {
                    std::cout << " BAS ";
                    continue;
                }
                auto where = std::find(coords.begin(), coords.end(), pos);
                if (where == coords.end()) {
                    std::cout << " .  ";
                }
                else {
                    auto distance = static_cast<u32>(where - coords.begin());
                    auto out = StrUtil::Format("%03d ", distance);
                    std::cout << out;
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    auto Part2(const std::vector<std::string>&lines) {
        auto map = ParseMap(lines);
        auto visibility = BuildVisibilityMap(map);
        auto base = FindBase(map, visibility);
        u32 destroyedCount = 0;

        Coord target{ 0, 0 };
        while (!map.empty()) {
            std::vector<Coord> asteroids;
            std::copy_if(map.begin(), map.end(), std::back_inserter(asteroids), [&](Coord pos) {
                return visibility[base][pos];
                });
            auto count = static_cast<u32>(asteroids.size());
            if (destroyedCount + count < 200) {
                destroyedCount += count;
                std::erase_if(map, [&asteroids](const Coord& pos) { return std::find(asteroids.begin(), asteroids.end(), pos) != asteroids.end(); });
                visibility = BuildVisibilityMap(map);
            }
            else {
                std::sort(asteroids.begin(), asteroids.end(), [&](const Coord& lhs, const Coord& rhs) {
                    return Constexpr::GetOrientation(base, lhs, rhs) == Constexpr::Orientation::CounterClockwise;
                    });

                //PrintAsteroids(asteroids, base);

                size_t midpoint = 0;
                for (size_t i = 0; i < asteroids.size(); i++) {
                    if (asteroids[i].X >= base.X && asteroids[i].Y < base.Y) {
                        midpoint = i;
                        break;
                    }
                }

                std::rotate(asteroids.begin(), asteroids.begin() + midpoint, asteroids.end());
                //PrintAsteroids(asteroids, base);

                target = asteroids[199 - destroyedCount];
                break;
            }
        }

        return (target.X * 100) + target.Y;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            ".#..##.###...#######",
            "##.############..##.",
            ".#.######.########.#",
            ".###.#######.####.#.",
            "#####.##.#.##.###.##",
            "..#####..#.#########",
            "####################",
            "#.####....###.#.#.##",
            "##.#################",
            "#####.##.###..####..",
            "..######..##.#######",
            "####.##.####...##..#",
            ".#####..#.######.###",
            "##...#.##########...",
            "#.##########.#######",
            ".####.#.###.###.#.##",
            "....##.##.###..#####",
            ".#.#.###########.###",
            "#.#.#.#####.####.###",
            "###.##.####.##.#..##"
        };

        if (Part2(lines) != 802) return false;
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