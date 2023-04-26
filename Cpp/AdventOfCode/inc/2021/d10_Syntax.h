#pragma once

#include "Common.h"
#include <stack>

struct State {
    std::vector<char> Expected;
    bool IsCorrupt = false;
    char InvalidChar;
};

constexpr State ValidateLine(const std::string& line) {
    State result;

    for (auto c : line) {
        bool checkBack = false;
        switch (c) {
        case '(': result.Expected.push_back(')'); break;
        case '[': result.Expected.push_back(']'); break;
        case '{': result.Expected.push_back('}'); break;
        case '<': result.Expected.push_back('>'); break;
        default: checkBack = true; break;
        }
        if (!checkBack) continue;
        if (c == result.Expected.back()) {
            result.Expected.pop_back();
            continue;
        }
        else {
            result.IsCorrupt = true;
            result.InvalidChar = c;
            return result;
        }
    }
    return result;
}

/*
static_assert(GetLineScore("()") == 0);
static_assert(GetLineScore("[<>({}){}[([])<>]]") == 0);
static_assert(GetLineScore("{([(<{}[<>[]}>{[]{[(<()>") == 1197);
static_assert(GetLineScore("<{([([[(<>()){}]>(<<{{") == 25137);
static_assert(GetLineScore("[(()[<>])]({[<{<<[]>>(") == 0);
*/

constexpr size_t GetCorruptScore(State& state) {
    auto c = state.InvalidChar;
    switch (c) {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
    default: return 0;
    }
}

auto PartOne(const std::vector<std::string>& lines) {
    size_t result = 0;
    for (const auto& line : lines) {
        auto state = ValidateLine(line);
        if (state.IsCorrupt) {
            result += GetCorruptScore(state);
        }
    }
    return result;
}

constexpr size_t GetAutoCompleteScore(State& state) {
    size_t result = 0;
    while (!state.Expected.empty()) {
        auto c = state.Expected.back();
        state.Expected.pop_back();
        result *= 5;
        switch (c) {
        case ')': result += 1; break;
        case ']': result += 2; break;
        case '}': result += 3; break;
        case '>': result += 4; break;
        }
    }

    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    std::vector<size_t> scores;
    for (const auto& line : lines) {
        auto state = ValidateLine(line);
        if (state.IsCorrupt) continue;
        scores.push_back(GetAutoCompleteScore(state));
    }

    std::sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "[({(<(())[]>[[{[]{<()<>>",
        "[(()[<>])]({[<{<<[]>>(",
        "(((({<>}<{<{<>}{[]{[]{}",
        "{<[[]]>}<{[{[{[]{()[[[]",
        "<{([{{}}[<[[[<>{}]]]>[]]"
    };

    if (PartTwo(lines) != 288957) return false;
    return true;
}