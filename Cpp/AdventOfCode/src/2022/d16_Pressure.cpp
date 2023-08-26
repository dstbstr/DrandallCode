#include "2022/d16_Pressure.h"
#include "Algorithms/FloydWarshall.h"

SOLUTION(2022, 16) {

    struct Room {
        std::string Name = "";
        u32 FlowRate = 0;
        std::vector<std::string> Connections{};
    };

    template<size_t RoomCount, typename TDistance>
    struct State {
        std::vector<Room> Rooms{};
        std::unordered_map<std::string, size_t> IndexMap{};

        std::array<std::array<TDistance, RoomCount>, RoomCount> DistanceMap{};
    };

    //Valve XK has flow rate=15; tunnels lead to valves CD, JC, TP, UE
    constexpr Room ParseRoom(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        Room room;
        room.Name = std::string(s[1]);
        
        auto flowRate = Constexpr::Split(s[4], "=")[1];
        Constexpr::ParseNumber(flowRate, room.FlowRate);

        for (size_t i = 9; i < s.size(); i++) {
            room.Connections.push_back(std::string(s[i].substr(0, s[i].size() - 1)));
        }

        return room;
    }
    /*
    std::vector<Room> ParseRoomList(const auto& lines) {
        static const auto re = std::regex(R"(Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? ([\w, ]+))");
        std::smatch match;
        std::vector<Room> rooms;

        for (const auto& line : lines) {
            std::regex_search(line, match, re);
            Room room;
            room.Name = match[1].str();
            Constexpr::ParseNumber(match[2].str(), room.FlowRate);
            auto tunnels = match[3].str();
            auto split = StrUtil::Split(tunnels, ", ");
            for (auto connection : split) {
                room.Connections.push_back(std::string(connection));
            }

            rooms.push_back(room);
        }

        return rooms;
    }
    */

    std::unordered_map<std::string, size_t> GetIndexMap(const std::vector<Room>&rooms) {
        std::unordered_map<std::string, size_t> indexMap;

        for (size_t i = 0; i < rooms.size(); i++) {
            indexMap[rooms.at(i).Name] = i;
        }

        return indexMap;
    }

    template<size_t Verts, typename TDistance>
    constexpr std::array<std::array<TDistance, Verts>, Verts> GetDistanceMap(const std::vector<Room>&rooms, const std::unordered_map<std::string, size_t>&indexMap) {
        std::array<std::array<TDistance, Verts>, Verts> result;

        for (size_t i = 0; i < Verts; i++) {
            std::fill(result.at(i).begin(), result.at(i).end(), static_cast<TDistance>(99));
        }

        for (size_t i = 0; i < rooms.size(); i++) {
            auto room = rooms.at(i);
            for (const auto& connection : room.Connections) {
                auto a = indexMap.at(room.Name);
                auto b = indexMap.at(connection);

                result[a][a] = 0;
                result[b][b] = 0;
                result[a][b] = 1;
                result[b][a] = 1;
            }
        }
        FloydWarshall(result);

        return result;

    }

    template<size_t RoomCount, typename TDistance>
    State<RoomCount, TDistance> ParseState(const auto& lines) {
        State<RoomCount, TDistance> state;
        //state.Rooms = ParseRoomList(lines);
        state.Rooms = ParseLines(lines, ParseRoom);
        auto allIndexes = GetIndexMap(state.Rooms);
        state.DistanceMap = GetDistanceMap<RoomCount, TDistance>(state.Rooms, allIndexes);

        for (const auto& [name, index] : allIndexes) {
            if (state.Rooms[index].FlowRate > 0 || name == "AA") {
                state.IndexMap.insert({ name, index });
            }
        }

        return state;
    }

    template<size_t RoomCount, typename TDistance>
    constexpr void RecursePermutations(const State<RoomCount, TDistance>&state, u64 seen, size_t roomIndex, s16 remainingTime, u32 totalSteam, Constexpr::SmallMap<u64, u32>&outMap) {
        static const u64 done = [&state]() {
            u64 result = 0;
            for (const auto& [_, index] : state.IndexMap) {
                result += u64(1) << index;
            }

            return result;
        }();

        if (remainingTime <= 0) {
            if (outMap.contains(seen)) {
                if (outMap.at(seen) < totalSteam) {
                    outMap.at(seen) = totalSteam;
                }
            }
            else {
                outMap[seen] = totalSteam;
            }

            return;
        }

        totalSteam += static_cast<u32>(state.Rooms[roomIndex].FlowRate * remainingTime);
        seen += (u64(1) << roomIndex);

        if (seen == done) {
            if (outMap.contains(seen)) {
                if (outMap.at(seen) < totalSteam) {
                    outMap.at(seen) = totalSteam;
                }
            }
            else {
                outMap[seen] = totalSteam;
            }
            return;
        }

        for (const auto& [nextRoomName, nextIndex] : state.IndexMap) {
            if (seen & (u64(1) << nextIndex)) continue;
            auto distance = state.DistanceMap.at(roomIndex).at(nextIndex) + 1;
            auto nextTime = static_cast<s16>(remainingTime - distance);

            RecursePermutations(state, seen, nextIndex, nextTime, totalSteam, outMap);
        }
    }

    template<size_t RoomCount, typename TDistance>
    constexpr u32 FindBestSolo(const auto& lines) {
        auto state = ParseState<RoomCount, TDistance>(lines);
        auto startIndex = state.IndexMap.at("AA");
        Constexpr::SmallMap<u64, u32> permutations;

        RecursePermutations<RoomCount, TDistance>(state, 0, startIndex, 30, 0, permutations);
        std::vector<std::pair<u64, u32>> pairs;
        for (const auto& [valves, value] : permutations) {
            auto withoutStart = valves - (u64(1) << startIndex);
            pairs.push_back(std::make_pair(withoutStart, value));
        }

        std::sort(pairs.begin(), pairs.end(), [](const std::pair<u64, u32>& lhs, const std::pair<u64, u32>& rhs) {
            return lhs.second > rhs.second;
            });

        return pairs[0].second;
    }

    template<size_t RoomCount, typename TDistance>
    constexpr u32 FindBestPair(const auto&lines) {
        auto state = ParseState<RoomCount, TDistance>(lines);
        auto startIndex = state.IndexMap.at("AA");
        Constexpr::SmallMap<u64, u32> permutations;

        RecursePermutations<RoomCount, TDistance>(state, 0, startIndex, 26, 0, permutations);

        std::vector<std::pair<u64, u32>> pairs;
        for (const auto& [valves, value] : permutations) {
            auto withoutStart = valves - (u64(1) << startIndex);
            pairs.push_back(std::make_pair(withoutStart, value));
        }

        std::sort(pairs.begin(), pairs.end(), [](const std::pair<u64, u32>& lhs, const std::pair<u64, u32>& rhs) {
            return lhs.second > rhs.second;
            });

        u32 best = 0;
        for (auto i = 0; i < pairs.size(); i++) {
            for (auto j = i + 1; j < pairs.size(); j++) {
                if ((pairs[i].first & pairs[j].first) == 0) {
                    best = std::max(best, pairs[i].second + pairs[j].second);
                }
            }
        }
        return best;
    }

    PART_ONE() {
        return Constexpr::ToString(FindBestSolo<51, u8>(Lines));
    }

    PART_TWO() {
        return Constexpr::ToString(FindBestPair<51, u8>(Lines));
    }

    TESTS() {
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

        if (FindBestSolo<10, u8>(lines) != 1651) return false;
        //if (FindBestPair<10, u8>(lines) != 1707) return false;

        return true;
    }
}