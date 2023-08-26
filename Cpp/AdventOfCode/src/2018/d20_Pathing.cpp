#include "2018/d20_Pathing.h"
#include <queue>

SOLUTION(2018, 20) {
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

    constexpr size_t Solve(std::string_view line, auto OnStep) {
        size_t index = 1;
        Grid grid{};
        Coord origin = { 0, 0 };
        Recurse(line, index, origin, grid);

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
        auto result = Solve(Line, [](size_t length, size_t& r) {
            r = std::max(length, r);
            });
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto result = Solve(Line, [](size_t length, size_t& r) {
            r += length >= 1000;
            });
        return Constexpr::ToString(result);
    }

    TESTS() {
        auto KeepMax = [](size_t length, size_t& r) {
            r = std::max(length, r);
            };
        static_assert(Solve("^WNE$", KeepMax) == 3);
        static_assert(Solve("^ENWWW(NEEE|SSE(EE|N))$", KeepMax) == 10);
        static_assert(Solve("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$", KeepMax) == 18);
        static_assert(Solve("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$", KeepMax) == 23);
        static_assert(Solve( "^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$" , KeepMax) == 31);
 
        return true;
    }
}