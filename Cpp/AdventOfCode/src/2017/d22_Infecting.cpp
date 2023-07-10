#include "2017/d22_Infecting.h"
#include "Facing.h"

SOLUTION(2017, 22) {
    /*
    std::unordered_set<Coord> ParseInput(const std::vector<std::string>&lines) {
        std::unordered_set<Coord> result;
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.insert({ static_cast<s32>(col), static_cast<s32>(row) });
                }
            }
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines, u32 iterations) {
        auto infected = ParseInput(lines);
        u32 count = 0;
        Facing facing = Up;
        Coord currentPos;
        currentPos.Y = static_cast<s32>(lines.size() / 2);
        currentPos.X = static_cast<s32>(lines[0].size() / 2);

        for (u32 i = 0; i < iterations; i++) {
            if (infected.find(currentPos) != infected.end()) {
                facing = TurnRight(facing);
                infected.erase(currentPos);
            }
            else {
                facing = TurnLeft(facing);
                infected.insert(currentPos);
                count++;
            }

            Move(currentPos, facing);
        }

        return count;
    }

    auto Part2(const std::vector<std::string>&lines, u32 iterations) {
        auto infected = ParseInput(lines);
        std::unordered_set<Coord> weakened{};
        std::unordered_set<Coord> flagged{};
        Facing facing = Up;
        Coord currentPos;
        currentPos.Y = static_cast<s32>(lines.size() / 2);
        currentPos.X = static_cast<s32>(lines[0].size() / 2);

        u32 count = 0;

        for (u32 i = 0; i < iterations; i++) {
            if (infected.find(currentPos) != infected.end()) {
                facing = TurnRight(facing);
                infected.erase(currentPos);
                flagged.insert(currentPos);
            }
            else if (weakened.find(currentPos) != weakened.end()) {
                weakened.erase(currentPos);
                infected.insert(currentPos);
                count++;
            }
            else if (flagged.find(currentPos) != flagged.end()) {
                facing = TurnAround(facing);
                flagged.erase(currentPos);
            }
            else {
                facing = TurnLeft(facing);
                weakened.insert(currentPos);
            }

            Move(currentPos, facing);
        }

        return count;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 10000));
        return Constexpr::ToString(Part2(lines, 10'000'000));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "..#",
            "#..",
            "..."
        };

        if (Part1(lines, 70) != 41) return false;
        if (Part1(lines, 10000) != 5587) return false;
        if (Part2(lines, 100) != 26) return false;
        return true;
    }
    */

constexpr Constexpr::Set<Coord> ParseInput(const std::vector<std::string>& lines) {
    Constexpr::Set<Coord> result;
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            if (lines[row][col] == '#') {
                result.insert({ static_cast<s32>(col), static_cast<s32>(row) });
            }
        }
    }

    return result;
}

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
    /*
    auto infected = ParseInput(lines);
    u32 count = 0;
    Facing facing = Up;
    Coord currentPos;
    currentPos.Y = static_cast<s32>(lines.size() / 2);
    currentPos.X = static_cast<s32>(lines[0].size() / 2);

    for (u32 i = 0; i < 10'000; i++) {
        if (infected.contains(currentPos)) {
            facing = TurnRight(facing);
            infected.erase(currentPos);
        }
        else {
            facing = TurnLeft(facing);
            infected.insert(currentPos);
            count++;
        }

        Move(currentPos, facing);
    }

    return Constexpr::ToString(count);
    */
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

    /*
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

    for (size_t step = 0; step < 10'000'000; step++) {
        switch(map[y][x]) {
        case '.': dir = (dir + 3) % 4; map[y][x] = 'W'; break;
        case 'W': map[y][x] = '#'; ++result; break;
        case '#': dir = (dir + 1) % 4; map[y][x] = 'F'; break;
        case 'F': dir = (dir + 2) % 4; map[y][x] = '.'; break;
        }
        switch (dir) {
        case 0: y--; break;
        case 1: x++; break;
        case 2: y++; break;
        case 3: x--; break;
        }
    }

    return Constexpr::ToString(result);
    */
    }

    TESTS() {
        return true;
    }
}