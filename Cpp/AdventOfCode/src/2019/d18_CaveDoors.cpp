#include "2019/d18_CaveDoors.h"
#include <bit>

SOLUTION(2019, 18) {
    constexpr void SplitIntoRooms(std::vector<std::string>&lines) {
        RowCol origin{ 0, 0 };
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '@') {
                    origin = { row, col };
                    break;
                }
            }
        }

        lines[origin.Row][origin.Col] = '#';
        lines[origin.Row - 1][origin.Col] = '#';
        lines[origin.Row + 1][origin.Col] = '#';
        lines[origin.Row][origin.Col - 1] = '#';
        lines[origin.Row][origin.Col + 1] = '#';
        lines[origin.Row - 1][origin.Col - 1] = '@';
        lines[origin.Row - 1][origin.Col + 1] = '@';
        lines[origin.Row + 1][origin.Col - 1] = '@';
        lines[origin.Row + 1][origin.Col + 1] = '@';
    }

    struct KeyCollection {
        constexpr KeyCollection() = default;

        //TODO: move this to ConstexprBits?
        constexpr bool IsProperSubsetOf(KeyCollection other) const {
            return State != other.State && ((State & other.State) == State);
        }

        constexpr bool Contains(char key) const {
            return Constexpr::IsBitSet(State, key - 'a');
        }

        constexpr bool Contains(KeyCollection other) const {
            return (State & other.State) == other.State;
        }

        constexpr size_t CountMissingFrom(KeyCollection other) const {
            return std::popcount(State ^ other.State);
        }

        constexpr KeyCollection operator+(char key) const {
            auto result = *this;
            Constexpr::SetBit(result.State, key - 'a');
            return result;
        }

        constexpr KeyCollection operator+(KeyCollection key) const {
            return KeyCollection(State | key.State);
        }

        constexpr KeyCollection operator-(char key) const {
            auto result = *this;
            Constexpr::UnsetBit(result.State, key - 'a');
            return result;
        }

        constexpr bool operator==(KeyCollection other) const {
            return State == other.State;
        }
        constexpr bool operator<(KeyCollection other) const {
            return State < other.State;
        }

    private:
        constexpr KeyCollection(u32 state) : State(state) {}
        u32 State{ 0 };
        friend struct KeyHash;
    };

    struct KeyHash {
        constexpr size_t operator()(const KeyCollection& keys) const {
            return mHash(keys.State);
        }
        Constexpr::Hasher<u32> mHash{};
    };

    struct Path {
        std::vector<RowCol> Bots;
        KeyCollection Keys{};
        size_t Steps{ 0 };

        constexpr bool operator==(const Path& other) const {
            return Keys == other.Keys && Bots == other.Bots;
        }
        constexpr bool operator<(const Path& other) const {
            return other.Steps < Steps;
        }
    };

    struct PathHash {
        constexpr size_t operator()(const Path& path) const {
            return mHash(path.Keys);
        }
        KeyHash mHash{};
    };

    constexpr bool IsKey(char c) {
        return c >= 'a' && c <= 'z';
    }
    constexpr bool IsDoor(char c) {
        return c >= 'A' && c <= 'Z';
    }
    constexpr bool IsWall(char c) {
        return c == '#';
    }
    constexpr bool IsBotPos(char c) {
        return c == '@' || (c >= '0' && c <= '9');
    }
    constexpr char ToKey(char door) {
        return door + 32;
    }

    class Map {
    public:
        constexpr Map(const std::vector<std::string>& lines)
            : mGrid(lines)
            , mLimits({ lines.size() - 1, lines[0].size() - 1 })
        {
            u32 currentStart = 0;
            for (size_t row = 0; row < mGrid.size(); row++) {
                for (size_t col = 0; col < mGrid[row].size(); col++) {
                    auto& c = mGrid[row][col];
                    if (c == '@') {
                        c = static_cast<char>('0' + currentStart++);
                        mKeyNodes[c] = { row, col };
                    }
                    else if (IsKey(c)) {
                        AllKeys = AllKeys + c;
                        mKeyNodes[c] = { row, col };
                    }
                }
            }
        }

        constexpr size_t Solve() {
            Constexpr::PriorityQueue<Path> q;
            Constexpr::BigSet<Path, 1'000'000, PathHash> seen;

            Path initial{ {}, NoKeys, 0 };
            for (const auto& [node, pos] : mKeyNodes) {
                if (!IsKey(node)) {
                    initial.Bots.push_back(pos);
                }
            }
            q.push(initial);

            while (!q.empty()) {
                auto current = q.pop();

                if (current.Keys == AllKeys) {
                    return current.Steps;
                }
                if (!seen.insert(current)) continue;

                auto neighbors = GetEdgesFrom(current);
                for (const auto& n : neighbors) {
                    if (seen.contains(n)) continue;
                    q.push_or_update(n);
                }
            }

            return 0ull;
        }
    private:
        std::vector<std::string> mGrid;
        RowCol mLimits{};
        Constexpr::SmallSet<RowCol> mSeen{};
        Constexpr::SmallMap<RowCol, std::vector<Path>> mEdgeCache{};
        Constexpr::SmallMap<char, RowCol> mKeyNodes;

        KeyCollection AllKeys{};
        KeyCollection NoKeys{};

        constexpr char Get(RowCol rc) const {
            return mGrid[rc.Row][rc.Col];
        }

        //Given bots {a,b,c,d}, find all moves for each {a,b,c,d} with given keys

        constexpr std::vector<Path> GetEdgesFrom(const Path& from) {
            std::vector<Path> result;
            for (size_t botIndex = 0; botIndex < from.Bots.size(); botIndex++) {
                auto bot = from.Bots[botIndex];
                if (!mEdgeCache.contains(bot)) {
                    mEdgeCache[bot] = GetAllEdgePaths(from, botIndex);
                }

                for (const auto& to : mEdgeCache[bot]) {
                    auto key = Get(to.Bots[botIndex]);
                    if (from.Keys.Contains(to.Keys) && !from.Keys.Contains(key)) {
                        auto next = Path{ from.Bots, from.Keys + key, from.Steps + to.Steps };
                        next.Bots[botIndex] = to.Bots[botIndex];
                        result.push_back(next);
                    }
                }
            }

            return result;
        }

        constexpr void Recurse(const Path pos, size_t botIndex, auto IsDone) {
            auto rc = pos.Bots[botIndex];
            auto current = Get(rc);
            if (IsKey(current) && pos.Steps > 0) {
                IsDone(pos);
            }
            mSeen.insert(rc);
            for (auto n : GetDirectNeighbors(rc, mLimits)) {
                auto c = Get(n);
                if (c == '#' || mSeen.contains(n)) continue;
                auto next = pos;
                next.Bots[botIndex] = n;
                next.Keys = IsDoor(c) ? next.Keys + ToKey(c) : next.Keys;
                next.Steps++;
                Recurse(next, botIndex, IsDone);
            }
            mSeen.erase(rc);
        }

        constexpr void Recurse(const Path& from, size_t botIndex, size_t pathLength, KeyCollection keys, auto IsDone) {
            auto pos = from.Bots[botIndex];
            auto current = Get(pos);
            if (IsKey(current) && pathLength > 0) {
                IsDone(from);
            }
            mSeen.insert(pos);
            for (auto n : GetDirectNeighbors(pos, mLimits)) {
                auto c = Get(n);
                if (c == '#' || mSeen.contains(n)) continue;
                auto nextKeys = IsDoor(c) ? keys + ToKey(c) : keys;
                auto next = from;
                next.Bots[botIndex] = n;
                next.Keys = nextKeys;
                Recurse(next, botIndex, pathLength + 1, nextKeys, IsDone);
            }
            mSeen.erase(pos);
        }

        constexpr std::vector<Path> GetAllEdgePaths(const Path& from, size_t botIndex) {
            std::vector<Path> result;
            auto IsDone = [&](const Path& to) {
                if (from.Keys.Contains(Get(to.Bots[botIndex]))) return false;

                bool found = false;
                for (auto& existing : result) {
                    if (existing != to) continue;
                    found = true;
                    if (to.Keys.IsProperSubsetOf(existing.Keys)) {
                        existing = to;
                        return true;
                    }
                    else if (to.Keys == existing.Keys && to.Steps < existing.Steps) {
                        existing = to;
                        return true;
                    }
                }
                if (!found) {
                    result.push_back(to);
                    return true;
                }
                return false;
            };

            auto initial = from;
            initial.Keys = NoKeys;
            initial.Steps = 0;
            Recurse(initial, botIndex, IsDone);

            return result;
        }
    };


    PART_ONE() {
        Map solution(lines);
        return Constexpr::ToString(solution.Solve());
    }

    PART_TWO() {
        auto copy = lines;
        SplitIntoRooms(copy);

        Map solution(copy);
        return Constexpr::ToString(solution.Solve());
    }

    TESTS() {
        std::vector<std::string> lines = {
            "#########",
            "#b.A.@.a#",
            "#########"
        };

        if (PartOne(lines) != "8") return false;

        lines = {
            "########################",
            "#f.D.E.e.C.b.A.@.a.B.c.#",
            "######################.#",
            "#d.....................#",
            "########################",
        };

        if (PartOne(lines) != "86") return false;

        lines = {
            "########################",
            "#...............b.C.D.f#",
            "#.######################",
            "#.....@.a.B.c.d.A.e.F.g#",
            "########################"
        };
        if (PartOne(lines) != "132") return false;

        lines = {
            "#######",
            "#a.#Cd#",
            "##...##",
            "##.@.##",
            "##...##",
            "#cB#Ab#",
            "#######"
        };
        if (PartTwo(lines) != "8") return false;

        lines = {
            "#############",
            "#DcBa.#.GhKl#",
            "#.###...#I###",
            "#e#d#.@.#j#k#",
            "###C#...###J#",
            "#fEbA.#.FgHi#",
            "#############"
        };
        if (PartTwo(lines) != "32") return false;

        lines = {
            "#############",
            "#g#f.D#..h#l#",
            "#F###e#E###.#",
            "#dCba...BcIJ#",
            "#####.@.#####",
            "#nK.L...G...#",
            "#M###N#H###.#",
            "#o#m..#i#jk.#",
            "#############"
        };
        if (PartTwo(lines) != "72") return false;

        return true;
    }
}
