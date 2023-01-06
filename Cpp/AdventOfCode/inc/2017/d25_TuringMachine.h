#pragma once

#include "Common.h"
#include "bitset"

/*
using State = std::function<size_t(std::bitset<1'000'000>&, size_t&)>;
using States = std::array<State, 6>;
*/

enum struct State {A, B, C, D, E, F};

void Step(std::bitset<1'000'000>& tape, size_t& cursor, State& state) {
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
/*
Begin in state A.
Perform a diagnostic checksum after 12667664 steps.
*/
auto PartOne(const std::vector<std::string>&) {
    State state = State::A;
    size_t cursor = 500'000;
    std::bitset<1'000'000> tape{};

    for (auto i = 0; i < 12667664; i++) {
        Step(tape, cursor, state);
    }

    return tape.count();
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(PartOne(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {

    };

    if (PartOne(lines) != 0) return false;
    return true;
}

