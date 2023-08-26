#include "2019/d11_Comp5.h"
#include "2019/Comp.h"
#include "Facing.h"

SOLUTION(2019, 11) {
    constexpr auto GetPainting(std::vector<s64>&instructions, bool startOnBlack) {
        Constexpr::SmallMap<Coord, bool> painted;
        Args args;
        args.Inputs.push_back(startOnBlack ? 0 : 1);
        args.Output = Unset;

        Facing facing = Up;
        Coord pos = { 0, 0 };
        bool firstOutputRead = false;

        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                if (!firstOutputRead) {
                    painted[pos] = args.Output == 0;
                }
                else {
                    if (args.Output == 0) {
                        facing = TurnLeft(facing);
                    }
                    else {
                        facing = TurnRight(facing);
                    }
                    Move(pos, facing);
                    bool onBlack = true;
                    if(painted.contains(pos)) {
                        onBlack = painted.at(pos);
                    }
                    args.Inputs.push_back(onBlack ? 0 : 1);
                }
                firstOutputRead = !firstOutputRead;
                args.Output = Unset;
            }
        }

        return painted;
    }

    PART_ONE() {
        auto instructions = ParseInstructions(Line);
        auto painted = GetPainting(instructions, true);

        return Constexpr::ToString(painted.size());
    }

    PART_TWO() {
        auto instructions = ParseInstructions(Line);
        auto painted = GetPainting(instructions, false);

        Coord min, max;
        GetLimitsFromMap(painted, min, max);
        std::string result = "\n";

        for (auto row = min.Y; row <= max.Y; row++) {
            for (auto col = min.X; col <= max.X; col++) {
                Coord pos = { col, row };
                if (!painted.contains(pos)) {
                    result.push_back(' ');
                }
                else {
                    result.push_back(painted.at(pos) ? '.' : '#');
                }
            }
            result.push_back('\n');
        }

        return result;
    }

    TESTS() {
        return true;
    }
}