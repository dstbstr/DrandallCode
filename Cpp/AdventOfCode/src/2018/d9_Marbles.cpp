#include "2018/d9_Marbles.h"

SOLUTION(2018, 9) {

    template<size_t PlayerCount, size_t MaxValue>
    constexpr size_t Play() {
        auto* ringPtr = new Constexpr::Ring<size_t, MaxValue>();
        auto& ring = *ringPtr;

        std::array<size_t, PlayerCount> elves{};
        ring.push_front(0);

        for (size_t marble = 1; marble <= MaxValue; marble++) {
            if (marble % 23 == 0) {
                ring.rotate(-7);
                elves[marble % PlayerCount] += marble + ring.front();
                ring.pop_front();
            }
            else {
                ring.rotate(2);
                ring.push_front(marble);
            }
        }

        delete ringPtr;
        return Constexpr::FindMax(elves);
    }

    constexpr size_t Play2(size_t playerCount, size_t maxValue) {
        auto* ringPtr = new Constexpr::VecRing<size_t>{ maxValue };
        auto& ring = *ringPtr;

        std::vector<size_t> elves;
        elves.resize(playerCount);
        ring.push_front(0);

        for (size_t marble = 1; marble <= maxValue; marble++) {
            if (marble % 23 == 0) {
                ring.rotate(-7);
                elves[marble % playerCount] += marble + ring.front();
                ring.pop_front();
            }
            else {
                ring.rotate(2);
                ring.push_front(marble);
            }
        }

        delete ringPtr;
        return Constexpr::FindMax(elves);

    }
    //476 players; last marble is worth 71657 points
    constexpr void ParseInput(std::string_view line, size_t& players, size_t& maxValue) {
        auto s = Constexpr::Split(line, " ");
        Constexpr::ParseNumber(s[0], players);
        Constexpr::ParseNumber(s[6], maxValue);
    }

    PART_ONE() {
        //this version is slower, but doesn't hard code the input
        size_t players, maxValue;
        ParseInput(lines[0], players, maxValue);
        return Constexpr::ToString(Play2(players, maxValue));
        /*
        (void)lines;
        return Constexpr::ToString(Play<476, 71657>());
        */
    }

    PART_TWO() {
        size_t players, maxValue;
        ParseInput(lines[0], players, maxValue);
        return Constexpr::ToString(Play2(players, maxValue * 100));
        /*
        (void)lines;
        return Constexpr::ToString(Play<476, 7165700>());
        */
    }

    TESTS() {
        static_assert (Play<9, 25>() == 32);
        static_assert (Play<10, 1618>() == 8317);
        static_assert (Play<13, 7999>() == 146373);
        static_assert (Play<17, 1104>() == 2764);
        static_assert (Play<21, 6111>() == 54718);
        static_assert (Play<30, 5807>() == 37305);

        static_assert (Play2(9, 25) == 32);
        static_assert (Play2(10, 1618) == 8317);
        static_assert (Play2(13, 7999) == 146373);
        static_assert (Play2(17, 1104) == 2764);
        static_assert (Play2(21, 6111) == 54718);
        static_assert (Play2(30, 5807) == 37305);

        return true;
    }
}