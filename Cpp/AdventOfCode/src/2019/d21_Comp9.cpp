#include "2019/d21_Comp9.h"
#include "2019/Comp.h"

SOLUTION(2019, 21) {
    enum Register { A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', T = 'T', J = 'J' };
    enum Command { AND, OR, NOT, WALK, RUN };
    constexpr s64 SPACE = ' ';

    constexpr void AddCommand(Command cmd, Register lhs, Register rhs, Args & args) {
        auto AddAll = [&args](const std::string& in) {
            for (auto c : in) {
                args.Inputs.push_back(static_cast<s64>(c));
            }
        };

        switch (cmd) {
        case AND: AddAll("AND "); break;
        case OR: AddAll("OR "); break;
        case NOT: AddAll("NOT "); break;
        }

        args.Inputs.push_back(lhs);
        args.Inputs.push_back(SPACE);
        args.Inputs.push_back(rhs);
        AddAll("\n");
    }

    constexpr void AddCommand(Command cmd, Args & args) {
        auto AddAll = [&args](const std::string& in) {
            for (auto c : in) {
                args.Inputs.push_back(static_cast<s64>(c));
            }
        };

        switch (cmd) {
        case WALK: AddAll("WALK\n"); break;
        case RUN: AddAll("RUN\n"); break;
        }
    }

    constexpr std::string Start(std::vector<s64>&instructions, Args & args) {
        args.Output = Unset;
        std::string result;

        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                if (args.Output > 255) {
                    return Constexpr::ToString(args.Output);
                }
                result.push_back(static_cast<char>(args.Output));
                args.Output = Unset;
            }
        }

        return result;
    }

    PART_ONE() {
        auto instructions = ParseInstructions(lines[0]);
        Args args;

        //if(!a) jump = true
        AddCommand(NOT, A, J, args);

        //jump = jump || !b
        AddCommand(NOT, B, T, args);
        AddCommand(OR, T, J, args);

        //jump = jump || !c
        AddCommand(NOT, C, T, args);
        AddCommand(OR, T, J, args);

        //if(!d) jump= false
        AddCommand(AND, D, J, args);

        AddCommand(WALK, args);
        return Start(instructions, args);
    }

    PART_TWO() {
        auto instructions = ParseInstructions(lines[0]);
        Args args;

        //if(!a) jump = true
        AddCommand(NOT, A, J, args);

        //jump = jump || !b
        AddCommand(NOT, B, T, args);
        AddCommand(OR, T, J, args);

        //jump = jump || !c
        AddCommand(NOT, C, T, args);
        AddCommand(OR, T, J, args);

        //if(!d) jump= false
        AddCommand(AND, D, J, args);

        //if(e || h) jump = true
        AddCommand(NOT, T, T, args); //reset T
        AddCommand(OR, E, T, args);
        AddCommand(OR, H, T, args);
        AddCommand(AND, T, J, args);

        AddCommand(RUN, args);
        return Start(instructions, args);
    }

    TESTS() {
        return true;
    }
}