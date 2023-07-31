#include "2019/d19_Comp8.h"
#include "2019/Comp.h"

SOLUTION(2019, 19) {
    constexpr bool IsInBeam(const std::vector<s64>&instructions, UCoord pos) {
        auto running = instructions;
        Args args;
        args.Inputs.push_back(pos.X);
        args.Inputs.push_back(pos.Y);

        while (Apply(running, args));

        return args.Output == 1;
    }

    /*
    auto Part1(const std::string & line) {
        const auto instructions = ParseInstructions(line);
        u32 result = 0;

        for (u32 x = 0; x < 50; x++) {
            for (u32 y = 0; y < 50; y++) {
                UCoord pos = { x, y };
                result += IsInBeam(instructions, pos);
            }
        }

        return result;
    }
    */

    constexpr bool CanContainBox(const std::vector<s64>&instructions, size_t row, size_t col, u32 boxSize) {
        size_t bottomLeft = col;
        while (!IsInBeam(instructions, { bottomLeft++, row + boxSize }));
        bottomLeft--;
        return IsInBeam(instructions, { bottomLeft + boxSize, row });
    }

    constexpr UCoord FindRoughPosition(const std::vector<s64>&instructions, u32 boxSize) {
        u32 row = boxSize;
        u32 col = boxSize / 2;
        u32 searchSpace = boxSize;

        while (searchSpace > 0) {
            while (true) {
                while (!IsInBeam(instructions, { col++, row }));
                col--;
                if (IsInBeam(instructions, { col + boxSize, row })) {
                    if (CanContainBox(instructions, row, col, boxSize)) break;
                }
                row += searchSpace;
            }
            row -= searchSpace;
            col -= boxSize;
            searchSpace /= 2;
        }
        row++;
        while (!IsInBeam(instructions, { col++, row }));
        return { col - 1, row };
    }

    constexpr UCoord FineTune(const std::vector<s64>&instructions, const UCoord & pos, u32 boxSize) {
        for (auto row = pos.Y; row < pos.Y + boxSize; row++) {
            for (auto col = pos.X; col < pos.X + boxSize; col++) {
                if (CanContainBox(instructions, row, col, boxSize)) {
                    auto bottomX = col;
                    while (!IsInBeam(instructions, { bottomX++, row + boxSize }));
                    return { bottomX - 1, row };
                }
            }
        }

        return { 0, 0 };
    }

    constexpr bool Validate(const std::vector<s64>&instructions, const UCoord & pos, u32 boxSize) {

        return IsInBeam(instructions, pos) &&
            IsInBeam(instructions, { pos.X + boxSize, pos.Y }) &&
            IsInBeam(instructions, { pos.X, pos.Y + boxSize }) &&
            IsInBeam(instructions, { pos.X + boxSize, pos.Y + boxSize });
    }

    /*
    auto Part2(const std::string & line) {
        const auto instructions = ParseInstructions(line);

        auto roughPos = FindRoughPosition(instructions, 99);
        auto finalPos = FineTune(instructions, roughPos, 99);
        if (!Validate(instructions, finalPos, 99)) {
            return 0ull;
        }

        return (finalPos.X * 10000) + finalPos.Y;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }
    */

    bool RunTests() {
        std::vector<s64> instructions = { 109,424,203,1,21101,0,11,0,1105,1,282,21101,0,18,0,1106,0,259,2102,1,1,221,203,1,21102,31,1,0,1105,1,282,21101,38,0,0,1105,1,259,21001,23,0,2,21201,1,0,3,21101,0,1,1,21101,0,57,0,1106,0,303,1202,1,1,222,20102,1,221,3,21002,221,1,2,21101,259,0,1,21102,80,1,0,1106,0,225,21101,0,189,2,21102,91,1,0,1105,1,303,2102,1,1,223,20101,0,222,4,21102,259,1,3,21101,225,0,2,21102,225,1,1,21102,1,118,0,1105,1,225,21001,222,0,3,21102,1,57,2,21102,1,133,0,1106,0,303,21202,1,-1,1,22001,223,1,1,21102,148,1,0,1106,0,259,1202,1,1,223,21001,221,0,4,20101,0,222,3,21101,0,24,2,1001,132,-2,224,1002,224,2,224,1001,224,3,224,1002,132,-1,132,1,224,132,224,21001,224,1,1,21101,195,0,0,106,0,108,20207,1,223,2,20102,1,23,1,21102,-1,1,3,21101,0,214,0,1106,0,303,22101,1,1,1,204,1,99,0,0,0,0,109,5,1201,-4,0,249,22101,0,-3,1,22101,0,-2,2,22102,1,-1,3,21102,250,1,0,1106,0,225,22101,0,1,-4,109,-5,2106,0,0,109,3,22107,0,-2,-1,21202,-1,2,-1,21201,-1,-1,-1,22202,-1,-2,-2,109,-3,2106,0,0,109,3,21207,-2,0,-1,1206,-1,294,104,0,99,21201,-2,0,-2,109,-3,2105,1,0,109,5,22207,-3,-4,-1,1206,-1,346,22201,-4,-3,-4,21202,-3,-1,-1,22201,-4,-1,2,21202,2,-1,-1,22201,-4,-1,1,21201,-2,0,3,21102,343,1,0,1105,1,303,1105,1,415,22207,-2,-3,-1,1206,-1,387,22201,-3,-2,-3,21202,-2,-1,-1,22201,-3,-1,3,21202,3,-1,-1,22201,-3,-1,2,21201,-4,0,1,21101,384,0,0,1106,0,303,1106,0,415,21202,-4,-1,-4,22201,-4,-3,-4,22202,-3,-2,-2,22202,-2,-4,-4,22202,-3,-2,-3,21202,-4,-1,-2,22201,-3,-2,1,22102,1,1,-4,109,-5,2105,1,0 };

        UCoord known = { 65, 122 };
        if (!Validate(instructions, known, 10)) return false;

        auto pos = FindRoughPosition(instructions, 10);
        auto finalPos = FineTune(instructions, pos, 10);

        if (!Validate(instructions, finalPos, 10)) return false;
        if (finalPos != known) return false;
        return true;
    }

    PART_ONE() {
        const auto instructions = ParseInstructions(lines[0]);
        u32 result = 0;

        for (u32 x = 0; x < 50; x++) {
            for (u32 y = 0; y < 50; y++) {
                UCoord pos = { x, y };
                result += IsInBeam(instructions, pos);
            }
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        const auto instructions = ParseInstructions(lines[0]);

        auto roughPos = FindRoughPosition(instructions, 99);
        auto finalPos = FineTune(instructions, roughPos, 99);
        if (!Validate(instructions, finalPos, 99)) {
            return "Not Found";
        }

        return Constexpr::ToString((finalPos.X * 10000) + finalPos.Y);
    }

    TESTS() {
        std::vector<s64> instructions = { 109,424,203,1,21101,0,11,0,1105,1,282,21101,0,18,0,1106,0,259,2102,1,1,221,203,1,21102,31,1,0,1105,1,282,21101,38,0,0,1105,1,259,21001,23,0,2,21201,1,0,3,21101,0,1,1,21101,0,57,0,1106,0,303,1202,1,1,222,20102,1,221,3,21002,221,1,2,21101,259,0,1,21102,80,1,0,1106,0,225,21101,0,189,2,21102,91,1,0,1105,1,303,2102,1,1,223,20101,0,222,4,21102,259,1,3,21101,225,0,2,21102,225,1,1,21102,1,118,0,1105,1,225,21001,222,0,3,21102,1,57,2,21102,1,133,0,1106,0,303,21202,1,-1,1,22001,223,1,1,21102,148,1,0,1106,0,259,1202,1,1,223,21001,221,0,4,20101,0,222,3,21101,0,24,2,1001,132,-2,224,1002,224,2,224,1001,224,3,224,1002,132,-1,132,1,224,132,224,21001,224,1,1,21101,195,0,0,106,0,108,20207,1,223,2,20102,1,23,1,21102,-1,1,3,21101,0,214,0,1106,0,303,22101,1,1,1,204,1,99,0,0,0,0,109,5,1201,-4,0,249,22101,0,-3,1,22101,0,-2,2,22102,1,-1,3,21102,250,1,0,1106,0,225,22101,0,1,-4,109,-5,2106,0,0,109,3,22107,0,-2,-1,21202,-1,2,-1,21201,-1,-1,-1,22202,-1,-2,-2,109,-3,2106,0,0,109,3,21207,-2,0,-1,1206,-1,294,104,0,99,21201,-2,0,-2,109,-3,2105,1,0,109,5,22207,-3,-4,-1,1206,-1,346,22201,-4,-3,-4,21202,-3,-1,-1,22201,-4,-1,2,21202,2,-1,-1,22201,-4,-1,1,21201,-2,0,3,21102,343,1,0,1105,1,303,1105,1,415,22207,-2,-3,-1,1206,-1,387,22201,-3,-2,-3,21202,-2,-1,-1,22201,-3,-1,3,21202,3,-1,-1,22201,-3,-1,2,21201,-4,0,1,21101,384,0,0,1106,0,303,1106,0,415,21202,-4,-1,-4,22201,-4,-3,-4,22202,-3,-2,-2,22202,-2,-4,-4,22202,-3,-2,-3,21202,-4,-1,-2,22201,-3,-2,1,22102,1,1,-4,109,-5,2105,1,0 };

        UCoord known = { 65, 122 };
        if (!Validate(instructions, known, 10)) return false;

        auto pos = FindRoughPosition(instructions, 10);
        auto finalPos = FineTune(instructions, pos, 10);

        if (!Validate(instructions, finalPos, 10)) return false;
        if (finalPos != known) return false;
        return true;
    }
}