#pragma once

#include "Common.h"

struct Room {
    u32 FlowRate = 0;
    std::vector<std::string> Connections{};
};

using Map = std::unordered_map<std::string, Room>;
using Graph = std::unordered_map<std::string, std::unordered_map<std::string, u32>>;

void ParseLine(const std::string& line, Map& map) {
    static const auto re = std::regex(R"(Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? ([\w, ]+))");
    std::smatch match;

    if (std::regex_search(line, match, re)) {
        Room room;
        ParseNumber(match[2].str(), room.FlowRate);
        auto tunnels = match[3].str();
        auto split = StrUtil::Split(tunnels, ", ");
        for (auto connection : split) {
            room.Connections.push_back(std::string(connection));
        }

        map.insert({ match[1].str(), room });
    }
    else {
        throw std::logic_error("Unable to parse " + line);
    }
}

Map MakeMap(const std::vector<std::string>& lines) {
    Map result;
    for (const auto& line : lines) {
        ParseLine(line, result);
    }

    return result;
}

u32 FindDistance(const std::unordered_map<std::string, Room>& map, const std::string start, const std::string& end, std::unordered_set<std::string> path) {
    auto t = map.at(start).Connections;
    if (std::find(t.begin(), t.end(), end) != t.end()) {
        return 1;
    }
    u32 best = 99;
    for (auto next : t) {
        if (path.find(next) != path.end()) continue;

        path.insert(next);
        best = std::min(best, FindDistance(map, next, end, path) + 1);
        path.erase(next);
    }
    return best;
}


void Recurse(const std::string& current, const Map& map, Graph& graph) {
    if (graph.contains(current)) return;
    auto room = map.at(current);

    for (auto r : room.Connections) {
        graph[current][r] = 1;
        graph[r][current] = 1;
    }
}

Graph MakeGraph(const std::unordered_map<std::string, Room>& map) {
    Graph result;
    std::vector<std::string> names;

    for (const auto& [name, room] : map) {
        if (room.FlowRate > 0) {
            names.push_back(name);
        }
    }
    names.push_back("AA"); //need to keep the starting point

    for (size_t i = 0; i < names.size() - 1; i++) {
        for (size_t j = i + 1; j < names.size(); j++) {
            std::unordered_set<std::string> path;
            auto start = names[i];
            auto end = names[j];
            result[start][end] = FindDistance(map, start, end, path);
            result[end][start] = result[start][end];
        }
    }

    return result;
}

u32 CalculateValue(const std::vector<std::string> path, const Map& map, const Graph& graph, u32 seconds) {
    u32 result = 0;
    u32 remainingTime = seconds;

    for (size_t i = 0; i < path.size() - 1; i++) {
        auto pos = path[i];
        result += map.at(pos).FlowRate * remainingTime;

        auto next = path[i + 1];
        auto distance = graph.at(pos).at(next) + 1;
        remainingTime -= distance;
    }

    result += map.at(path[path.size() - 1]).FlowRate * remainingTime;
    return result;
}

//Only 15 valuable valves
//Given a number of time steps
//Travel between all valves
/*
size_t CountValves(const std::unordered_map<std::string, Room>& map) {
    std::unordered_set<std::string> valves;
    for (const auto& [name, room] : map) {
        valves.insert(name);
    }

    return valves.size();
}

struct Santa {
    std::string CurrentRoom = "";
    std::vector<std::string> OpenValves{};
    u32 TotalSteam = 0;
    u32 SteamPerMinute = 0;
    std::string NextValve = "";
    std::string NextRoom = "";

    constexpr bool operator==(const Santa& other) const = default;

    void Tick(const std::unordered_map<std::string, Room>& map) {
        if (!NextValve.empty()) {
            OpenValves.push_back(NextValve);
            SteamPerMinute += map.at(NextValve).FlowRate;
            NextValve.clear();
        }
        
        if (!NextRoom.empty()) {
            CurrentRoom = NextRoom;
            NextRoom.clear();
        }

        TotalSteam += SteamPerMinute;
    }
};

struct Pair : Santa {
    std::string ECurrentRoom = "";
    std::string ENextValve = "";
    std::string ENextRoom = "";

    constexpr bool operator==(const Pair& other) const = default;

    void Tick(const std::unordered_map<std::string, Room>& map) {
        if (!NextValve.empty()) {
            OpenValves.push_back(NextValve);
            SteamPerMinute += map.at(NextValve).FlowRate;
            NextValve.clear();
        }
        if (!ENextValve.empty()) {
            OpenValves.push_back(ENextValve);
            SteamPerMinute += map.at(ENextValve).FlowRate;
            ENextValve.clear();
        }

        if (!NextRoom.empty()) {
            CurrentRoom = NextRoom;
            NextRoom.clear();
        }
        if (!ENextRoom.empty()) {
            ECurrentRoom = ENextRoom;
            ENextRoom.clear();
        }

        TotalSteam += SteamPerMinute;
    }

};


struct SantaHash {
    size_t operator()(const Santa& d) const {
        return d.TotalSteam ^ d.SteamPerMinute + d.OpenValves.size();
    }
};

struct PairHash {
    size_t operator()(const Pair& p) const {
        return p.TotalSteam ^ p.SteamPerMinute + p.OpenValves.size();
    }
};

u32 FindBestRelease(const std::vector<std::string>& lines, u32 seconds) {
    std::unordered_map<std::string, Room> map;
    for (const auto& line : lines) {
        ParseLine(line, map);
    }
    auto valveCount = CountValves(map);

    std::vector<Santa> current;
    std::unordered_set<Santa, SantaHash> next;

    Santa initial;
    Santa allOpened;

    initial.CurrentRoom = "AA";
    initial.OpenValves.push_back("AA"); //valve of 0, no need to come back here
    current.push_back(initial);
    auto sortDesc = [](const Santa& lhs, const Santa& rhs) -> bool {
        return lhs.TotalSteam > rhs.TotalSteam;
    };

    for (u32 i = 0; i < seconds; i++) {
        allOpened.Tick(map);
        for (auto state : current) {
            state.Tick(map);

            if (state.OpenValves.size() == valveCount) {
                if (allOpened.TotalSteam < state.TotalSteam) {
                    allOpened = state;
                }
                continue;
            }
            if (state.TotalSteam < allOpened.TotalSteam) {
                continue;
            }

            if (std::find(state.OpenValves.cbegin(), state.OpenValves.cend(), state.CurrentRoom) == state.OpenValves.cend()) {
                auto nextState = state;
                nextState.NextValve = nextState.CurrentRoom;
                next.insert(nextState);
            }
            for (auto tunnel : map.at(state.CurrentRoom).Connections) {
                auto nextState = state;
                nextState.NextRoom = tunnel;
                next.insert(nextState);
            }
        }

        current.clear();
        std::copy(next.begin(), next.end(), std::back_inserter(current));
        next.clear();

        //clear out the worst performers
        size_t keepCount = 50000;
        if (current.size() > keepCount) {
            std::sort(current.begin(), current.end(), sortDesc);

            current.erase(current.begin() + keepCount, current.end());
        }
    }

    std::sort(current.begin(), current.end(), sortDesc);
    return current[0].TotalSteam;
}
*/

u32 FindBestPath(const std::vector<std::string>& lines) {
    auto map = MakeMap(lines);
    auto graph = MakeGraph(map);

    std::vector<std::string> path;
    for (const auto& [key, value] : graph) {
        if (key == "AA") continue;
        path.push_back(key);
    }

    std::sort(path.begin(), path.end());
    u32 best = 0;
    while (std::next_permutation(path.begin(), path.end())) {
        path.insert(path.begin(), "AA");
        best = std::max(best, CalculateValue(path, map, graph, 30));
        path.erase(path.begin());
    }

    return best;
}
std::string Run(const std::vector<std::string>& lines) {
    return ToString(FindBestPath(lines));
    //return ToString(FindBestRelease(lines, 30));
    //return ToString(FindBestWithPair(lines, 26));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB",
        "Valve BB has flow rate=13; tunnels lead to valves CC, AA",
        "Valve CC has flow rate=2; tunnels lead to valves DD, BB",
        "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE",
        "Valve EE has flow rate=3; tunnels lead to valves FF, DD",
        "Valve FF has flow rate=0; tunnels lead to valves EE, GG",
        "Valve GG has flow rate=0; tunnels lead to valves FF, HH",
        "Valve HH has flow rate=22; tunnel leads to valve GG",
        "Valve II has flow rate=0; tunnels lead to valves AA, JJ",
        "Valve JJ has flow rate=21; tunnel leads to valve II"
    };

    /*
    auto map = MakeMap(lines);
    auto graph = MakeGraph(map);

    std::vector<std::string> path = {
        "AA", "DD", "BB", "JJ", "HH", "EE", "CC"
    };
    if (CalculateValue(path, map, graph, 30) != 1651) return false;
    */
    if (FindBestPath(lines) != 1651) return false;

    /*
    if (FindBestRelease(lines, 30) != 1651) return false;
    if (FindBestWithPair(lines, 26) != 1707) return false;
    */

    return true;
}