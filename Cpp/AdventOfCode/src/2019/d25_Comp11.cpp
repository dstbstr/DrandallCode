#include "2019/d25_Comp11.h"
#include "2019/Comp.h"
#include "Facing.h"

#include "Core/Algorithms/AStar.h"

SOLUTION(2019, 25) {
    struct Room {
        std::string Name;
        std::vector<Facing> Doors;
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
            stream << "\nItems: " << Constexpr::JoinVec(", ", room.Items) << "\n";
            return stream;
        }
    };

    struct RoomHash {
        constexpr size_t operator()(const Room& room) const {
            return mHash(room.Name);
        }

        Constexpr::Hasher<std::string> mHash;
    };

    class Droid {
        std::vector<s64> Instructions;
        Args Args{};

        constexpr void IssueCommand(const std::string& cmd) {
            const s64 newline = 10;

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

        constexpr Droid(std::string_view line) {
            Instructions = ParseInstructions(line);
            Args.Output = Unset;
        }

        constexpr Droid(const Droid& other) {
            Instructions = other.Instructions;
            Args = other.Args;
            Pos = other.Pos;
        }

        constexpr std::vector<std::string> ReadOutput() {
            std::vector<std::string> result;
            std::string currentString;
            bool CommandRequested = false;
            while (!CommandRequested) {
                while (!NeedsInput(Instructions, Args)) {
                    if (!Apply(Instructions, Args)) return result;

                    if (Args.Output != Unset) {
                        auto c = static_cast<char>(Args.Output);
                        if (c == '\n') {
                            result.push_back(currentString);
                            currentString.clear();
                        }
                        else {
                            currentString.push_back(c);
                        }
                        CommandRequested |= c == '?';
                        Args.Output = Unset;
                    }
                }
                if (CommandRequested) break;
                IssueCommand("");
            }

            result.push_back(currentString);
            return result;
        }

        constexpr void PrintOutput() {
            auto output = ReadOutput();
            for (const auto& line : output) {
                GET_LOGS().push_back(line);
            }
        }

        constexpr void Move(Facing dir) {
            switch (dir) {
            case Up: IssueCommand("north"); break;
            case Down: IssueCommand("south"); break;
            case Left: IssueCommand("west"); break;
            case Right: IssueCommand("east"); break;
            }
        }

        constexpr void PickUp(const std::string& item) {
            IssueCommand("take " + item);
        }

        constexpr void Drop(const std::string& item) {
            IssueCommand("drop " + item);
        }

        constexpr void PrintInventory() {
            IssueCommand("inv");
        }
    };

    using CoordMap = Constexpr::SmallMap<Coord, Room>;
    using NameMap = Constexpr::SmallMap<std::string, Room>;

    constexpr CoordMap GetCoordMap() {
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

    constexpr NameMap GenerateNameMap(const CoordMap & coordMap) {
        NameMap result;
        for (const auto& [pos, room] : coordMap) {
            result[room.Name] = room;
        }

        return result;
    }


    constexpr Facing FindMoveDirection(const Room & from, const Room & to) {
        if (from.Pos.X == to.Pos.X) {
            return from.Pos.Y < to.Pos.Y ? Down : Up;
        }
        else {
            return from.Pos.X < to.Pos.X ? Right : Left;
        }
    }

    constexpr Coord GetPosFromDirection(const Room & room, Facing dir) {
        switch (dir) {
        case Up: return { room.Pos.X, room.Pos.Y - 1 };
        case Down: return { room.Pos.X, room.Pos.Y + 1 };
        case Left: return { room.Pos.X - 1, room.Pos.Y };
        case Right: return { room.Pos.X + 1, room.Pos.Y };
        default: return room.Pos;
        }
    }

    constexpr void MoveAdjacent(Droid & droid, const Room & from, const Room & to) {
        if (to.IsBlank) return;

        auto dir = FindMoveDirection(from, to);
        droid.Pos = to.Pos;
        droid.Move(dir);
    }

    constexpr void MoveTo(Droid & droid, const Room & to, const CoordMap & map) {
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

    PART_ONE() {
        auto droid = Droid(lines[0]);

        auto coordMap = GetCoordMap();
        auto nameMap = GenerateNameMap(coordMap);

        droid.Pos = nameMap.at("Hull Breach").Pos;

        std::vector<std::string> roomsToVisit = {
            "Holodeck",
            "Stables",
            "Corridor",
            "Kitchen"
        };

        for (const auto& targetRoom : roomsToVisit) {
            MoveTo(droid, nameMap.at(targetRoom), coordMap);
            for (const auto& item : nameMap.at(targetRoom).Items) {
                droid.PickUp(item);
            }
        }

        MoveTo(droid, nameMap.at("Sensor Room"), coordMap);
        auto output = droid.ReadOutput();
        auto result = "\n" + Constexpr::JoinVec("\n", output);
        return result;
    }

    PART_TWO() {
        (void)lines;
        return "Merry Christmas";
    }

    TESTS() {
        return true;
    }
}