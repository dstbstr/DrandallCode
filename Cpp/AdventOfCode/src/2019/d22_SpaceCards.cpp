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

    void Shuffle(const std::vector<std::string>&lines, std::vector<u64>&deck) {
        for (const auto& line : lines) {
            if (StrUtil::StartsWith(line, "deal into")) {
                NewStack(deck);
            }
            else if (StrUtil::StartsWith(line, "cut")) {
                s32 cutSize;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[1], cutSize);
                Cut(deck, cutSize);
            }
            else {
                u32 increment;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[3], increment);
                DealWithIncrement(deck, increment);
            }
        }
    }
    auto Part1(const std::vector<std::string>&lines, size_t deckSize) {
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

        return result;
    }

    template<typename T>
    std::pair<T, T> GetMulAddToReverseShuffle(const std::vector<std::string>&lines, T deckSize) {
        auto mul = T(1);
        auto add = T(0);
        const auto one = T(1);
        for (auto it = lines.rbegin(); it != lines.rend(); it++) {
            auto line = *it;
            if (StrUtil::StartsWith(line, "deal into")) {
                add++;
                auto x = deckSize - one;
                if constexpr (std::is_unsigned_v<T>) {
                    mul = Constexpr::MulMod(mul, x, deckSize);
                    add = Constexpr::MulMod(add, x, deckSize);
                }
                else {
                    mul = (mul * x) % deckSize;
                    add = (add * x) % deckSize;
                }
            }
            else if (StrUtil::StartsWith(line, "cut")) {
                s32 cutSize;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[1], cutSize);
                if (cutSize < 0) {
                    add = deckSize + cutSize;
                }
                else {
                    add = cutSize;
                }
                add %= deckSize;
            }
            else {
                T increment;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[3], increment);
                auto x = Constexpr::MultiplicativeInverse(increment, deckSize);

                if constexpr (std::is_unsigned_v<T>) {
                    mul = Constexpr::MulMod(mul, x, deckSize);
                    add = Constexpr::MulMod(add, x, deckSize);
                }
                else {
                    mul = (mul * x) % deckSize;
                    add = (add * x) % deckSize;
                }
            }
        }

        return std::make_pair(mul, add);
    }

    template<typename T>
    auto FindPosValue(const std::vector<std::string>&lines, T pos, T deckSize, T repCount) {
        T one = 1;
        T zero = 0;

        auto [mul, add] = GetMulAddToReverseShuffle(lines, deckSize);
        auto mx = Constexpr::ModPow(mul, repCount, deckSize);
        T pmx;
        T amx;
        if constexpr (std::is_unsigned_v<T>) {
            pmx = Constexpr::MulMod(pos, mx, deckSize);
            amx = Constexpr::MulMod(add, mx, deckSize);
        }
        else {
            pmx = (pos * mx) % deckSize;
            amx = (add * mx) % deckSize;
        }
        auto inv = Constexpr::MultiplicativeInverse(mul - one, deckSize);
        T res;
        if constexpr (std::is_unsigned_v<T>) {
            res = Constexpr::MulMod(pmx + (amx - add), inv, deckSize);
        }
        else {
            res = (pmx + (amx - add) * inv) % deckSize;
        }

        if (res < zero) {
            res += deckSize;
        }

        return res;
    }

    template<typename T>
    auto Part2(const std::vector<std::string>&lines) {
        /*
        T pos = 2020;
        T deckSize = 119'315'717'514'047;
        T repCount = 101'741'582'076'661;
        */
        T pos = 2519;
        T deckSize = 10007;
        T repCount = 1;
        return FindPosValue(lines, pos, deckSize, repCount);
    }

    struct LinFunc {
        BigInt k, m;
        BigInt apply(BigInt x) { return x * k + m; }
    };

    LinFunc AggregateFunc(LinFunc f, LinFunc g) {
        return LinFunc(g.k * f.k, g.k * f.m + g.m);
    }

    LinFunc executeTimes(BigInt k, BigInt m, BigInt shuffles, BigInt deckSize) {
        if (shuffles == 0) return LinFunc(1, 0);
        else if (shuffles % 2 == 0) return executeTimes((k * k) % deckSize, ((k * m) + m) % deckSize, shuffles / 2, deckSize);
        else {
            LinFunc cd = executeTimes(k, m, shuffles - 1, deckSize);
            return LinFunc((k * cd.k) % deckSize, ((k * cd.m) + m) % deckSize);
        }
    }


    BigInt CardAt(BigInt in, const std::vector<std::string>&lines, const BigInt & deckSize, const BigInt & shuffleCount) {
        static const BigInt one = 1;
        static const BigInt zero = 0;
        static const LinFunc ID = { 1, 0 };
        std::vector<LinFunc> ops;
        for (auto it = lines.rbegin(); it != lines.rend(); it++) {
            auto line = *it;
            if (StrUtil::StartsWith(line, "deal into")) {
                ops.push_back(LinFunc(deckSize - 1, deckSize - 1));
            }
            else if (StrUtil::StartsWith(line, "cut")) {
                s32 cutSize;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[1], cutSize);

                BigInt add = cutSize < 0 ? deckSize + cutSize : cutSize;
                ops.push_back(LinFunc(one, add % deckSize));
            }
            else {
                BigInt increment;
                Constexpr::ParseNumber(Constexpr::Split(line, " ")[3], increment);
                auto x = Constexpr::MultiplicativeInverse(increment, deckSize);
                ops.push_back(LinFunc(x % deckSize, zero));
            }
        }

        auto shuffle = std::reduce(ops.begin(), ops.end(), ID, AggregateFunc);
        return executeTimes(shuffle.k, shuffle.m, shuffleCount, deckSize).apply(in) % deckSize;
    };


    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 10007));
        std::cout << CardAt(2020, lines, 119'315'717'514'047, 101'741'582'076'661) << "\n";
        return "0";

    }

    bool RunTests() {
        return BigIntTests::Run();
        //return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}