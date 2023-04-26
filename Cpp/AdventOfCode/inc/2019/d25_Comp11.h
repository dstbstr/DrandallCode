#pragma once

#include "Common.h"
#include "Comp.h"
#include "AStar.h"

constexpr s64 Consumed = -9919;

struct Room {
    std::string Name;
    std::vector<Direction> Doors;
    std::vector<std::string> Items;
    Coord Pos{ 0, 0 };
    bool IsBlank = false;

    constexpr bool operator==(const Room& other) const {
        return other.Pos == Pos;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Room& room) {
        stream << room.Pos << "\n";
        stream << room.Name << "\nDoors: ";
        for (const auto& door : room.Doors) {
            stream << ToString(door);
            stream << " ";
        }
        stream << "\nItems: " << StrUtil::JoinVec(", ", room.Items) << "\n";
        return stream;
    }
};

struct RoomHash {
    size_t operator()(const Room& room) const {
        return std::hash<std::string>()(room.Name);
    }
};

class Droid {
    std::vector<s64> Instructions;
    Args Args{};

    void IssueCommand(const std::string& cmd) {
        static const s64 newline = 10;

        for (const auto& c : cmd) {
            Args.Inputs.push_back(c);
        }
        Args.Inputs.push_back(newline);

        while (!HasConsumedAllInput(Args)) {
            if (!Apply(Instructions, Args)) return;
        }
    }
public:
    Coord Pos{ 0, 0 };

    Droid(const std::string& line) {
        Instructions = ParseInstructions(line);
        Args.Output = Consumed;
    }

    Droid(const Droid& other) {
        Instructions = other.Instructions;
        Args = other.Args;
        Pos = other.Pos;
    }

    std::vector<std::string> ReadOutput() {
        std::vector<std::string> result;
        std::string currentString;
        bool CommandRequested = false;
        while (!CommandRequested) {
            while (!NeedsInput(Instructions, Args)) {
                if (!Apply(Instructions, Args)) return result;

                if (Args.Output != Consumed) {
                    auto c = static_cast<char>(Args.Output);
                    if (c == '\n') {
                        result.push_back(currentString);
                        currentString.clear();
                    }
                    else {
                        currentString.push_back(c);
                    }
                    CommandRequested |= c == '?';
                    Args.Output = Consumed;
                }
            }
            if (CommandRequested) break;
            IssueCommand("");
        }

        result.push_back(currentString);
        return result;
    }

    void PrintOutput() {
        auto output = ReadOutput();
        for (const auto& line : output) {
            std::cout << line << "\n";
        }
    }

    void Move(Direction dir) {
        switch (dir) {
        case Up: IssueCommand("north"); break;
        case Down: IssueCommand("south"); break;
        case Left: IssueCommand("west"); break;
        case Right: IssueCommand("east"); break;
        }
    }

    void PickUp(const std::string& item) {
        IssueCommand("take " + item);
    }

    void Drop(const std::string& item) {
        IssueCommand("drop " + item);
    }

    void PrintInventory() {
        IssueCommand("inv");
    }
};

using CoordMap = std::unordered_map<Coord, Room>;
using NameMap = std::unordered_map<std::string, Room>;

CoordMap GetCoordMap() {
    CoordMap coordMap;
    coordMap[{0, 1}] = Room{ "Gift Wrapping Center", {Down}, {}, {0, 1} };
    coordMap[{0, 2}] = Room{ "Blank 1", {Up, Down}, {}, {0, 2}, true };
    coordMap[{0, 3}] = Room{ "Holodeck", {Up, Right, Down}, {"manifold"}, {0, 3} };
    coordMap[{0, 4}] = Room{ "Hot Chocolate Fountain", {Up}, {"photons"}, {0, 4} };
    coordMap[{1, 0}] = Room{ "Arcade", {Down}, {"loom"}, {1, 0} };
    coordMap[{1, 1}] = Room{ "Stables", {Up, Right, Down}, {"space law space brochure"}, {1, 1} };
    coordMap[{1, 2}] = Room{ "Blank 2", {Up, Down}, {}, {1, 2}, true };
    coordMap[{1, 3}] = Room{ "Corridor", {Up, Down, Left}, {"hologram"}, {1, 3} };
    coordMap[{1, 4}] = Room{ "Storage", {Up}, {"molten lava"}, {1, 4} };
    coordMap[{2, 0}] = Room{ "Passages", {Right}, {"escape pod"}, {2, 0} };
    coordMap[{2, 1}] = Room{ "Blank 3", {Right, Left}, {}, {2, 1}, true };
    coordMap[{2, 2}] = Room{ "Warp Drive Maintenance", {Down}, {"giant electromagnet"}, {2, 2} };
    coordMap[{2, 3}] = Room{ "Hallway", {Up, Right, Down}, {}, {2, 3} };
    coordMap[{2, 4}] = Room{ "Science Lab", {Up, Down}, {"easter egg"}, {2, 4} };
    coordMap[{2, 5}] = Room{ "Security Checkpoint", {Up, Down}, {}, {2, 5} };
    coordMap[{2, 6}] = Room{ "Sensor Room", {Up}, {}, {2, 6} };
    coordMap[{3, 0}] = Room{ "Kitchen", {Right, Down, Left}, {"mutex"}, {3, 0} };
    coordMap[{3, 1}] = Room{ "Hull Breach", {Up, Down, Left}, {}, {3, 1} };
    coordMap[{3, 2}] = Room{ "Blank 4", {Up, Down}, {}, {3, 2}, true };
    coordMap[{3, 3}] = Room{ "Navigation", {Up, Down, Left}, {"cake"}, {3, 3} };
    coordMap[{3, 4}] = Room{ "Observatory", {Up}, {"infinite loop"}, {3, 4} };
    coordMap[{4, 0}] = Room{ "Sick Bay", {Left, Right}, {}, {4, 0} };
    coordMap[{5, 0}] = Room{ "Engineering", {Left, Right}, {}, {5, 0} };
    coordMap[{6, 0}] = Room{ "Crew Quarters", {Left}, {"whirled peas"}, {6, 0} };
    
    return coordMap;
}

NameMap GenerateNameMap(const CoordMap& coordMap) {
    NameMap result;
    for (const auto& [pos, room] : coordMap) {
        result[room.Name] = room;
    }

    return result;
}


Direction FindMoveDirection(const Room& from, const Room& to) {
    if (from.Pos.X == to.Pos.X) {
        return from.Pos.Y < to.Pos.Y ? Down : Up;
    }
    else {
        return from.Pos.X < to.Pos.X ? Right : Left;
    }
}

Coord GetPosFromDirection(const Room& room, Direction dir) {
    switch (dir) {
    case Up: return { room.Pos.X, room.Pos.Y - 1 };
    case Down: return { room.Pos.X, room.Pos.Y + 1 };
    case Left: return { room.Pos.X - 1, room.Pos.Y };
    case Right: return { room.Pos.X + 1, room.Pos.Y };
    default: return room.Pos;
    }
}

void MoveAdjacent(Droid& droid, const Room& from, const Room& to) {
    if (to.IsBlank) return;

    auto dir = FindMoveDirection(from, to);
    droid.Pos = to.Pos;
    droid.Move(dir);
}

void MoveTo(Droid& droid, const Room& to, const CoordMap& map) {
    auto n = [&map](const Coord& pos) {
        std::vector<Coord> result;
        const auto& room = map.at(pos);
        for (const auto dir : room.Doors) {
            result.push_back(GetPosFromDirection(room, dir));
        }

        return result;
    };

    auto path = AStarMin<Coord>(droid.Pos, to.Pos, n);

    for (auto i = 1; i < path.size(); ++i) {
        MoveAdjacent(droid, map.at(path[i - 1]), map.at(path[i]));
    }
}

auto PartOne(const std::string& line) {
    auto droid = Droid(line);

    auto coordMap = GetCoordMap();
    auto nameMap = GenerateNameMap(coordMap);

    droid.Pos = nameMap.at("Hull Breach").Pos;

    std::vector<std::string> roomsToVisit = {
        "Holodeck",
        "Stables",
        //"Arcade",
        "Corridor",
        //"Science Lab",
        "Kitchen"
    };

    for (const auto& targetRoom : roomsToVisit) {
        MoveTo(droid, nameMap.at(targetRoom), coordMap);
        for (const auto& item : nameMap.at(targetRoom).Items) {
            droid.PickUp(item);
        }
    }

    MoveTo(droid, nameMap.at("Sensor Room"), coordMap);
    droid.PrintOutput();

    return 0;
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(PartOne(lines[0]));
}

bool RunTests() {
    return true;
}