#pragma once

#include "Common.h"

constexpr std::string Add(const std::string& lhs, const std::string& rhs) {
    return "[" + lhs + "," + rhs + "]";
}
static_assert(Add("[1,2]", "[[3,4],5]") == "[[1,2],[[3,4],5]]");

constexpr std::string Explode(const std::string& val) {
    size_t index = std::string::npos;
    size_t depth = 0;
    for (size_t i = 0; i < val.size(); i++) {
        if (val[i] == '[') {
            depth++;
            if (depth == 5) {
                index = i + 1;
                break;
            }
        }
        else if (val[i] == ']') {
            depth--;
        }
    }
    if (index == std::string::npos) {
        return val;
    }
    
    auto leftSide = val.substr(0, index - 1);
    auto closeBracket = val.find(']', index);
    auto currentPair = val.substr(index, closeBracket - index);
    auto rightSide = val.substr(closeBracket + 1);

    auto split = Constexpr::Split(currentPair, ",");
    u32 leftNumber, rightNumber;
    Constexpr::ParseNumber(split[0], leftNumber);
    Constexpr::ParseNumber(split[1], rightNumber);

    auto nLeft = leftSide.find_last_not_of("[],");
    auto nRight = rightSide.find_first_not_of("[],");
    if (nLeft != leftSide.npos) {
        auto digitStart = nLeft;
        while (Constexpr::IsDigit(leftSide[digitStart])) digitStart--;
        digitStart++;
        nLeft++;
        u32 number;
        Constexpr::ParseNumber(leftSide.substr(digitStart, nLeft - digitStart), number);

        number += leftNumber;
        leftSide.replace(leftSide.begin() + digitStart, leftSide.begin() + nLeft, Constexpr::ToString(number));
    }
    if (nRight != rightSide.npos) {
        auto digitEnd = nRight;
        while (Constexpr::IsDigit(rightSide[digitEnd])) digitEnd++;
        u32 number;
        Constexpr::ParseNumber(rightSide.substr(nRight, digitEnd - nRight), number);

        number += rightNumber;
        rightSide.replace(rightSide.begin() + nRight, rightSide.begin() + digitEnd, Constexpr::ToString(number));
    }

    return leftSide + "0" + rightSide;
}

static_assert(Explode("[1,2]") == "[1,2]");
static_assert(Explode("[[[[[9,8],1],2],3],4]") == "[[[[0,9],2],3],4]");
static_assert(Explode("[7,[6,[5,[4,[3,2]]]]]") == "[7,[6,[5,[7,0]]]]");
static_assert(Explode("[[6,[5,[4,[3,2]]]],1]") == "[[6,[5,[7,0]]],3]");
static_assert(Explode("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]") == "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
static_assert(Explode("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]") == "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
static_assert(Explode("[[[[4,0],[5,4]],[[7,0],[15,5]]],[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]]") == "[[[[4,0],[5,4]],[[7,0],[15,5]]],[10,[[0,[11,3]],[[6,3],[8,8]]]]]");

constexpr std::string Split(const std::string& val) {
    size_t digitStart = 0, digitLength = 0;

    for (size_t i = 0; i < val.size(); i++) {
        if (Constexpr::IsDigit(val[i])) {
            digitLength++;
            if (digitLength > 1) {
                digitStart = i - 1;
                break;
            }
        }
        else {
            digitLength = 0;
        }
    }
    if (digitLength == 0) {
        return val;
    }

    auto resultString = val;
    auto digitEnd = val.find_first_of(",]", digitStart);
    u32 number;
    Constexpr::ParseNumber(resultString.substr(digitStart, digitEnd - digitStart), number);
    auto leftNum = number / 2;
    auto rightNum = number % 2 == 0 ? leftNum : leftNum + 1;
    auto newPair = "[" + Constexpr::ToString(leftNum) + "," + Constexpr::ToString(rightNum) + "]";
    resultString.replace(resultString.begin() + digitStart, resultString.begin() + digitEnd, newPair);

    return resultString;
}

static_assert(Split("[1,2]") == "[1,2]");
static_assert(Split("[[[[0,7],4],[15,[0,13]]],[1,1]]") == "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]");
static_assert(Split("[[[[0,7],4],[[7,8],[0,13]]],[1,1]]") == "[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]");

constexpr std::string Reduce(const std::string& val) {
    auto result = Explode(val);
    if (result == val) {
        result = Split(val);
    }
    return result;
}

constexpr size_t CalculateMagnitude(const std::string& val) {
    auto running = val;
    while (true) {
        auto closeIndex = running.find(']');
        auto openIndex = running.substr(0, closeIndex).rfind('[');

        auto substring = running.substr(openIndex + 1, closeIndex - openIndex - 1);
        auto split = Constexpr::Split(substring, ",");
        size_t lhs, rhs;
        Constexpr::ParseNumber(split[0], lhs);
        Constexpr::ParseNumber(split[1], rhs);
        auto replacementVal = Constexpr::ToString((lhs * 3) + (rhs * 2));

        running = running.replace(running.begin() + openIndex, running.begin() + closeIndex + 1, replacementVal.c_str());
        if (running.find(']') == running.npos) break;
    }

    size_t result;
    Constexpr::ParseNumber(running, result);
    return result;
}

static_assert(CalculateMagnitude("[9,1]") == 29);
static_assert(CalculateMagnitude("[1,9]") == 21);
static_assert(CalculateMagnitude("[[9,1],[1,9]]") == 129);

constexpr std::string FullyReduce(const std::string& eq) {
    auto running = eq;
    std::string prev = "";
    while (running != prev) {
        prev = running;
        running = Reduce(running);
    }

    return running;
}

constexpr auto PartOne(const std::vector<std::string>& lines) {
    std::string current = lines[0];
    for (size_t i = 1; i < lines.size(); i++) {
        current = FullyReduce(Add(current, lines[i]));
    }

    return CalculateMagnitude(current);
}

constexpr auto PartTwo(const std::vector<std::string>& lines) {
    size_t best = 0;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i + 1; j < lines.size(); j++) {
            auto e1 = FullyReduce(Add(lines[i], lines[j]));
            auto e2 = FullyReduce(Add(lines[j], lines[i]));
            best = std::max(best, CalculateMagnitude(e1));
            best = std::max(best, CalculateMagnitude(e2));
        }
    }

    return best;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]",
        "[[[5,[2,8]],4],[5,[[9,9],0]]]",
        "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
        "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]",
        "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]",
        "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
        "[[[[5,4],[7,7]],8],[[8,3],8]]",
        "[[9,3],[[9,9],[6,[4,9]]]]",
        "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
        "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"
    };

    if (PartOne(lines) != 4140) return false;
    if (PartTwo(lines) != 3993) return false;
    return true;
}