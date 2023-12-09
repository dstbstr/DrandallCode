#include "2016/d11_Generators.h"

SOLUTION(2016, 11) {
//4 floors
//RTG & Chips
//Chip + RTG is safe
//Chip without RTG can be fried by any other RTG
//Elevator stops on each floor
//Elevator holds at least 1, up to 2 objects


    struct Floor {
        std::string Generators;
        std::string Chips;

        constexpr bool operator==(const Floor& other) const {
            return Generators == other.Generators && Chips == other.Chips;
        }

        constexpr bool IsEmpty() const {
            return Generators.empty() && Chips.empty();
        }

        constexpr bool IsStable() const {
            for (auto c : Chips) {
                if (Generators.find(c) == Generators.npos && !Generators.empty()) return false;
            }
            return true;
        }

        constexpr void Sort() {
            std::sort(Generators.begin(), Generators.end());
            std::sort(Chips.begin(), Chips.end());
        }
    };
    struct State {
        std::vector<Floor> Floors;
        size_t Elevator = 0;

        constexpr bool operator==(const State& other) const {
            return Elevator == other.Elevator && Floors == other.Floors;
        }

        constexpr bool IsStable() const {
            for (const auto& floor : Floors) {
                if (!floor.IsStable()) return false;
            }
            return true;
        }

        constexpr void Sort() {
            for (auto& floor : Floors) {
                floor.Sort();
            }
        }
    };

    constexpr State ParseInput(const auto& lines) {
        State result;
        for (const auto& line : lines) {
            Floor floor;
            auto s = Constexpr::Split(line, " ");
            for (auto i = 0; i < s.size(); i++) {
                if (s[i].substr(0, 3) == "gen") {
                    floor.Generators.push_back(s[i - 1][0]);
                }
                else if (s[i].substr(0, 3) == "mic") {
                    floor.Chips.push_back(s[i - 1][0]);
                }
            }
            result.Floors.push_back(floor);
        }
        return result;
    }

    constexpr State CreateTarget(const State& initial) {
        State result;
        size_t floorCount = initial.Floors.size();
        result.Elevator = floorCount - 1;
        result.Floors.resize(floorCount);
        auto& topFloor = result.Floors.back();

        for (const auto& floor : initial.Floors) {
            std::copy(floor.Chips.begin(), floor.Chips.end(), std::back_inserter(topFloor.Chips));
            std::copy(floor.Generators.begin(), floor.Generators.end(), std::back_inserter(topFloor.Generators));
        }

        return result;
    }

    constexpr void MoveChip(size_t from, size_t to, char chip, State& outState) {
        outState.Floors[to].Chips.push_back(chip);
        Constexpr::RemoveAllOf(outState.Floors[from].Chips, chip);
    }
    constexpr void MoveGen(size_t from, size_t to, char gen, State& outState) {
        outState.Floors[to].Generators.push_back(gen);
        Constexpr::RemoveAllOf(outState.Floors[from].Generators, gen);
    }

    constexpr State MakeNext(const State& current, size_t from, size_t to, const std::vector<char>& chips, const std::vector<char>& gens) {
        State next = current;
        next.Elevator = to;
        for (auto c : chips) {
            MoveChip(from, to, c, next);
        }
        for (auto g : gens) {
            MoveGen(from, to, g, next);
        }
        return next;
    }

    constexpr std::vector<State> GetSingles(const State& current, size_t from, size_t to) {
        const auto& here = current.Floors[from];
        std::vector<State> result;

        for (auto c : here.Chips) {
            auto next = MakeNext(current, from, to, { c }, {});
            if (next.IsStable()) {
                result.push_back(next);
            }
        }
        for (auto g : here.Generators) {
            auto next = MakeNext(current, from, to, {}, { g });
            if (next.IsStable()) {
                result.push_back(next);
            }
        }

        return result;
    }

    constexpr std::vector<State> GetPairs(const State& current, size_t from, size_t to) {
        const auto& here = current.Floors[from];
        std::vector<State> result;

        for (size_t c1 = 0; c1 < here.Chips.size(); c1++) {
            for (size_t c2 = c1 + 1; c2 < here.Chips.size(); c2++) {
                auto next = MakeNext(current, from, to, { here.Chips[c1], here.Chips[c2] }, {});
                if (next.IsStable()) {
                    result.push_back(next);
                }
            }
            for (size_t g1 = 0; g1 < here.Generators.size(); g1++) {
                auto next = MakeNext(current, from, to, { here.Chips[c1] }, { here.Generators[g1] });
                if (next.IsStable()) {
                    result.push_back(next);
                }
            }
        }
        for (size_t g1 = 0; g1 < here.Generators.size(); g1++) {
            for (size_t g2 = g1 + 1; g2 < here.Generators.size(); g2++) {
                auto next = MakeNext(current, from, to, {}, { here.Generators[g1], here.Generators[g2] });
                if (next.IsStable()) {
                    result.push_back(next);
                }
            }
        }

        return result;
    }
    constexpr std::vector<State> GetUpMoves(const State& current) {
        size_t from = current.Elevator;
        size_t to = from + 1;
        if (from >= current.Floors.size() - 1) return {};
        auto result = GetPairs(current, from, to);
        if (result.empty()) {
            result = GetSingles(current, from, to);
        }
        return result;
    }

    constexpr std::vector<State> GetDownMoves(const State& current) {
        size_t from = current.Elevator;
        if (from == 0) return {};
        size_t to = from - 1;
        auto result = GetSingles(current, from, to);
        if (result.empty()) {
            result = GetPairs(current, from, to);
        }
        return result;
    }
    constexpr std::vector<State> GetNextMoves(const State& current) {
        std::vector<State> result = GetUpMoves(current);

        if (current.Floors[0].IsEmpty()) {
            if (current.Elevator == 1) return result;
            if (current.Elevator == 2 && current.Floors[1].IsEmpty()) return result;
        }

        auto downMoves = GetDownMoves(current);
        std::copy(downMoves.begin(), downMoves.end(), std::back_inserter(result));
        
        return result;
    }

    constexpr size_t Hash(const State& state) {
        size_t hash = state.Elevator;
        size_t factor = 10;

        for (size_t f1 = 0; f1 < state.Floors.size(); f1++) {
            for (auto c : state.Floors[f1].Chips) {
                for (size_t f2 = 0; f2 < state.Floors.size(); f2++) {
                    const auto& gens = state.Floors[f2].Generators;
                    if (std::find(gens.begin(), gens.end(), c) != gens.end()) {
                        hash += factor * ((9 - f1) - f2); //9 is a constant to make negative numbers positive
                        break;
                    }
                }
            }

            factor *= 100;
        }
        return hash;
    }

    constexpr u32 Solve(const auto& lines) {
        auto initial = ParseInput(lines);
        auto target = CreateTarget(initial);
        
        target.Sort();
        initial.Sort();

        std::vector<State> current = { initial };
        std::vector<State> next;
        Constexpr::SmallSet<size_t> seen;
        seen.insert(Hash(initial));

        u32 moveIndex = 0;

        while (!current.empty()) {
            moveIndex++;
            for (const auto& state : current) {
                if (state == target) {
                    return moveIndex - 1;
                }

                auto nextMoves = GetNextMoves(state);
                for (auto& move : nextMoves) {
                    move.Sort();
                }

                for (const auto& move : nextMoves) {
                    auto hash = Hash(move);
                    if (!seen.contains(hash)) {
                        seen.insert(hash);
                        next.push_back(move);
                    }
                }
            }
            current = next;
            next.clear();
        }

        return 0; //fail case
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines));
    }
    PART_TWO() {
        auto copy = lines;
        copy[0] += " elerium generator, elerium microchip, dilithium generator, dilithium microchip";
        return Constexpr::ToString(Solve(copy));
    }

    constexpr bool TestHash() {
        State lhs;
        State rhs;
        lhs.Elevator = 0;
        rhs.Elevator = 0;

        lhs.Floors.push_back({ { "ab" }, {} });
        lhs.Floors.push_back({ { "c"}, {"a"} });
        lhs.Floors.push_back({ {}, {"bc"} });

        rhs.Floors.push_back({ {"bc"}, {} });
        rhs.Floors.push_back({ {"a"},{"b"} });
        rhs.Floors.push_back({ {}, {"ca"} });

        if (Hash(lhs) != Hash(rhs)) return false;
        if (Hash(lhs) != Hash(lhs)) return false;

        lhs.Elevator = 1;
        if (Hash(lhs) == Hash(rhs)) return false;

        lhs.Elevator = 0;
        lhs.Floors.clear();
        rhs.Floors.clear();

        lhs.Floors.push_back({ {}, {} });
        lhs.Floors.push_back({ {"ab"},{} });
        lhs.Floors.push_back({ {},{"ab"} });

        rhs.Floors.push_back({ {"ab"},{} });
        rhs.Floors.push_back({ {},{"ab"} });
        rhs.Floors.push_back({ {},{} });

        if (Hash(lhs) == Hash(rhs)) return false;

        return true;
    }

    TESTS() {
        static_assert(TestHash());

        std::vector<std::string> lines = {
            "The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.",
            "The second floor contains a hydrogen generator.",
            "The third floor contains a lithium generator.",
            "The fourth floor contains nothing relevant."
        };

        if (Solve(lines) != 11) return false;

        return true;
    }
}