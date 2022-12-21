#pragma once

#include "Common.h"
using Func = std::function<void(std::string&)>;

Func ParseCommand(const std::string& line, bool scramble) {
    static auto swapPosRe = std::regex(R"(swap position (\d) with position (\d))");
    static auto swapLetRe = std::regex(R"(swap letter (\w) with letter (\w))");
    static auto revPosRe = std::regex(R"(reverse positions (\d) through (\d))");
    static auto rotLeftRe = std::regex(R"(rotate left (\d) steps?)");
    static auto rotRightRe = std::regex(R"(rotate right (\d) steps?)");
    static auto movePosRe = std::regex(R"(move position (\d) to position (\d))");
    static auto rotPosRe = std::regex(R"(rotate based on position of letter (\w))");

    std::smatch match;
    if (std::regex_search(line, match, swapPosRe)) {
        u32 left, right;
        ParseNumber(match[1].str(), left);
        ParseNumber(match[2].str(), right);
        return [left, right](std::string& str) -> void {
            std::swap(str[left], str[right]);
        };
    }
    else if (std::regex_search(line, match, swapLetRe)) {
        auto first = match[1].str();
        auto second = match[2].str();
        return [first, second](std::string& str) -> void {
            std::swap(str[str.find(first)], str[str.find(second)]);
        };
    }
    else if (std::regex_search(line, match, revPosRe)) {
        u32 left, right;
        ParseNumber(match[1].str(), left);
        ParseNumber(match[2].str(), right);
        return [left, right](std::string& str) -> void {
            std::reverse(str.begin() + left, str.begin() + right + 1);
        };
    }
    else if (std::regex_search(line, match, rotLeftRe)) {
        u32 steps;
        ParseNumber(match[1].str(), steps);
        return [steps, scramble](std::string& str) -> void {
            if (scramble) {
                std::rotate(str.begin(), str.begin() + steps, str.end());
            }
            else {
                std::rotate(str.rbegin(), str.rbegin() + steps, str.rend());
            }
        };
    }
    else if (std::regex_search(line, match, rotRightRe)) {
        u32 steps;
        ParseNumber(match[1].str(), steps);
        return [steps, scramble](std::string& str) -> void {
            if (scramble) {
                std::rotate(str.rbegin(), str.rbegin() + steps, str.rend());
            }
            else {
                std::rotate(str.begin(), str.begin() + steps, str.end());
            }
        };
    }
    else if (std::regex_search(line, match, movePosRe)) {
        u32 left, right;
        if (scramble) {
            ParseNumber(match[1].str(), left);
            ParseNumber(match[2].str(), right);
        }
        else {
            ParseNumber(match[1].str(), right);
            ParseNumber(match[2].str(), left);
        }
        return [left, right](std::string& str) -> void {
            if (left < right) {
                str.insert(str.begin() + right + 1, str[left]);
                str.erase(str.begin() + left);
            }
            else {
                str.insert(str.begin() + right, str[left]);
                str.erase(str.begin() + left + 1);
            }
        };
    }
    else if (std::regex_search(line, match, rotPosRe)) {
        auto letter = match[1].str();
        return [letter, scramble](std::string& str) -> void {
            auto index = str.find(letter);
            if (scramble) {
                auto rot = index + 1;
                if (index >= 4) rot++;
                rot %= str.size();
                std::rotate(str.rbegin(), str.rbegin() + rot, str.rend());
            }
            else {
                size_t shift[] = { 1, 1, 6, 2, 7, 3, 0, 4 };
                std::rotate(str.begin(), str.begin() + shift[index], str.end());
            }
        };
    }
    else {
        throw std::logic_error("Failed to parse");
    }
}
std::string Scramble(const std::vector<std::string>& lines, const std::string& input) {
    std::vector<Func> steps;
    for (const auto& line : lines) {
        steps.push_back(ParseCommand(line, true));
    }

    std::string running = input;
    for (const auto& step : steps) {
        step(running);
    }
    return running;
}

std::string UnScramble(const std::vector<std::string>& lines, const std::string& input) {
    std::vector<Func> steps;
    for (s32 i = static_cast<s32>(lines.size() - 1); i >= 0; i--) {
        steps.push_back(ParseCommand(lines[i], false));
    }

    std::string running = input;
    for (const auto& step : steps) {
        step(running);
    }

    return running;
}

std::string Run(const std::vector<std::string>& lines) {
    //return Scramble(lines, "abcdefgh");
    return UnScramble(lines, "fbgdceah");
}

bool RunTests() {
    std::vector<std::string> lines = {
        "swap position 4 with position 0",
        "swap letter d with letter b",
        "reverse positions 0 through 4",
        "rotate left 1 step",
        "move position 1 to position 4",
        "move position 3 to position 0",
        "rotate based on position of letter b",
        "rotate based on position of letter d"
    };

    if (Scramble(lines, "abcde") != "decab") return false;
    if (UnScramble(lines, "decab") != "abcde") return false;

    return true;
}