#include "2021/d23_Hanoi.h"
#include <queue>

SOLUTION(2021, 23) {

    /*
    Hallway: 11 spots (but only 7 valid stopping points)
    4 Dens: 2-4 spots each

    Valid Moves:
        Den to Hallway
        Hallway to Final Position

    Costs per step: 1, 10, 100, 1000

    Distance between any 2 points == Manhatten distance
    Path available =
        Hallway between this and den is clear
        Den is clear
    */

    constexpr size_t GetCol(char type) {
        return (type - 'A') * 2 + 3;
    }

    static_assert(GetCol('A') == 3);
    static_assert(GetCol('B') == 5);
    static_assert(GetCol('C') == 7);
    static_assert(GetCol('D') == 9);

    struct Bot {
        RowCol Pos{ 0, 0 };
        char Type = ' ';

        constexpr bool operator<(const Bot& other) const {
            return Pos < other.Pos;
        }
    };

    struct Den {
        size_t Col;
        std::string Occupants;

        constexpr bool Belongs(const Bot& bot) const {
            return GetCol(bot.Type) == Col;
        }

        constexpr bool CanAdd() const {
            for (auto c : Occupants) {
                if (c == '.') continue;
                if (GetCol(c) != Col) return false;
            }
            return true;
        }

        constexpr RowCol NextPos() const {
            for (size_t i = 0; i < Occupants.size(); i++) {
                if (Occupants[i] == '.') {
                    return { 1 + (Occupants.size() - i), Col };
                }
            }

            return { 0, 0 }; //Something went very wrong
        }

        constexpr Bot Top() const {
            for (size_t i = Occupants.size() - 1; i > 0; i--) {
                if (Occupants[i] != '.') {
                    return { {1 + (Occupants.size() - i), Col}, Occupants[i] };
                }
            }

            return { { 1 + Occupants.size(), Col}, Occupants[0] };
        }

        constexpr void Push(const Bot& bot) {
            for (size_t i = 0; i < Occupants.size(); i++) {
                if (Occupants[i] == '.') {
                    Occupants[i] = bot.Type;
                    break;
                }
            }
        }

        constexpr void Pop() {
            for (size_t i = Occupants.size() - 1; i > 0; i--) {
                if (Occupants[i] != '.') {
                    Occupants[i] = '.';
                    return;
                }
            }
            Occupants[0] = '.';
        }
    };


    class Hall {
    public:
        constexpr Hall() = default;
        constexpr Hall(const std::string& cache) {
            Occupants = cache;
        }

        constexpr bool IsClear(size_t from, size_t to) const {
            if (to < from) std::swap(from, to);
            for (size_t p = from + 1; p < to; p++) {
                //#.A -> Occupants[1], Col == 2
                if (Occupants[p - 1] != '.') return false;
            }
            return true;
        }

        constexpr std::vector<RowCol> GetAvailablePositions() const {
            std::vector<RowCol> result;
            if (Occupants[0] == '.') result.push_back({ 1, 1 });
            if (Occupants.back() == '.') result.push_back({ 1, 11 });
            for (size_t i = 1; i < 10; i += 2) {
                if (Occupants[i] == '.') {
                    result.push_back({ 1, i + 1 });
                }
            }

            return result;
        }
        constexpr std::vector<Bot> GetOccupants() const {
            std::vector<Bot> result;
            if (Occupants[0] != '.') result.push_back({ {1, 1}, Occupants[0] });
            if (Occupants.back() != '.') result.push_back({ {1, 11}, Occupants.back() });
            for (size_t i = 1; i < 10; i += 2) {
                if (Occupants[i] != '.') {
                    result.push_back({ {1, i + 1}, Occupants[i] });
                }
            }

            return result;
        }

        constexpr void Occupy(const Bot& bot) {
            Occupants[bot.Pos.Col - 1] = bot.Type;
        }
        constexpr void Remove(const Bot& bot) {
            Occupants[bot.Pos.Col - 1] = '.';
        }

        constexpr std::string ToCacheString() const {
            return Occupants;
        }
    private:
        std::string Occupants = "...........";
    };


    //...........BDDACCBDBBACDACA
    constexpr std::string ToCacheString(const Hall & hall, const std::vector<Den>&dens) {
        std::string result = hall.ToCacheString();
        for (const auto& den : dens) {
            result += den.Occupants;
        }

        return result;
    }

    constexpr void FromCacheString(const std::string & cache, Hall & hall, std::vector<Den>&dens) {
        hall = Hall(cache.substr(0, 11));
        dens.clear();
        auto denSize = (cache.size() - 11) / 4;
        for (size_t idx = 11, col = 3; idx < cache.size(); idx += denSize, col += 2) {
            Den den;
            den.Col = col;
            den.Occupants = cache.substr(idx, denSize);
            dens.push_back(den);
        }
    }

    constexpr std::vector<Bot> GetStartingBots(const std::vector<std::string>&lines) {
        std::vector<Bot> result;
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                auto current = lines[row][col];
                if (current >= 'A' && current <= 'Z') {
                    result.push_back({ {row, col}, current });
                }
            }
        }

        return result;
    }

    constexpr std::vector<Den> GetInitialDens(const std::vector<std::string>&lines, bool isPartTwo) {
        auto copy = lines;
        if (isPartTwo) {
            copy.insert(copy.begin() + 3, "  #D#C#B#A#  ");
            copy.insert(copy.begin() + 4, "  #D#B#A#C#  ");
        }

        std::vector<Den> result;
        for (size_t col = 3; col < 10; col += 2) {
            Den den;
            den.Col = col;
            for (size_t row = 2; row < copy.size() - 1; row++) {
                den.Occupants.push_back(copy[row][col]);
            }
            std::reverse(den.Occupants.begin(), den.Occupants.end()); //make first char the bottom of the den
            result.push_back(den);
        }
        return result;
    }

    constexpr std::string GetFinishedString(const std::vector<Den>&dens) {
        auto denSize = dens[0].Occupants.size();

        std::string result = "...........";
        for (auto bot = 'A'; bot < 'E'; bot++) {
            for (auto spot = 0; spot < denSize; spot++) {
                result += bot;
            }
        }

        return result;
    }

    struct Entry {
        size_t Cost;
        std::string CacheString;

        constexpr bool operator==(const Entry& e) const {
            return CacheString == e.CacheString;
        }
        constexpr bool operator<(const Entry& e) const {
            return e.Cost < Cost;
        }
    };

    constexpr size_t GetMoveCost(char type, const RowCol & from, const RowCol & to) {
        auto distance = MDistance(from, to);
        switch (type) {
        case 'A': return distance;
        case 'B': return distance * 10;
        case 'C': return distance * 100;
        case 'D': return distance * 1000;
        }
        throw "Bad type";
    }

    constexpr std::vector<Entry> GetNextMoves(const Hall & hall, const std::vector<Den>&dens, size_t costSoFar) {
        std::vector<Entry> result;

        for (const auto& bot : hall.GetOccupants()) {
            auto nextDens = dens;
            for (auto& den : nextDens) {
                if (den.Belongs(bot)) {
                    if (den.CanAdd() && hall.IsClear(bot.Pos.Col, den.Col)) {
                        auto cost = GetMoveCost(bot.Type, bot.Pos, den.NextPos());
                        cost += costSoFar;
                        auto nextHall = hall;
                        nextHall.Remove(bot);
                        den.Push(bot);

                        result.push_back({ cost, ToCacheString(nextHall, nextDens) });
                    }
                    break;
                }
            }
        }

        auto availableHallSlots = hall.GetAvailablePositions();
        for (size_t denIndex = 0; denIndex < dens.size(); denIndex++) {
            const auto& den = dens[denIndex];
            if (den.CanAdd()) continue; //if we can add bots to a den, don't remove any more
            const auto topBot = den.Top();

            for (const auto& dest : availableHallSlots) {
                if (hall.IsClear(dest.Col, den.Col)) {
                    auto nextHall = hall;
                    auto nextDens = dens;
                    auto nextBot = topBot;

                    auto cost = GetMoveCost(nextBot.Type, nextBot.Pos, dest);
                    cost += costSoFar;

                    nextBot.Pos = dest;
                    nextDens[denIndex].Pop();
                    nextHall.Occupy(nextBot);

                    result.push_back({ cost, ToCacheString(nextHall, nextDens) });
                }
            }
        }

        std::sort(result.begin(), result.end());
        return result;
    }

    constexpr auto Solve(std::vector<Den> dens) {
        Hall hall{};

        Constexpr::BigMap<std::string, size_t> seen;
        Constexpr::PriorityQueue<Entry> queue;
        auto initialState = ToCacheString(hall, dens);

        queue.push({ 0, initialState });
        seen[initialState] = 0;

        auto finishedString = GetFinishedString(dens);

        while (!queue.empty()) {
            auto next = queue.pop();
            if (next.CacheString == finishedString) return next.Cost;

            FromCacheString(next.CacheString, hall, dens);

            auto nextMoves = GetNextMoves(hall, dens, next.Cost);
            for (const auto& move : nextMoves) {
                if (seen.contains(move.CacheString)) {
                    if (move.Cost < seen.at(move.CacheString)) {
                        queue.push(move);
                    }
                }
                else {
                    seen[move.CacheString] = move.Cost;
                    queue.push(move);
                }
            }
        }

        return 0ull;
    }

    PART_ONE() {
        auto dens = GetInitialDens(CopyToVector(lines), false);
        return Constexpr::ToString(Solve(dens));
    }

    PART_TWO() {
        auto dens = GetInitialDens(CopyToVector(lines), true);
        return Constexpr::ToString(Solve(dens));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "#############",
            "#...........#",
            "###B#C#B#D###",
            "  #A#D#C#A#",
            "  #########"
        };

        auto d1 = GetInitialDens(lines, false);
        auto d2 = GetInitialDens(lines, true);
        if (Solve(d1) != 12521) return false;
        if (Solve(d2) != 44169) return false;

        return true;
    }
}