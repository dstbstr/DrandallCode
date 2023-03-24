#pragma once

#include "Common.h"

enum struct Cmd {North, East, South, West, Left, Right, Forward};

struct Instruction {
    Cmd Op = Cmd::North;
    s32 Value = 0;
};

constexpr Instruction ParseInstruction(const std::string& line) {
    auto c = line[0];
    Cmd command;
    switch (c) {
    case 'N': command = Cmd::North; break;
    case 'S': command = Cmd::South; break;
    case 'E': command = Cmd::East; break;
    case 'W': command = Cmd::West; break;
    case 'L': command = Cmd::Left; break;
    case 'R': command = Cmd::Right; break;
    case 'F': command = Cmd::Forward; break;
    default: throw "Unrecognized command";
    }

    s32 value;
    Constexpr::ParseNumber(line.substr(1), value);

    return { command, value };
}

constexpr std::vector<Instruction> ParseInput(const std::vector<std::string>& lines) {
    std::vector<Instruction> result;
    for (const auto& line : lines) {
        result.push_back(ParseInstruction(line));
    }
    return result;
}

constexpr void ApplyInstruction(const Instruction& instruction, Direction& facing, Coord& pos) {
    auto val = instruction.Value;
    switch (instruction.Op) {
    case Cmd::North: pos.Y -= val; return;
    case Cmd::South: pos.Y += val; return;
    case Cmd::East: pos.X += val; return;
    case Cmd::West: pos.X -= val; return;
    }

    if (instruction.Op == Cmd::Forward) {
        switch (facing) {
        case Direction::Up: pos.Y -= val; return;
        case Direction::Down: pos.Y += val; return;
        case Direction::Left: pos.X -= val; return;
        case Direction::Right: pos.X += val; return;
        }
    }
    
    if (instruction.Op == Cmd::Left) {
        switch (val) {
        case 90: facing = TurnLeft(facing); return;
        case 180: facing = TurnAround(facing); return;
        case 270: facing = TurnRight(facing); return;
        }
    }

    if (instruction.Op == Cmd::Right) {
        switch (val) {
        case 90: facing = TurnRight(facing); return;
        case 180: facing = TurnAround(facing); return;
        case 270: facing = TurnLeft(facing); return;
        }
    }

    throw "Unhandled instruction op";
}

auto PartOne(const std::vector<std::string>& lines) {
    Direction facing = Direction::Right;
    Coord pos = { 0, 0 };
    const auto instructions = ParseInput(lines);

    for (const auto& instruction : instructions) {
        ApplyInstruction(instruction, facing, pos);
    }

    return MDistance(pos, Coord{ 0, 0 });
}

constexpr void TurnLeft(Coord& wayPoint) {
    std::swap(wayPoint.X, wayPoint.Y);
    wayPoint.Y = -wayPoint.Y;
}

constexpr void TurnRight(Coord& wayPoint) {
    std::swap(wayPoint.X, wayPoint.Y);
    wayPoint.X = -wayPoint.X;
}

constexpr bool TestTurns() {
    Coord pos = { 1, 3 };
    TurnRight(pos);
    if (pos != Coord{-3, 1}) return false;
    TurnRight(pos);
    if (pos != Coord{ -1, -3 }) return false;
    TurnRight(pos);
    if (pos != Coord{ 3, -1 }) return false;
    TurnRight(pos);
    if (pos != Coord{ 1, 3 }) return false;

    return true;
}

static_assert(TestTurns());

constexpr void ApplyInstruction2(const Instruction& instruction, Coord& pos, Coord& wayPoint) {
    auto val = instruction.Value;
    switch (instruction.Op) {
    case Cmd::North: wayPoint.Y -= val; return;
    case Cmd::South: wayPoint.Y += val; return;
    case Cmd::West: wayPoint.X -= val; return;
    case Cmd::East: wayPoint.X += val; return;
    }

    if (instruction.Op == Cmd::Forward) {
        pos.Y += (wayPoint.Y * val);
        pos.X += (wayPoint.X * val);
        return;
    }

    if (val == 180) {
        wayPoint.X = -wayPoint.X;
        wayPoint.Y = -wayPoint.Y;
        return;
    }

    if (instruction.Op == Cmd::Left) {
        switch (val) {
        case 90: TurnLeft(wayPoint); return;
        case 270: TurnRight(wayPoint); return;
        }
    }

    if (instruction.Op == Cmd::Right) {
        switch (val) {
        case 90: TurnRight(wayPoint); return;
        case 270: TurnLeft(wayPoint); return;
        }
    }

    throw "Unhandled instruction op";
}

auto PartTwo(const std::vector<std::string>& lines) {
    Coord pos = { 0, 0 };
    Coord wayPoint = { 10, -1 };
    const auto instructions = ParseInput(lines);

    for (const auto& instruction : instructions) {
        ApplyInstruction2(instruction, pos, wayPoint);
    }

    return MDistance(pos, { 0, 0 });
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    if (!TestTurns()) return false;

    std::vector<std::string> lines = {
        "F10",
        "N3",
        "F7",
        "R90",
        "F11"
    };

    if (PartOne(lines) != 25) return false;
    if (PartTwo(lines) != 286) return false;

    return true;
}