#include "2023/d14_Tilting.h"
#include "Facing.h"

#include "Core/Algorithms/FloydCycle.h"

SOLUTION(2023, 14) {
    constexpr s32 left = -1;
    constexpr s32 right = 1;
    constexpr s32 up = -1;
    constexpr s32 down = 1;
    constexpr s32 same = 0;

    constexpr void Slide(RowCol pos, s32 deltaX, s32 deltaY, std::vector<std::string>&map, RowCol max) {
        if (map[pos.Row][pos.Col] != 'O') return;
        auto next = pos;
        next.Row += deltaY;
        next.Col += deltaX;

        while (map[next.Row][next.Col] == '.') {
            std::swap(map[pos.Row][pos.Col], map[next.Row][next.Col]);
            if (deltaX == -1 && next.Col == 0) break;
            if (deltaY == -1 && next.Row == 0) break;
            if (deltaX == 1 && next.Col == max.Col) break;
            if (deltaY == 1 && next.Row == max.Row) break;

            pos = next;
            next.Row += deltaY;
            next.Col += deltaX;
        }
    }
    constexpr void Tilt(Direction dir, std::vector<std::string>&map, RowCol max) {
        RowCol pos{ 0, 0 };
        if (dir == Direction::North) {
            pos.Row++;
            
            for (size_t row = 1; row < map.size(); row++) {
                for(auto col = 0; col < map[row].size(); col++) {
                    Slide(pos, same, up, map, max);
                    pos.Col++;
                }
                pos.Row++;
                pos.Col = 0;
            }
        }
        else if (dir == Direction::South) {
            pos.Row = map.size() - 2;
            for (auto row = 0; row < map.size() - 1; row++) {
                for (auto col = 0; col < map[row].size(); col++) {
                    Slide(pos, same, down, map, max);
                    pos.Col++;
                }
                pos.Row--;
                pos.Col = 0;
            }
        }
        else if (dir == Direction::East) {
            pos.Col = map[0].size() - 2;
            for (auto col = 0; col < map[0].size() - 1; col++) {
                for (auto row = 0; row < map.size(); row++) {
                    Slide(pos, right, same, map, max);
                    pos.Row++;
                }
                pos.Col--;
                pos.Row = 0;
            }
        }
        else {
            pos.Col = 1;
            for (auto col = 0; col < map[0].size() - 1; col++) {
                for (auto row = 0; row < map.size(); row++) {
                    Slide(pos, left, same, map, max);
                    pos.Row++;
                }
                pos.Col++;
                pos.Row = 0;
            }
        }
    }

    constexpr size_t CalculateLoad(const std::vector<std::string>& map) {
        size_t result = 0;
        size_t weight = map.size();
        for (auto i = 0; i < map.size(); i++) {
            result += std::count(map[i].begin(), map[i].end(), 'O') * weight;
            weight--;
        }
        return result;
    }

    PART_ONE() {
        auto map = lines;
        RowCol max = { map.size() - 1, map[0].size() - 1 };

        Tilt(Direction::North, map, max);

        size_t result = CalculateLoad(map);

        return Constexpr::ToString(result);
    }

    constexpr std::vector<std::string> Cycle(const std::vector<std::string>& map, RowCol max) {
        auto result = map;
        Tilt(Direction::North, result, max);
        Tilt(Direction::West, result, max);
        Tilt(Direction::South, result, max);
        Tilt(Direction::East, result, max);

        return result;
    }


    PART_TWO() {
        auto map = lines;
        RowCol max = { map.size() - 1, map[0].size() - 1 };
        u32 cycleLength;
        u32 cycleStart;

        FloydCycle::FindCycle(map, Cycle, cycleLength, cycleStart, max);

        u32 remaining = (1'000'000'000 - cycleStart) % cycleLength;

        for (auto i = 0u; i < remaining + cycleStart; i++) {
            map = Cycle(map, max);
        }

        return Constexpr::ToString(CalculateLoad(map));
    }

    TESTS() {
        std::vector<std::string> example = {
            "O....#....",
            "O.OO#....#",
            ".....##...",
            "OO.#O....O",
            ".O.....O#.",
            "O.#..O.#.#",
            "..O..#O..O",
            ".......O..",
            "#....###..",
            "#OO..#...."
        };

        if (PartOne(example) != "136") return false;
        return true;
    }
}