#include "2018/d18_Lumber.h"

SOLUTION(2018, 18) {
    enum struct GroundType { Open, Trees, Lumber };

    template<size_t Rows, size_t Cols>
    using Map = std::array<std::array<GroundType, Cols>, Rows>;

    template<size_t Rows, size_t Cols>
    constexpr Map<Rows, Cols> ParseMap(const std::vector<std::string>&lines) {
        Map<Rows, Cols> result;
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                result[row][col] = lines[row][col] == '.' ? GroundType::Open :
                    lines[row][col] == '|' ? GroundType::Trees : GroundType::Lumber;
            }
        }
        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr std::vector<GroundType> GetNeighbors(size_t row, size_t col, const Map<Rows, Cols>&map) {
        std::vector<GroundType> result;
        RowCol pos = { row, col };
        RowCol max = { Rows - 1, Cols - 1 };
        auto neighbors = GetAllNeighbors(pos, max);

        for (const auto& neighbor : neighbors) {
            result.push_back(map[neighbor.Row][neighbor.Col]);
        }
        return result;
    }

    constexpr GroundType GetNext(GroundType current, const std::vector<GroundType>&neighbors) {
        auto countTrees = std::count_if(neighbors.begin(), neighbors.end(), [](GroundType g) { return g == GroundType::Trees; });
        auto countLumber = std::count_if(neighbors.begin(), neighbors.end(), [](GroundType g) { return g == GroundType::Lumber; });

        if (current == GroundType::Open && countTrees > 2) {
            return GroundType::Trees;
        }
        else if (current == GroundType::Trees && countLumber > 2) {
            return GroundType::Lumber;
        }
        else if (current == GroundType::Lumber && (countTrees == 0 || countLumber == 0)) {
            return GroundType::Open;
        }

        return current;
    }

    template<size_t Rows, size_t Cols>
    constexpr Map<Rows, Cols> TickMap(const Map<Rows, Cols>&map) {
        Map<Rows, Cols> result;
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                auto neighbors = GetNeighbors(row, col, map);
                result[row][col] = GetNext(map[row][col], neighbors);
            }
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    auto GetScore(const Map<Rows, Cols>&map) {
        u32 trees = 0, lumber = 0;
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                if (map[row][col] == GroundType::Trees) trees++;
                if (map[row][col] == GroundType::Lumber) lumber++;
            }
        }

        return trees * lumber;
    }

    template<size_t Rows, size_t Cols>
    auto Part1(const std::vector<std::string>&lines) {
        auto map = ParseMap<Rows, Cols>(lines);
        for (auto i = 0; i < 10; i++) {
            map = TickMap(map);
        }

        return GetScore(map);
    }

    template<size_t Rows, size_t Cols>
    void FindPeriod(Map<Rows, Cols>&map, size_t & current, size_t & period) {
        std::unordered_set<size_t> seen;
        size_t inARow = 0;
        while (inARow < 5) {
            current++;
            map = TickMap(map);
            auto score = GetScore(map);
            if (seen.contains(score)) {
                inARow++;
            }
            else {
                inARow = 0;
            }
            seen.insert(score);
        }

        auto score = GetScore(map);
        while (true) {
            current++;
            period++;
            map = TickMap(map);
            if (GetScore(map) == score) {
                break;
            }
        }
    }

    template<size_t Rows, size_t Cols>
    auto Part2(const std::vector<std::string>&lines) {
        auto map = ParseMap<Rows, Cols>(lines);
        size_t current = 0, period = 0;
        FindPeriod(map, current, period);
        auto remaining = 1'000'000'000 - current;
        remaining %= period;
        for (auto i = 0; i < remaining; i++) {
            map = TickMap(map);
        }
        return GetScore(map);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1<50, 50>(lines));
        return Constexpr::ToString(Part2<50, 50>(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            ".#.#...|#.",
            ".....#|##|",
            ".|..|...#.",
            "..|#.....#",
            "#.#|||#|#|",
            "...#.||...",
            ".|....|...",
            "||...#|.#|",
            "|.||||..|.",
            "...#.|..|."
        };

        if (Part1<10, 10>(lines) != 1147) return false;
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