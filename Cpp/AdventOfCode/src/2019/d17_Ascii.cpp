#include "2019/d17_Ascii.h"
#include "2019/Comp.h"

SOLUTION(2019, 17) {
    using Map = Constexpr::SmallMap<Coord, bool>;

    constexpr std::string ReadOutput(std::vector<s64>&instructions) {
        Args args;
        std::string result;
        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                result.push_back(static_cast<char>(args.Output));
                args.Output = Unset;
            }
        }

        return result;
    }

    constexpr Map ParseMap(const std::string & mapStr) {
        Map result;
        Coord pos{ 0, 0 };
        for (auto c : mapStr) {
            if (c == '.') {
                result[pos] = false;
            }
            else if (c == '\n') {
                pos.Y++;
                pos.X = 0;
                continue;
            }
            else {
                result[pos] = true;
            }
            pos.X++;
        }
        return result;
    }

    constexpr Args SetupInputs(const std::vector<std::string>&programs, bool videoFeed) {
        Args args;
        for (const auto& program : programs) {
            for (auto c : program) {
                args.Inputs.push_back(c);
            }
            args.Inputs.push_back('\n');
        }
        args.Inputs.push_back(videoFeed ? 'y' : 'n');
        args.Inputs.push_back('\n');

        return args;
    }

    PART_ONE() {
        auto instructions = ParseInstructions(Line);
        auto map = ParseMap(ReadOutput(instructions));
        s64 result = 0;
        Coord min, max, pos;
        GetLimitsFromMap(map, min, max);

        for (auto row = min.Y; row <= max.Y; row++) {
            for (auto col = min.X; col <= max.X; col++) {
                pos = { col, row };
                if (!map.at(pos)) continue;

                auto neighbors = GetDirectNeighbors(pos, max, min);
                bool isIntersection = true;
                for (auto neighbor : neighbors) {
                    if (!map.at(neighbor)) {
                        isIntersection = false;
                        break;
                    }
                }

                if (isIntersection) {
                    result += (pos.X * pos.Y);
                }
            }
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto instructions = ParseInstructions(Line);
        instructions[0] = 2;
        std::string mainProgram = "C,A,C,B,C,A,B,A,B,A";
        std::string programA = "R,8,L,12,R,4,R,4";
        std::string programB = "R,8,L,10,R,8";
        std::string programC = "R,8,L,10,L,12,R,4";

        auto args = SetupInputs({ mainProgram, programA, programB, programC }, false);

        auto output = Unset;
        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                output = args.Output;
                args.Output = Unset;
            }
        }

        return Constexpr::ToString(output);
    }

    TESTS() {
        return true;
    }
}