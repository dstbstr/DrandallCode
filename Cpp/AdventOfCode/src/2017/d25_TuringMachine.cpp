#include "2017/d25_TuringMachine.h"
#include "bitset"

SOLUTION(2017, 25) {
    /*
using State = std::function<size_t(std::bitset<1'000'000>&, size_t&)>;
using States = std::array<State, 6>;
*/

    enum struct State { A, B, C, D, E, F };

    void Step(std::bitset<1'000'000>&tape, size_t & cursor, State & state) {
        switch (state) {
        case State::A: {
            if (tape[cursor]) {
                tape.set(cursor, false);
                cursor--;
                state = State::C;
            }
            else {
                tape.set(cursor);
                cursor++;
                state = State::B;
            }
            break;
        }
        case State::B: {
            if (tape[cursor]) {
                cursor++;
                state = State::D;
            }
            else {
                tape.set(cursor);
                cursor--;
                state = State::A;
            }
            break;
        }
        case State::C: {
            if (tape[cursor]) {
                tape.set(cursor, false);
                cursor--;
                state = State::E;
            }
            else {
                cursor--;
                state = State::B;
            }
            break;
        }
        case State::D: {
            if (tape[cursor]) {
                tape.set(cursor, false);
                cursor++;
                state = State::B;
            }
            else {
                tape.set(cursor);
                cursor++;
                state = State::A;
            }
            break;
        }
        case State::E: {
            if (tape[cursor]) {
                cursor--;
                state = State::C;
            }
            else {
                tape.set(cursor);
                cursor--;
                state = State::F;
            }
            break;
        }
        case State::F: {
            if (tape[cursor]) {
                cursor++;
                state = State::A;
            }
            else {
                tape.set(cursor);
                cursor++;
                state = State::D;
            }
            break;
        }
        default: break;
        }
    }

    auto Part1(const std::vector<std::string>&) {
        State state = State::A;
        size_t cursor = 500'000;
        std::bitset<1'000'000> tape{};

        for (auto i = 0; i < 12667664; i++) {
            Step(tape, cursor, state);
        }

        return tape.count();
    }

    auto Part2(const std::vector<std::string>&lines) {
        return lines.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        return Constexpr::ToString(Part1(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {

        };

        if (Part1(lines) != 0) return false;
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