#include "2018/d20_Pathing.h"
#include <queue>

SOLUTION(2018, 20) {
    //using Grid = std::unordered_map<Coord, std::vector<Coord>>;
    using Grid = Constexpr::SmallMap<Coord, std::vector<Coord>>;

    constexpr void Connect(Coord & a, const Coord & b, Grid & grid) {
        grid[a].push_back(b);
        grid[b].push_back(a);
        a = b;
    }

    constexpr void Recurse(std::string_view chars, size_t & index, Coord pos, Grid & grid) {
        auto origin = pos;
        auto& [x, y] = pos;
        while (true) {
            switch (chars[index++]) {
            case 'N': Connect(pos, { x, y - 1 }, grid); break;
            case 'S': Connect(pos, { x, y + 1 }, grid); break;
            case 'E': Connect(pos, { x - 1, y }, grid); break;
            case 'W': Connect(pos, { x + 1, y }, grid); break;
            case '(': Recurse(chars, index, pos, grid); break;
            case ')': return;
            case '|': pos = origin; break;
            case '$': return;
            }
        }
    }

    auto Part1(const std::string & line) {
        size_t index = 1;
        Grid grid{};
        Recurse(line, index, { 0, 0 }, grid);

        std::unordered_set<Coord> visited;
        std::queue<std::pair<Coord, int>> queue;
        queue.push({ {0, 0}, 0 });
        auto longest = 0;
        while (!queue.empty()) {
            auto [pos, length] = queue.front();
            queue.pop();
            if (visited.contains(pos)) continue;
            visited.insert(pos);
            longest = std::max(longest, length);
            for (auto neighbors : grid[pos]) {
                queue.push({ neighbors, length + 1 });
            }
        }

        return longest;
    }

    auto Part2(const std::string & line) {
        size_t index = 1;
        Grid grid{};
        Recurse(line, index, { 0, 0 }, grid);

        std::unordered_set<Coord> visited;
        std::queue<std::pair<Coord, int>> queue;
        queue.push({ {0, 0}, 0 });
        auto overTarget = 0;
        while (!queue.empty()) {
            auto [pos, length] = queue.front();
            queue.pop();
            if (visited.contains(pos)) continue;
            visited.insert(pos);
            if (length >= 1000) overTarget++;
            for (auto neighbors : grid[pos]) {
                queue.push({ neighbors, length + 1 });
            }
        }

        return overTarget;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        if (Part1("^WNE$") != 3) return false;
        if (Part1("^ENWWW(NEEE|SSE(EE|N))$") != 10) return false;
        if (Part1("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$") != 18) return false;
        if (Part1("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$") != 23) return false;
        if (Part1("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$") != 31) return false;
        return true;
    }

    constexpr size_t Solve(const std::vector<std::string>& lines, auto OnStep) {
        size_t index = 1;
        Grid grid{};
        Coord origin = { 0, 0 };
        Recurse(lines[0], index, origin, grid);

        Constexpr::SmallSet<Coord> visited;
        Constexpr::Queue<std::pair<Coord, int>> queue;
        queue.push({ origin, 0 });

        size_t result = 0;
        while (!queue.is_empty()) {
            auto [pos, length] = queue.front();
            queue.pop();
            if (visited.contains(pos)) continue;
            visited.insert(pos);
            OnStep(length, result);
            for (auto neighbor : grid[pos]) {
                queue.push({ neighbor, length + 1 });
            }
        }

        return result;
    }
    PART_ONE() {
        auto result = Solve(lines, [](size_t length, size_t& r) {
            r = std::max(length, r);
            });
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto result = Solve(lines, [](size_t length, size_t& r) {
            r += length >= 1000;
            });
        return Constexpr::ToString(result);
    }

    TESTS() {
        static_assert(PartOne({ "^WNE$" }) == "3");
        static_assert(PartOne({"^ENWWW(NEEE|SSE(EE|N))$"}) == "10");
        static_assert(PartOne({"^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$"}) == "18");
        static_assert(PartOne({"^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$"}) == "23");
        static_assert(PartOne({ "^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$" }) == "31");
        return true;
    }
}