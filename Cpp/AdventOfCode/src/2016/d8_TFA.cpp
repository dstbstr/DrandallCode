#include "2016/d8_TFA.h"

SOLUTION(2016, 8) {
    //50 wide, 6 tall
    //rect AxB -> top left through A wide, B tall
    //rotate row y=A by B -> push all pixels of A right B positions (with wrap)
    //rotate column x=A by B -> push all pixesls of A down B pixesls (with wrap)

    template<size_t Rows, size_t Cols>
    using Bits = std::array<std::array<bool, Cols>, Rows>;

    template<size_t Rows, size_t Cols>
    constexpr void Rect(Bits<Rows, Cols>&bits, u32 width, u32 height) {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                bits[row][col] = true;
            }
        }
    }

    template<size_t Rows, size_t Cols>
    constexpr void RotRow(Bits<Rows, Cols>&bits, u32 row, u32 amount) {
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
    constexpr void RotCol(Bits<Rows, Cols>&bits, u32 col, u32 amount) {
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
    constexpr u32 CountLit(const Bits<Rows, Cols>&bits) {
        u32 result = 0;
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                result += bits[row][col] * 1;
            }
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr auto ParseLine(const std::string& line) {
        return [line](Bits<Rows, Cols>& bits) {
            auto s = Constexpr::Split(line, " ");
            if (s[0] == "rect") {
                auto s1 = Constexpr::Split(s[1], "x");
                u32 arg1, arg2;
                Constexpr::ParseNumber(s1[0], arg1);
                Constexpr::ParseNumber(s1[1], arg2);
                Rect<Rows, Cols>(bits, arg1, arg2);
            }
            else if (s[1] == "row") {
                auto s1 = Constexpr::Split(s[2], "=");
                u32 arg1, arg2;
                Constexpr::ParseNumber(s1[1], arg1);
                Constexpr::ParseNumber(s[4], arg2);
                RotRow<Rows, Cols>(bits, arg1, arg2);
            }
            else {
                auto s1 = Constexpr::Split(s[2], "=");
                u32 arg1, arg2;
                Constexpr::ParseNumber(s1[1], arg1);
                Constexpr::ParseNumber(s[4], arg2);
                RotCol<Rows, Cols>(bits, arg1, arg2);
            }
        };
    }

    template<size_t Rows, size_t Cols>
    constexpr std::string PrintWords(const Bits<Rows, Cols>&bits) {
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

    template<size_t Rows, size_t Cols>
    constexpr std::string ToString(const Bits<Rows, Cols>&bits) {
        std::string result = "";
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                result += (bits[row][col] ? "#" : ".");
            }
        }

        return result;
    }

    PART_ONE() {
        Bits<6, 50> bits{};
        for (const auto& line : lines) {
            ParseLine<6, 50>(line)(bits);
        }
        return Constexpr::ToString(CountLit(bits));
    }
    PART_TWO() {
        Bits<6, 50> bits{};
        for (const auto& line : lines) {
            ParseLine<6, 50>(line)(bits);
        }
        return PrintWords(bits);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "rect 3x2",
            "rotate column x=1 by 1",
            "rotate row y=0 by 4",
            "rotate column x=1 by 1"
        };
        constexpr size_t Rows = 3;
        constexpr size_t Cols = 7;
        auto bits = Bits<Rows, Cols>{};
        for (const auto& line : lines) {
            ParseLine<Rows, Cols>(line)(bits);
        }

        if (ToString(bits) !=
            ".#..#.#"
            "#.#...."
            ".#.....") return false;

        if (CountLit(bits) != 6) return false;

        return true;
    }
}