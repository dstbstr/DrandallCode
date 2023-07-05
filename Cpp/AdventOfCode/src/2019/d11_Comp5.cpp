#include "2019/d11_Comp5.h"
#include "2019/Comp.h"
#include "Facing.h"

SOLUTION(2019, 11) {
    auto GetPainting(std::vector<s64>&instructions, bool startOnBlack) {
        std::unordered_map<Coord, bool> painted;
        Args args;
        args.Inputs.push_back(startOnBlack ? 0 : 1);
        args.Output = 99;

        Facing facing = Up;
        Coord pos = { 0, 0 };
        bool firstOutputRead = false;

        while (Apply(instructions, args)) {
            if (args.Output != 99) {
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
                    if (painted.find(pos) != painted.end()) {
                        onBlack = painted.at(pos);
                    }
                    args.Inputs.push_back(onBlack ? 0 : 1);
                }
                firstOutputRead = !firstOutputRead;
                args.Output = 99;
            }
        }

        return painted;
    }

    auto Part1(const std::string & line) {
        auto instructions = ParseInstructions(line);
        auto painted = GetPainting(instructions, true);

        return painted.size();
    }

    auto Part2(const std::string & line) {
        auto instructions = ParseInstructions(line);
        auto painted = GetPainting(instructions, false);

        Coord min, max;
        GetLimitsFromMap(painted, min, max);
        std::string result;

        for (auto row = min.Y; row <= max.Y; row++) {
            for (auto col = min.X; col <= max.X; col++) {
                Coord pos = { col, row };
                if (painted.find(pos) == painted.end()) {
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

    std::string Run(const std::vector<std::string>&lines) {
        //return PartOne(lines[0]);
        return Part2(lines[0]);
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