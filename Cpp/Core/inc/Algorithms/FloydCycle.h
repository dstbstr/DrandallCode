#pragma once

namespace FloydCycle {
    //Given a starting state, a NextFunc, finds the start of a cycle, and the cycle's length
    constexpr void FindCycle(const auto& initial, auto NextFunc, u32& outCycleLength, u32& outCycleStart) {
        auto [h0, h1] = std::pair(NextFunc(initial), NextFunc(NextFunc(initial)));
        while (h0 != h1) {
            h0 = NextFunc(h0);
            h1 = NextFunc(NextFunc(h1));
        }
        outCycleStart = 0;
        for (h0 = initial; h0 != h1; h0 = NextFunc(h0), h1 = NextFunc(h1)) {
            ++outCycleStart;
        }
        outCycleLength = 1;
        for (h1 = NextFunc(h0); h0 != h1; h1 = NextFunc(h1)) {
            ++outCycleLength;
        }
    }
}