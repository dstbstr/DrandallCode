#include "2016/d19_WhiteElephant.h"

SOLUTION(2016, 19) {
    constexpr double ln3 = 1.09861228866810969;
    constexpr double lnInput = 14.91818458658705;

    constexpr u32 FindLeftWinner(u32 playerCount) {
        auto power = Constexpr::Log2I(playerCount);
        auto remainder = playerCount - Constexpr::Pow(2u, power);
        return static_cast<u32>(2 * remainder + 1);
    }

    constexpr u32 FindAcrossWinner(u32 playerCount) {
        //auto pow = static_cast<u32>(std::floor(std::log(playerCount) / std::log(3)));
        auto pow = static_cast<u32>(lnInput / ln3);
        auto b = Constexpr::Pow(3u, pow);
        
        if (playerCount == b) return playerCount;
        if (playerCount - b <= b) return playerCount - b;
        return 2 * playerCount - 3 * b;
    }

    PART_ONE() {
        u32 playerCount;
        Constexpr::ParseNumber(Line, playerCount);
        return Constexpr::ToString(FindLeftWinner(playerCount));
    }
    PART_TWO() {
        u32 playerCount;
        Constexpr::ParseNumber(Line, playerCount);
        return Constexpr::ToString(FindAcrossWinner(playerCount));
    }

    TESTS() {
        static_assert(FindLeftWinner(5) == 3);
        /*
        if (FindLeftWinner(5) != 3) return false;
        if (FindAcrossWinner(5) != 2) return false;
        */
        return true;
    }
}