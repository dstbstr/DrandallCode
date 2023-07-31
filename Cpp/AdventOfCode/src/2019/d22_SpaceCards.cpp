#include "2019/d22_SpaceCards.h"
#include "BigInt.h"

SOLUTION(2019, 22) {
    constexpr void NewStack(std::vector<u64>&deck) {
        std::reverse(deck.begin(), deck.end());
    }

    constexpr void Cut(std::vector<u64>&deck, s32 amount) {
        if (amount < 0) {
            std::rotate(deck.rbegin(), deck.rbegin() - amount, deck.rend());
        }
        else {
            std::rotate(deck.begin(), deck.begin() + amount, deck.end());
        }
    }

    constexpr void DealWithIncrement(std::vector<u64>&deck, u32 increment) {
        std::vector<u64> result = deck;
        size_t cardIndex = 0;
        for (auto i = 0; i < deck.size(); i++) {
            result[cardIndex] = deck[i];
            cardIndex = (cardIndex + increment) % deck.size();
        }

        deck = result;
    }

    template<typename Func, typename... Args>
    constexpr bool RunTest(const std::vector<u64>&initial, const std::vector<u64>&expected, Func func, Args&&... args) {
        std::vector<u64> running = initial;
        func(running, std::forward<Args&&>(args)...);
        return running == expected;
    }

    static_assert(RunTest({ 1, 2, 3 }, { 3, 2, 1 }, NewStack));
    static_assert(RunTest({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 }, Cut, 3));
    static_assert(RunTest({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 }, Cut, -4));
    static_assert(RunTest({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 0, 7, 4, 1, 8, 5, 2, 9, 6, 3 }, DealWithIncrement, 3));

    constexpr void Shuffle(const std::vector<std::string>&lines, std::vector<u64>&deck) {
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " ");
            if (s[1] == "into") {
                NewStack(deck);
            }
            else if (s[0] == "cut") {
                s32 cutSize;
                Constexpr::ParseNumber(s[1], cutSize);
                Cut(deck, cutSize);
            }
            else {
                u32 increment;
                Constexpr::ParseNumber(s[3], increment);
                DealWithIncrement(deck, increment);
            }
        }
    }

    struct LinFunc {
        BigInt k, m;
        constexpr BigInt apply(BigInt x) { return x * k + m; }
    };

    constexpr LinFunc AggregateFunc(LinFunc f, LinFunc g) {
        return LinFunc(g.k * f.k, g.k * f.m + g.m);
    }

    constexpr LinFunc executeTimes(BigInt k, BigInt m, BigInt shuffles, BigInt deckSize) {
        if (shuffles == 0) return LinFunc(1, 0);
        else if (shuffles % 2 == 0) return executeTimes((k * k) % deckSize, ((k * m) + m) % deckSize, shuffles / 2, deckSize);
        else {
            LinFunc cd = executeTimes(k, m, shuffles - 1, deckSize);
            return LinFunc((k * cd.k) % deckSize, ((k * cd.m) + m) % deckSize);
        }
    }

    constexpr BigInt CardAt(BigInt in, const std::vector<std::string>&lines, const BigInt & deckSize, const BigInt & shuffleCount) {
        const BigInt one = 1;
        const BigInt zero = 0;
        const LinFunc ID = { 1, 0 };
        std::vector<LinFunc> ops;
        for (auto it = lines.rbegin(); it != lines.rend(); it++) {
            auto line = *it;
            auto s = Constexpr::Split(line, " ");
            if (s[1] == "into") {
                ops.push_back(LinFunc(deckSize - 1, deckSize - 1));
            }
            else if (s[0] == "cut") {
                s32 cutSize;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[1], cutSize);

                BigInt add = cutSize < 0 ? deckSize + cutSize : cutSize;
                ops.push_back(LinFunc(one, add % deckSize));
            }
            else {
                BigInt increment;
                Constexpr::ParseNumber(s[3], increment);
                auto x = Constexpr::MultiplicativeInverse(increment, deckSize);
                ops.push_back(LinFunc(x % deckSize, zero));
            }
        }

        auto shuffle = std::reduce(ops.begin(), ops.end(), ID, AggregateFunc);
        return executeTimes(shuffle.k, shuffle.m, shuffleCount, deckSize).apply(in) % deckSize;
    };

    PART_ONE() {
        size_t deckSize = 10'007;
        std::vector<u64> deck(deckSize);
        for (auto i = 0; i < deckSize; i++) {
            deck[i] = i;
        }
        Shuffle(lines, deck);

        size_t result = 0;
        for (const auto& card : deck) {
            if (card == 2019) {
                break;
            }
            result++;
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        BigInt pos = 2020;
        BigInt deckSize = 119'315'717'514'047;
        BigInt repCount = 101'741'582'076'661;

        return CardAt(pos, lines, deckSize, repCount).ToString();
    }

    TESTS() {
        return true;
    }
}