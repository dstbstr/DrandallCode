#pragma once

#include "Common.h"

std::string Decompress(const std::string& input, size_t& index) {
    std::string runLengthStr = "";
    u32 runLength = 0;

    std::string repeatLengthStr = "";
    u32 repeatLength = 0;

    std::string result = input.substr(0, index);
    bool started = false;
    bool middle = false;
    for (; index < input.size(); index++) {
        auto c = input[index];
        if (c == '(') {
            started = true;
        }
        else if (c == 'x' && started) {
            middle = true;
        } else if(c == ')' && started) {
            started = false;
            middle = false;
            ParseNumber(runLengthStr, runLength);
            ParseNumber(repeatLengthStr, repeatLength);

            for (size_t repeat = 0; repeat < repeatLength; repeat++) {
                for (size_t run = 0; run < runLength; run++) {
                    result += input[index + 1 + run];
                }
            }

            result += input.substr(index + runLength + 1);

            index -= (3 + runLengthStr.size() + repeatLengthStr.size()); //remove (x) and the numbers
            index += (runLength * repeatLength); //the number of characters added
            index++; //move forward one more
            return result;
        }
        else if (started) {
            if (middle) {
                repeatLengthStr += c;
            }
            else {
                runLengthStr += c;
            }
        }
        else {
            result += c;
        }
    }

    return result;
}

u64 CalculateSubexpression(const std::string& line) {
    u64 result = 0;
    static const auto re = std::regex(R"(\((\d+)x(\d+)\))");
    std::smatch match;
    u32 runLength;
    u32 repeatCount;
    std::string running = line;
    while (!running.empty()) {
        if (running[0] == '(' && std::regex_search(running, match, re)) {
            ParseNumber(match[1].str(), runLength);
            ParseNumber(match[2].str(), repeatCount);
            result += repeatCount * CalculateSubexpression(running.substr(match[0].length(), runLength));
            running = running.substr(match[0].length() + runLength);
        }
        else {
            result++;
            running = running.substr(1);
        }
    }

    return result;
}

// X(8x2)(3x3)ABCY
// XABCABCABCABCABCABCY

//(27x12)(20x12)(13x14)(7x10)(1x12)A
//12 * 12 * 14 * 10 * 12 = 241920
//(27x12) = 12 * (20x12)(13x14)(7x10)(1x12)A
//(20x12) = 12 * 12 * (13x14)(7x10)(1x12)A
//(13x14) = 12 * 12 * 14 * (7x10)(1x12)A
//(7x10) = 12 * 12 * 14 * 10 * (1x12)A
//(1x12) = 12 * 12 * 14 * 10 * 12 * 1

std::string RunFirst(const std::vector<std::string>& lines) {
    size_t index = 0;
    std::string running = lines[0];
    while (index < running.size()) {
        running = Decompress(running, index);
    }

    return ToString(running.size());
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(CalculateSubexpression(lines[0]));
}

bool RunTests() {
    if (CalculateSubexpression("(3x3)XYZ") != 9) return false;
    if (CalculateSubexpression("X(8x2)(3x3)ABCY") != 20) return false;
    if (CalculateSubexpression("(27x12)(20x12)(13x14)(7x10)(1x12)A") != 241920) return false;
    if (CalculateSubexpression("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN") != 445) return false;

    return true;
}