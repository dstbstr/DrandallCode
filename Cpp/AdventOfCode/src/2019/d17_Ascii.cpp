#include "2019/d17_Ascii.h"
#include "2019/Comp.h"

SOLUTION(2019, 17) {
    static constexpr s64 Consumed = -91234;
    using Map = std::unordered_map<Coord, bool>;

    constexpr std::string ReadOutput(std::vector<s64>&instructions) {
        Args args;
        args.Output = Consumed;
        std::string result;
        while (Apply(instructions, args)) {
            if (args.Output != Consumed) {
                result.push_back(static_cast<char>(args.Output));
                args.Output = Consumed;
            }
        }

        return result;
    }

    Map ParseMap(const std::string & mapStr) {
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

    auto Part1(const std::string & line) {
        auto instructions = ParseInstructions(line);
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

        return result;
    }

    Args SetupInputs(const std::vector<std::string>&programs, bool videoFeed) {
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
    auto Part2(const std::string & line) {
        auto instructions = ParseInstructions(line);
        instructions[0] = 2;
        std::string mainProgram = "C,A,C,B,C,A,B,A,B,A";
        std::string programA = "R,8,L,12,R,4,R,4";
        std::string programB = "R,8,L,10,R,8";
        std::string programC = "R,8,L,10,L,12,R,4";

        auto args = SetupInputs({ mainProgram, programA, programB, programC }, false);

        auto output = Consumed;
        while (Apply(instructions, args)) {
            if (args.Output != Consumed) {
                output = args.Output;
                args.Output = Consumed;
            }
        }

        return output;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
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