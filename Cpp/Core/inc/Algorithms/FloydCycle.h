#pragma once

namespace FloydCycle {
    //Given a starting state, a NextFunc, finds the start of a cycle, and the cycle's length
    constexpr auto FindCycle(const auto& initial, auto NextFunc, u32& outCycleLength, u32& outCycleStart, auto... args) {
        auto [h0, h1] = std::pair(NextFunc(initial, args...), NextFunc(NextFunc(initial, args...), args...));
        while (h0 != h1) {
            h0 = NextFunc(h0, args...);
            h1 = NextFunc(NextFunc(h1, args...), args...);
        }
        outCycleStart = 0;
        for (h0 = initial; h0 != h1; h0 = NextFunc(h0, args...), h1 = NextFunc(h1, args...)) {
            ++outCycleStart;
        }
        outCycleLength = 1;
        for (h1 = NextFunc(h0, args...); h0 != h1; h1 = NextFunc(h1, args...)) {
            ++outCycleLength;
        }

        return h0;
    }
}