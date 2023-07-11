#include "2017/d22_Infecting.h"

SOLUTION(2017, 22) {
    constexpr size_t Solve(const std::vector<std::string>& lines, size_t iterations, auto MoveFunc) {
        constexpr const size_t mapSize = 10'001;
        std::vector<std::vector<char>> map(mapSize);
        for (auto& m : map) {
            m.resize(mapSize, '.');
        }

        size_t x = mapSize / 2;
        size_t y = x;
        size_t dx(x - lines.size() / 2);
        size_t dy = dx;

        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                map[row + dy][col + dx] = lines[row][col];
            }
        }

        size_t dir = 0;
        size_t result = 0;

        for (size_t step = 0; step < iterations; step++) {
            result += MoveFunc(map[y][x], dir);
            switch (dir) {
            case 0: y--; break;
            case 1: x++; break;
            case 2: y++; break;
            case 3: x--; break;
            }
        }

        return result;
    }

    PART_ONE() {
        auto result = Solve(lines, 10'000, [](char& currentPos, size_t& dir) {
            switch (currentPos) {
            case '.': dir = (dir + 3) % 4; currentPos = '#'; return 1;
            case '#': dir = (dir + 1) % 4; currentPos = '.'; return 0;
            }
            return 0;
        });

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto result = Solve(lines, 10'000'000, [](char& currentPos, size_t& dir) {
            switch (currentPos) {
            case '.': dir = (dir + 3) % 4; currentPos = 'W'; return 0;
            case 'W': currentPos = '#'; return 1;
            case '#': dir = (dir + 1) % 4; currentPos = 'F'; return 0;
            case 'F': dir = (dir + 2) % 4; currentPos = '.'; return 0;
            }
            return 0;
        });

        return Constexpr::ToString(result);

    }

    TESTS() {
        return true;
    }
}