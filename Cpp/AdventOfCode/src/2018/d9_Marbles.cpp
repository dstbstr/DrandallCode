#include "2018/d9_Marbles.h"

SOLUTION(2018, 9) {
    constexpr void InsertMarble(std::vector<u32>&marbles, size_t & currentIndex, u32 value) {
        auto targetIndex = (currentIndex + 2) % marbles.size();
        marbles.insert(marbles.begin() + targetIndex, value);
        currentIndex = targetIndex;
    }

    constexpr u32 ScorePoints(std::vector<u32>&marbles, size_t & currentIndex) {
        auto targetIndex = (marbles.size() + currentIndex - 7) % marbles.size();
        auto value = marbles[targetIndex];
        marbles.erase(marbles.begin() + targetIndex);
        currentIndex = targetIndex;
        return value;
    }

    //Really slow for big numbers.  Maybe find all the 23s, and add in all the 'removed' marbles?
    constexpr u32 PlayRound(u32 players, u32 maxValue) {
        std::vector<u32> marbles;
        marbles.push_back(0);
        marbles.reserve(maxValue);

        std::vector<u32> playerScores;
        for (u32 i = 0; i < players; i++) {
            playerScores.push_back(0);
        }

        size_t currentIndex = 0;
        for (u32 i = 1; i <= maxValue; i++) {
            if (i % 23 == 0) {
                playerScores[i % players] += i;
                playerScores[i % players] += ScorePoints(marbles, currentIndex);
            }
            else {
                InsertMarble(marbles, currentIndex, i);
            }
        }

        return Constexpr::FindMax(playerScores);
    }

    static_assert(PlayRound(9, 25) == 32);

    std::string Run(const std::vector<std::string>&) {
        //return Constexpr::ToString(PlayRound(476, 71657));
        return Constexpr::ToString(PlayRound(476, 7165700));
    }

    bool RunTests() {
        if (PlayRound(10, 1618) != 8317) return false;
        if (PlayRound(13, 7999) != 146373) return false;
        if (PlayRound(17, 1104) != 2764) return false;
        if (PlayRound(21, 6111) != 54718) return false;
        if (PlayRound(30, 5807) != 37305) return false;

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