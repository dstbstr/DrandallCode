#pragma once

#include "Common.h"
#include "Comp.h"

auto GetPainting(std::vector<s64>& instructions, bool startOnBlack) {
    std::unordered_map<Coord, bool, CoordHash> painted;
    Args args;
    args.Inputs.push_back(startOnBlack ? 0 : 1);
    args.Output = 99;

    Direction facing = Up;
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

auto PartOne(const std::string& line) {
    auto instructions = ParseInstructions(line);
    auto painted = GetPainting(instructions, true);

    return painted.size();
}

auto PartTwo(const std::string& line) {
    auto instructions = ParseInstructions(line);
    auto painted = GetPainting(instructions, false);

    Coord min, max;
    GetLimitsFromMap(painted, min, max);
    std::string result;
    
    for (s32 row = min.Y; row <= max.Y; row++) {
        for (s32 col = min.X; col <= max.X; col++) {
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

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    return true;
}