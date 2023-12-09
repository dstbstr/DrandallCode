#include "2016/d2_BathroomSecurity.h"

#include "Facing.h"

SOLUTION(2016, 2) {
    constexpr Facing ParseDirection(const char letter) {
        switch (letter) {
        case 'U': return Facing::Up;
        case 'D': return Facing::Down;
        case 'L': return Facing::Left;
        default: case 'R': return Facing::Right;
        }
    }

    using LookupType = Constexpr::SmallMap<std::string, Constexpr::SmallMap<Facing, std::string>>;

    template<size_t Rows, size_t Cols>
    constexpr LookupType BuildMap(const std::vector<std::string>& input) {
        auto result = LookupType{};

        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                const auto& current = input[row * Rows + col];
                auto upIndex = (row - 1) * Rows + col;
                auto downIndex = (row + 1) * Rows + col;
                auto rightIndex = row * Rows + col + 1;
                auto leftIndex = row * Rows + col - 1;

                if (current == " ") continue;
                if (col == 0 || input[leftIndex] == " ") {
                    result[current][Facing::Left] = current;
                }
                else {
                    result[current][Facing::Left] = input[leftIndex];
                }

                if (row == 0 || input[upIndex] == " ") {
                    result[current][Facing::Up] = current;
                }
                else {
                    result[current][Facing::Up] = input[upIndex];
                }

                if (col == Cols - 1 || input[rightIndex] == " ") {
                    result[current][Facing::Right] = current;
                }
                else {
                    result[current][Facing::Right] = input[rightIndex];
                }

                if (row == Rows - 1 || input[downIndex] == " ") {
                    result[current][Facing::Down] = current;
                }
                else {
                    result[current][Facing::Down] = input[downIndex];
                }
            }
        }

        return result;
    }

    constexpr std::vector<std::string> GetCode(const auto& lines, const LookupType& lookup) {
        std::string key = "5";
        std::vector<std::string> result;

        for (const auto& line : lines) {
            for (auto c : line) {
                key = lookup.at(key).at(ParseDirection(c));
            }
            result.push_back(key);
        }

        return result;
    }

    constexpr std::string GetSimpleCode(const auto& lines) {
        auto lookup = BuildMap<3, 3>({
            "1", "2", "3",
            "4", "5", "6",
            "7", "8", "9"
            }
        );

        auto keys = GetCode(lines, lookup);
        return Constexpr::JoinVec("", keys);
    }

    constexpr std::string GetRealCode(const auto& lines) {
        auto lookup = BuildMap<5, 5>({
            " ", " ", "1", " ", " ",
            " ", "2", "3", "4", " ",
            "5", "6", "7", "8", "9",
            " ", "A", "B", "C", " ",
            " ", " ", "D", " ", " "
            });

        auto keys = GetCode(lines, lookup);
        return Constexpr::JoinVec("", keys);
    }

    PART_ONE() {
        return GetSimpleCode(lines);
    }
    PART_TWO() {
        return GetRealCode(lines);
    }

    TESTS() {
        std::vector<std::string_view> lines = {
            "ULL",
            "RRDDD",
            "LURDL",
            "UUUUD"
        };

        if (GetSimpleCode(lines) != "1985") return false;

        return true;
    }
}
