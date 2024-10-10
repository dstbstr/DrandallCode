#include "2023/d23_ScenicWalk.h"

SOLUTION(2023, 23) {
    constexpr std::vector<RowCol> GetNeighbors(RowCol pos, const std::vector<std::string>&lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        const auto h = RowCol{ 0, 1 };
        const auto v = RowCol{ 1, 0 };
        const auto c = lines[pos.Row][pos.Col];
        std::vector<RowCol> result;
        if (c != '.') {
            switch (c) {
            case '<': result.push_back(pos - h); break;
            case '>': result.push_back(pos + h); break;
            case 'v': result.push_back(pos + v); break;
            case '^': result.push_back(pos - v); break;
            }
        }
        else {
            auto neighbors = GetDirectNeighbors(pos, limits);
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](RowCol rc) {
                return lines[rc.Row][rc.Col] != '#';
                });
        }

        return result;
    }

    constexpr std::vector<RowCol> GetNeighborsNoSlopes(RowCol pos, const std::vector<std::string>& lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        const auto h = RowCol{ 0, 1 };
        const auto v = RowCol{ 1, 0 };
        std::vector<RowCol> result;
        const auto neighbors = GetDirectNeighbors(pos, limits);
        std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](RowCol rc) {
            return lines[rc.Row][rc.Col] != '#';
            });

        return result;
    }

    constexpr size_t FindMaxPath(const std::vector<std::string>& lines, auto NFunc) {
        const RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        const RowCol start = { 0, 1 };
        const RowCol end = { limits.Row, limits.Col - 1 };

        struct State {
            RowCol Pos;
            Constexpr::BigSet<RowCol, 10000> Path;
        };
        std::vector<State> q{ {start, {}} };
        
        size_t best = 0;
        State current;
        while (!q.empty()) {
            current = q.back();
            q.pop_back();

            if (current.Pos == end) {
                best = std::max(best, current.Path.size());
                continue;
            }

            auto ns = NFunc(current.Pos, lines);
            auto next = current;
            next.Path.insert(current.Pos);
            for (auto n : ns) {
                if (current.Path.contains(n)) continue;
                next.Pos = n;
                q.push_back(next);
            }
        }

        return best;
    }
    PART_ONE() {
        return Constexpr::ToString(FindMaxPath(lines, GetNeighbors));
    }

    constexpr bool IsTerminus(const std::vector<std::string>& lines, RowCol pos) {
        return pos.Row == 0 || pos.Row == lines.size() - 1;
    }

    constexpr bool IsNode(const std::vector<std::string>& lines, RowCol pos) {
        auto IsPath = [&](RowCol rc) {
            return lines[rc.Row][rc.Col] != '#';
            };

        if (!IsPath(pos)) return false;

        const RowCol h = { 0, 1 };
        const RowCol v = { 1, 0 };

        size_t exits = 0;
        exits += IsPath(pos - h);
        exits += IsPath(pos + h);
        exits += IsPath(pos - v);
        exits += IsPath(pos + v);

        return exits > 2;
    }

    constexpr std::vector<RowCol> FindNodes(const std::vector<std::string>& lines) {
        std::vector<RowCol> result;

        RowCol rc;
        for (size_t row = 1; row < lines.size() - 1; row++) {
            for (size_t col = 1; col < lines[0].size() - 1; col++) {
                rc.Row = row;
                rc.Col = col;
                if(IsNode(lines, rc)) {
                    result.push_back(rc);
                }
            }
        }

        return result;
    }

    std::pair<RowCol, size_t> TracePath(const std::vector<std::string>& lines, RowCol current, RowCol prev) {
        size_t steps = 1;
        RowCol next = current;
        while (!IsTerminus(lines, current) && !IsNode(lines, current)) {
            auto ns = GetNeighborsNoSlopes(current, lines);
            next = ns[0] == prev ? ns[1] : ns[0];
            prev = current;
            current = next;
            steps++;
        }

        return std::make_pair(current, steps);
    }

    using DistMap = Constexpr::SmallMap<RowCol, Constexpr::SmallMap<RowCol, size_t>>;
    constexpr DistMap BuildEdges(const std::vector<std::string>& lines, std::vector<RowCol>& nodes) {
        DistMap map;

        for (auto node : nodes) {
            auto ns = GetNeighborsNoSlopes(node, lines);
            for (auto n : ns) {
                auto [to, dist] = TracePath(lines, n, node);

                if (dist > 0) {
                    map[node][to] = dist;
                    map[to][node] = dist;
                }
            }
        }

        return map;
    }

    constexpr size_t GetPathLength(const std::vector<RowCol>& path, const DistMap& map) {
        size_t result = 0;
        for (size_t i = 1; i < path.size(); i++) {
            result += map.at(path[i - 1]).at(path[i]);
        }

        return result;
    }

    PART_TWO() {
        auto nodes = FindNodes(lines);
        auto map = BuildEdges(lines, nodes);

        struct State {
            RowCol Pos;
            std::vector<RowCol> Path;
        };

        RowCol start{ 0, 1 };
        RowCol end{ lines.size() - 1, lines[0].size() - 2 };

        std::vector<State> q{ {start, {start} } };
        
        size_t best = 0;

        while (!q.empty()) {
            auto current = q.back();
            q.pop_back();

            if (current.Pos == end) {
                best = std::max(best, GetPathLength(current.Path, map));
                continue;
            }

            auto ns = map.at(current.Pos).GetKeys();
            for (auto n : ns) {
                if (std::find(current.Path.begin(), current.Path.end(), n) != current.Path.end()) continue;
                auto next = current;
                next.Path.push_back(n);
                next.Pos = n;
                q.push_back(next);
            }
        }

        return Constexpr::ToString(best);
    }

    TESTS() {
        std::vector<std::string> example = {
            "#.#####################",
            "#.......#########...###",
            "#######.#########.#.###",
            "###.....#.>.>.###.#.###",
            "###v#####.#v#.###.#.###",
            "###.>...#.#.#.....#...#",
            "###v###.#.#.#########.#",
            "###...#.#.#.......#...#",
            "#####.#.#.#######.#.###",
            "#.....#.#.#.......#...#",
            "#.#####.#.#.#########v#",
            "#.#...#...#...###...>.#",
            "#.#.#v#######v###.###v#",
            "#...#.>.#...>.>.#.###.#",
            "#####v#.#.###v#.#.###.#",
            "#.....#...#...#.#.#...#",
            "#.#########.###.#.#.###",
            "#...###...#...#...#.###",
            "###.###.#.###v#####v###",
            "#...#...#.#.>.>.#.>.###",
            "#.###.###.#.###.#.#v###",
            "#.....###...###...#...#",
            "#####################.#"
        };

        if (FindMaxPath(example, GetNeighbors) != 94) return false;
        if (PartTwo(example) != "154") return false;

        return true;
    }
}