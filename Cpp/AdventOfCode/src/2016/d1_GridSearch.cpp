#include "2016/d1_GridSearch.h"
#include "Facing.h"

SOLUTION(2016, 1) {
    constexpr size_t FindTarget(std::string_view directions) {
        auto split = Constexpr::Split(directions, ", ");
        Direction facing = Direction::North;
        u32 amount;
        Coord target = { 0, 0 };

        for (auto instruction : split) {
            facing = instruction[0] == 'L' ? TurnLeft(facing) : TurnRight(facing);
            Constexpr::ParseNumber(instruction.substr(1), amount);
            Move(target, facing, amount);
        }

        return MDistance(target, Coord{ 0, 0 });
    }

    constexpr size_t FindTarget2(std::string_view directions) {
        Constexpr::SmallSet<Coord> seen;
        Direction facing = North;
        Coord pos{ 0, 0 };
        u32 distance;

        auto steps = Constexpr::Split(directions, ", ");
        for (const auto& step : steps) {
            facing = step[0] == 'L' ? TurnLeft(facing) : TurnRight(facing);
            Constexpr::ParseNumber(step.substr(1), distance);
            for (u32 i = 0; i < distance; i++) {
                Move(pos, facing);
                if (seen.contains(pos)) return MDistance(pos, Coord{ 0, 0 });
                seen.insert(pos);
            }
        }
        return 0;
    }

    PART_ONE() {
        return Constexpr::ToString(FindTarget(lines[0]));
    }
    PART_TWO() {
        return Constexpr::ToString(FindTarget2(lines[0]));
    }

    TESTS() {
        if (FindTarget("R2, L3") != 5) return false;
        if (FindTarget("R2, R2, R2") != 2) return false;
        if (FindTarget("R5, L5, R5, R3") != 12) return false;

        if (FindTarget2("R8, R4, R4, R8") != 4) return false;
        
        return true;
    }
}
