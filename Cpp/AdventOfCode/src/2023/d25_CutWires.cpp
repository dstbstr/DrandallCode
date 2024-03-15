#include "2023/d25_CutWires.h"

SOLUTION(2023, 25) {    
    using Graph = std::vector<std::vector<int>>;
    using Edges = std::vector<std::pair<int, int>>;

    constexpr std::pair<Edges, size_t> CreateEdges(const std::vector<std::string>& lines) {
        Edges result;
        Constexpr::SmallMap<int, int> ids;

        int index = 0;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, ":");
            auto s2 = Constexpr::Split(s[1], " ");
            auto lhs = Constexpr::FromBase26<int>(s[0]);
            if (!ids.contains(lhs)) ids[lhs] = index++;

            auto a = ids[lhs];

            for (auto r : s2) {
                auto rhs = Constexpr::FromBase26<int>(r);
                if (!ids.contains(rhs)) ids[rhs] = index++;
                auto b = ids[rhs];
                result.push_back(std::make_pair(a, b));
            }
        }

        return std::make_pair(result, static_cast<size_t>(index));
    }

    constexpr Graph CreateGraph(const Edges& edges, size_t n) {
        Graph result{ n };
        for (auto [u, v] : edges) {
            result[u].push_back(v);
            result[v].push_back(u);
        }
        return result;
    }

    constexpr std::vector<std::vector<int>> CalcMinDistances(const Graph& graph, size_t n) {
        auto unset = std::numeric_limits<int>::max();
        std::vector<std::vector<int>> result{ n, std::vector<int>(n, unset) };

        auto bfs = [&](int start) {
            Constexpr::Queue<int> q;
            q.push(start);
            result[start][start] = 0;
            while (!q.is_empty()) {
                auto u = q.front(); q.pop();
                for (auto v : graph[u]) {
                    if (result[start][v] != unset) continue;
                    result[start][v] = result[start][u] + 1;
                    q.push(v);
                }
            }

            };
        for (auto i = 0; i < n; i++) bfs(i);
        return result;
    }

    constexpr bool CouldBeBridge(int u, int v, size_t n, const std::vector<std::vector<int>>& dist) {
        int tries = 300, failures = 0;
        for (int i = 0; i < tries; i++) {
            int target = rand() % n;
            failures += 0 == Constexpr::AbsDistance(dist[u][target], dist[v][target]);
        }

        return failures <= 15; //0.05 * tries
    }

    constexpr int CountGroup(int v, const Graph& graph) {
        Constexpr::SmallSet<int> seen;

        std::vector<int> next{ v };
        while (!next.empty()) {
            auto current = next.back(); next.pop_back();
            for (auto n : graph[current]) {
                if (seen.insert(n)) {
                    next.push_back(n);
                }
            }
        }

        return static_cast<int>(seen.size());
    }

    constexpr int Solve(const std::vector<std::string>& lines) {
        auto [edges, n] = CreateEdges(lines);
        auto graph = CreateGraph(edges, n);
        auto dist = CalcMinDistances(graph, n);

        auto addEdge = [&graph](int u, int v) {
            graph[u].push_back(v);
            graph[v].push_back(u);
            };
        auto removeEdge = [&graph](int u, int v) {
            graph[u].erase(std::find(graph[u].begin(), graph[u].end(), v));
            graph[v].erase(std::find(graph[v].begin(), graph[v].end(), u));
            };

        for (auto i = 0; i < edges.size(); i++) {
            auto [u1, v1] = edges[i];
            if (!CouldBeBridge(u1, v1, n, dist)) continue;
            removeEdge(u1, v1);

            for (auto j = i + 1; j < edges.size(); j++) {
                auto [u2, v2] = edges[j];
                if (!CouldBeBridge(u2, v2, n, dist)) continue;
                removeEdge(u2, v2);

                for (auto k = j + 1; k < edges.size(); k++) {
                    auto [u3, v3] = edges[k];
                    if (!CouldBeBridge(u3, v3, n, dist)) continue;
                    removeEdge(u3, v3);

                    //Solved
                    auto g1 = CountGroup(0, graph);
                    if (g1 < n) {
                        return g1 * (static_cast<int>(n) - g1);
                    }
                }
            }
        }

        return 0;
    }
    
    PART_ONE() {
        return Constexpr::ToString(Solve(lines));
    }

    PART_TWO() {
        (void)lines;
        return "Merry Christmas";
    }

    TESTS() {
        return true;
    }
}