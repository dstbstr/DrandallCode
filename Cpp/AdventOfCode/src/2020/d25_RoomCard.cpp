#include "2020/d25_RoomCard.h"

SOLUTION(2020, 25) {
    constexpr size_t RunLoop(size_t value, size_t subject) {
        value *= subject;
        return value % 20201227;
    }

    constexpr bool RunTest() {
        size_t value = 1;
        size_t subject = 7;
        for (auto i = 0; i < 8; i++) {
            value = RunLoop(value, subject);
        }
        if (value != 5764801) return false;

        value = 1;
        for (auto i = 0; i < 11; i++) {
            value = RunLoop(value, subject);
        }
        if (value != 17807724) return false;
        return true;
    }

    static_assert(RunTest());

    constexpr size_t FindLoopSize(size_t publicKey) {
        size_t value = 1;
        size_t subject = 7;
        size_t loopCount = 0;
        while (value != publicKey) {
            value = RunLoop(value, subject);
            loopCount++;
        }

        return loopCount;
    }

    static_assert(FindLoopSize(5764801) == 8);
    static_assert(FindLoopSize(17807724) == 11);

    constexpr size_t Decrypt(size_t pk1, size_t pk2) {
        auto loopSize = FindLoopSize(pk1);
        size_t value = 1;
        for (auto i = 0; i < loopSize; i++) {
            value = RunLoop(value, pk2);
        }

        return value;
    }

    static_assert(Decrypt(5764801, 17807724) == 14897079);

    std::string Run(const std::vector<std::string>&) {
        return Constexpr::ToString(Decrypt(14788856, 19316454));
    }

    bool RunTests() {
        return true;
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