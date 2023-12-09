#include "2020/d12_RainRisk.h"
#include "Facing.h"

SOLUTION(2020, 12) {
    enum struct Cmd { North, East, South, West, Left, Right, Forward };

    struct Instruction {
        Cmd Op = Cmd::North;
        s32 Value = 0;
    };

    constexpr Instruction ParseInstruction(std::string_view line) {
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

    constexpr void TurnLeft(Coord& in) {
        std::swap(in.X, in.Y);
        in.Y = -in.Y;
    }

    constexpr void TurnRight(Coord& in) {
        std::swap(in.X, in.Y);
        in.X = -in.X;
    }

    constexpr void TurnAround(Coord& in) {
        std::swap(in.X, in.Y);
        in.X = -in.X;
        in.Y = -in.Y;
    }

    constexpr void Apply(const Instruction& instruction, Coord& pos, Coord& wayPoint, Coord& toUpdate) {
        auto val = instruction.Value;
        switch (instruction.Op) {
        case Cmd::North: toUpdate.Y -= val; return;
        case Cmd::South: toUpdate.Y += val; return;
        case Cmd::East: toUpdate.X += val; return;
        case Cmd::West: toUpdate.X -= val; return;
        }

        if (instruction.Op == Cmd::Forward) {
            pos += (wayPoint * val);
            return;
        }

        if (instruction.Op == Cmd::Left) {
            switch (val) {
            case 90: TurnLeft(wayPoint); return;
            case 180: TurnAround(wayPoint); return;
            case 270: TurnRight(wayPoint); return;
            }
        }

        if (instruction.Op == Cmd::Right) {
            switch (val) {
            case 90: TurnRight(wayPoint); return;
            case 180: TurnAround(wayPoint); return;
            case 270: TurnLeft(wayPoint); return;
            }
        }

        throw "Unhandled instruction op";
    }

    constexpr size_t Solve(const auto& lines, Coord& wayPoint, Coord& pos, Coord& toUpdate) {
        const auto instructions = ParseLines(lines, ParseInstruction);
        for (const auto& inst : instructions) {
            Apply(inst, pos, wayPoint, toUpdate);
        }
        return MDistance(pos);
    }

    PART_ONE() {
        Coord wayPoint{ 1, 0 };
        Coord pos{ 0, 0 };
        auto result = Solve(lines, wayPoint, pos, pos);
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        Coord wayPoint{ 10, -1 };
        Coord pos{ 0, 0 };
        auto result = Solve(lines, wayPoint, pos, wayPoint);
        return Constexpr::ToString(result);
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

    TESTS() {
        static_assert(TestTurns());
        if (!TestTurns()) return false;

        std::vector<std::string> lines = {
            "F10",
            "N3",
            "F7",
            "R90",
            "F11"
        };
        Coord wayPoint{ 1,0 };
        Coord pos{ 0, 0 };
        if (Solve(lines, wayPoint, pos, pos) != 25) return false;

        pos = { 0, 0 };
        wayPoint = { 10, -1 };
        if (Solve(lines, wayPoint, pos, wayPoint) != 286) return false;

        return true;
    }
}