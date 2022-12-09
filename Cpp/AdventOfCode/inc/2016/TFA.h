#pragma once

#include "Common.h"

//50 wide, 6 tall
//rect AxB -> top left through A wide, B tall
//rotate row y=A by B -> push all pixels of A right B positions (with wrap)
//rotate column x=A by B -> push all pixesls of A down B pixesls (with wrap)

template<size_t Rows, size_t Cols>
using Bits = std::array<std::array<bool, Cols>, Rows>;

template<size_t Rows, size_t Cols>
constexpr void Rect(Bits<Rows, Cols>& bits, u32 width, u32 height) {
    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            bits[row][col] = true;
        }
    }
}

template<size_t Rows, size_t Cols>
constexpr void RotRow(Bits<Rows, Cols>& bits, u32 row, u32 amount) {
    //8-2, 9-2, 10-2, 11-2, 12-2
    //(size + col - amount) % size

    std::vector<bool> temp;
    for (size_t col = 0; col < bits[row].size(); col++) {
        size_t target = (bits[row].size() + col - amount) % bits[row].size();
        temp.push_back(bits[row][target]);
    }

    for (size_t col = 0; col < bits[row].size(); col++) {
        bits[row][col] = temp[col];
    }
}

template<size_t Rows, size_t Cols>
constexpr void RotCol(Bits<Rows, Cols>& bits, u32 col, u32 amount) {
    std::vector<bool> temp;
    for (auto row = 0; row < bits.size(); row++) {
        size_t target = (bits.size() + row - amount) % bits.size();
        temp.push_back(bits[target][col]);
    }

    for (size_t row = 0; row < bits.size(); row++) {
        bits[row][col] = temp[row];
    }
}

template<size_t Rows, size_t Cols>
constexpr u32 CountLit(const Bits<Rows, Cols>& bits) {
    u32 result = 0;
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            result += bits[row][col] * 1;
        }
    }

    return result;
}

template<size_t Rows, size_t Cols>
std::function<void(Bits<Rows, Cols>&, u32, u32)> ParseLine(const std::string& line, u32& arg1, u32& arg2) {
    static const auto rectRe = std::regex(R"(rect (\d+)x(\d+))");
    static const auto rotRowRe = std::regex(R"(rotate row y=(\d+) by (\d+))");
    static const auto rotColRe = std::regex(R"(rotate column x=(\d+) by (\d+))");

    std::smatch match;
    if (std::regex_search(line, match, rectRe)) {
        ParseNumber(match[1].str(), arg1);
        ParseNumber(match[2].str(), arg2);
        return Rect<Rows, Cols>;
    }
    else if (std::regex_search(line, match, rotRowRe)) {
        ParseNumber(match[1].str(), arg1);
        ParseNumber(match[2].str(), arg2);
        return RotRow<Rows, Cols>;
    }
    else if (std::regex_search(line, match, rotColRe)) {
        ParseNumber(match[1].str(), arg1);
        ParseNumber(match[2].str(), arg2);
        return RotCol<Rows, Cols>;
    }

    throw std::logic_error("Didn't match any regex");
}

template<size_t Rows, size_t Cols>
std::string PrintWords(const Bits<Rows, Cols>& bits) {
    std::string result = "\n";
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            if (col % 5 == 0) {
                result += " ";
            }
            result += (bits[row][col] ? "#" : " ");
        }
        result += "\n";
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    Bits<6, 50> bits;
    for (auto row = 0; row < bits.size(); row++) {
        std::fill(bits[row].begin(), bits[row].end(), false);
    }

    for (const auto& line : lines) {
        u32 arg1;
        u32 arg2;
        auto instruction = ParseLine<6, 50>(line, arg1, arg2);
        instruction(bits, arg1, arg2);
    }

    return PrintWords(bits);
}

template<size_t Rows, size_t Cols>
constexpr std::string ToString(const Bits<Rows, Cols>& bits) {
    std::string result = "";
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            result += (bits[row][col] ? "#" : ".");
        }
    }

    return result;
}

bool RunTests() {
    auto bits = Bits<3, 7>{};
    for (auto row = 0; row < bits.size(); row++) {
        std::fill(bits[row].begin(), bits[row].end(), false);
    }

    u32 arg1;
    u32 arg2;
    
    auto func = ParseLine<3, 7>("rect 3x2", arg1, arg2);
    func(bits, arg1, arg2);

    func = ParseLine<3, 7>("rotate column x=1 by 1", arg1, arg2);
    func(bits, arg1, arg2);

    func = ParseLine<3, 7>("rotate row y=0 by 4", arg1, arg2);
    func(bits, arg1, arg2);

    func = ParseLine<3, 7>("rotate column x=1 by 1", arg1, arg2);
    func(bits, arg1, arg2);

    if (ToString(bits) !=
        ".#..#.#"
        "#.#...."
        ".#.....") return false;

    if (CountLit(bits) != 6) return false;

    return true;
}