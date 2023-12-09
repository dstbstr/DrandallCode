#include "2018/d18_Lumber.h"

SOLUTION(2018, 18) {

    /*
    ______
    _...._
    _...._
    _...._
    _...._
    ______
    
    _______....__....__....__...._______
    TL = C - 7
    T = C - 6
    TR = C - 5
    L = C - 1
    R = C + 1
    BL = C + 5
    B = C + 6
    BR = C + 7
    
    C = 7, TL = 0, T = 1, TR = 2 L = 6 R = 8 BL = 12 B = 13 BR = 14
    {{}, {}, {}, {}, {}, {}, {}, {0, 1, 2, 6, 8, 12, 13, 14}, {1,2,3,7,9,13,14,15}, {2,3,4,8,10,14,15,16}, {3,4,5,9,11,15,16,17}, {}, {}, 
    */

    template<size_t Rows, size_t Cols>
    constexpr std::array<std::array<size_t, 8>, Rows * Cols> BuildLookup() {
        std::array<std::array<size_t, 8>, Rows * Cols> result;
        for (size_t i = 0; i < Rows * Cols; i++) {
            result[i] = { i - Cols - 1, i - Cols, i - Cols + 1, i - 1, i + 1, i + Cols - 1, i + Cols, i + Cols + 1 };
        }
        return result;
    }

    constexpr auto LookupTableBig = BuildLookup<52, 52>();
    constexpr auto LookupTableSmall = BuildLookup<12, 12>();

    constexpr void Next(const std::string& src, std::string& dest, const std::array<size_t, 8>& indexes, size_t pos) {
        auto n = std::string({ src[indexes[0]], src[indexes[1]], src[indexes[2]], src[indexes[3]], src[indexes[4]], src[indexes[5]], src[indexes[6]], src[indexes[7]] });
        auto treeCount = std::count(n.begin(), n.end(), '|');
        auto yardCount = std::count(n.begin(), n.end(), '#');

        switch (src[pos]) {
        case '.': dest[pos] = treeCount > 2 ? '|' : '.'; break;
        case '|': dest[pos] = yardCount > 2 ? '#' : '|'; break;
        case '#': dest[pos] = (treeCount && yardCount) ? '#' : '.'; break;
        }
    }

    constexpr void NextBig(const std::string& src, std::string& dest) {
        for (size_t i = 1; i < 51; i++) {
            for (size_t j = 1; j < 51; j++) {
                auto pos = i * 52 + j;
                Next(src, dest, LookupTableBig[pos], pos);
            }
        }
    }

    constexpr void NextSmall(const std::string& src, std::string& dest) {
        for (size_t i = 1; i < 11; i++) {
            for (size_t j = 1; j < 11; j++) {
                auto pos = i * 12 + j;
                Next(src, dest, LookupTableSmall[pos], pos);
            }
        }
    }

    constexpr size_t GetScore(const std::string& src) {
        return std::count(src.begin(), src.end(), '|') * std::count(src.begin(), src.end(), '#');
    }

    constexpr std::string Flatten(const auto& lines) {
        std::string border(lines.size() + 2, '_');
        std::string result = border;
        result.reserve(52 * 52);
        for (const auto& line : lines) {
            result.push_back('_');
            result += line;
            result.push_back('_');
        }
        result += border;
        return result;
    }

    PART_ONE() {
        auto src = Flatten(lines);
        auto dest = src;
        for (auto i = 0; i < 10; i++) {
            NextBig(src, dest);
            std::swap(src, dest);
        }

        return Constexpr::ToString(GetScore(src));
    }

    PART_TWO() {
        auto src = Flatten(lines);
        auto dest = src;
        Constexpr::SmallSet<std::string> seen;
        size_t cycleStart = 0;
        while (true) {
            if (!seen.insert(src)) break;
            NextBig(src, dest);
            std::swap(src, dest);
            cycleStart++;
        }
        auto target = src;
        size_t cycleSize = 0;
        do {
            NextBig(src, dest);
            std::swap(src, dest);
            cycleSize++;
        } while (src != target);

        auto remaining = 1'000'000'000 - cycleStart;
        remaining %= cycleSize;
        for (auto i = 0u; i < remaining; i++) {
            NextBig(src, dest);
            std::swap(src, dest);
        }

        return Constexpr::ToString(GetScore(src));
    }

    TESTS() {
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

        auto src = Flatten(lines);
        auto dest = src;
        for (auto i = 0; i < 10; i++) {
            NextSmall(src, dest);
            std::swap(src, dest);
        }
        
        if (GetScore(src) != 1147) return false;

        return true;
    }
}

/*
#include "Algorithms/FloydCycle.h"

constexpr char NextState(char current, const std::vector<char>&neighbors) {
    switch (current) {
    case '.': return std::count(neighbors.begin(), neighbors.end(), '|') > 2 ? '|' : '.';
    case '|': return std::count(neighbors.begin(), neighbors.end(), '#') > 2 ? '#' : '|';
    case '#': return std::count(neighbors.begin(), neighbors.end(), '|') && std::count(neighbors.begin(), neighbors.end(), '#') ? '#' : '.';
    }
    return ' ';
}

constexpr std::vector<std::string> Next(const std::vector<std::string>& current, RowCol limits) {
    std::vector<std::string> result = current;
    for (size_t row = 0; row <= limits.Row; row++) {
        for (size_t col = 0; col <= limits.Col; col++) {
            RowCol rc{ row, col };
            auto neighbors = GetAllNeighbors(rc, limits);
            std::vector<char> n;
            std::transform(neighbors.begin(), neighbors.end(), std::back_inserter(n), [&](RowCol pos) { return current[pos.Row][pos.Col]; });

            result[row][col] = NextState(current[row][col], n);
        }
    }

    return result;
}

constexpr size_t GetScore(const std::vector<std::string>&current) {
    size_t trees{ 0 }, yards{ 0 };
    for (const auto& row : current) {
        trees += std::count(row.begin(), row.end(), '|');
        yards += std::count(row.begin(), row.end(), '#');
    }

    return trees * yards;
}

PART_ONE() {
    auto running = lines;
    RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
    for (auto i = 0; i < 10; i++) {
        running = Next(running, limits);
    }
    return Constexpr::ToString(GetScore(running));
}

PART_TWO() {
    RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
    u32 cycleLength, cycleStart;
    auto running = FloydCycle::FindCycle(lines, Next, cycleLength, cycleStart, limits);

    auto remaining = 1'000'000'000 - cycleStart;
    remaining %= cycleLength;

    for (auto i = 0u; i < remaining; i++) {
        running = Next(running, limits);
    }
    return Constexpr::ToString(GetScore(running));
}

TESTS() {
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

    if (PartOne(lines) != "1147") return false;
    return true;
}
*/
