#include "2019/d7_Comp3.h"
#include "2019/Comp.h"

SOLUTION(2019, 7) {
    struct Amplifier {
        constexpr Amplifier(std::vector<s64> code, s64 phase) : Code(code) {
            State.Inputs.push_back(phase);
        }

        std::vector<s64> Code{};
        Args State{};
        constexpr s64 Run(s64 input) {
            State.Inputs.push_back(input);
            while (State.Output == Unset) if (!Apply(Code, State)) break;
            auto result = State.Output;
            State.Output = Unset;
            return result;
        }

        constexpr bool IsDone() const {
            return Code[State.Ip] % 100 == OpCode::Halt;
        }
    };
    PART_ONE() {
        auto code = ParseInstructions(lines[0]);
        s64 best = 0;
        std::vector<s64> phases = { 0, 1, 2, 3, 4 };
        do {
            std::vector<Amplifier> amps;
            for (auto i = 0; i < 5; i++) {
                amps.push_back(Amplifier{ code, phases[i] });
            }

            s64 previous = 0;
            for (auto& amp : amps) {
                previous = amp.Run(previous);
            }
            best = std::max(best, previous);
        } while (std::next_permutation(phases.begin(), phases.end()));

        return Constexpr::ToString(best);
    }

    PART_TWO() {
        auto code = ParseInstructions(lines[0]);
        s64 best = 0;
        std::vector<s64> phases = { 5, 6, 7, 8, 9 };
        do {
            std::vector<Amplifier> amps;
            for (auto i = 0; i < 5; i++) {
                amps.push_back(Amplifier{ code, phases[i] });
            }

            s64 previous = 0;
            while (!amps.back().IsDone()) {
                for (auto& amp : amps) {
                    previous = amp.Run(previous);
                }
            }
            best = std::max(best, previous);
        } while (std::next_permutation(phases.begin(), phases.end()));

        return Constexpr::ToString(best);
    }

    TESTS() {
        return true;
    }
}