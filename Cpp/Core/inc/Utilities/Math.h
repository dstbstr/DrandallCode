#pragma once

#include <vector>
#include <array>
#include <cmath>

namespace Math {
    template<typename T>
    std::vector<T> GetPrimes(T max) {
        static_assert(!std::is_floating_point_v<T>);
        static std::vector<T> knownPrimes;
        if (knownPrimes.back() >= max) {
            std::vector<T> result;
            for (auto prime : knownPrimes) {
                if (prime <= max) {
                    result.push_back(prime);
                }
                else {
                    return result;
                }
            }
        }

        char prime[max + 1];
        std::memset(prime, 1, sizeof(prime));

        std::vector<T> result;
        auto maxFactor = static_cast<T>(std::sqrt(static_cast<double>(max + 1)));
        for (T p = 2; p <= maxFactor; p++) {
            if (prime[p] == 0) continue;
            result.push_back(p);
            for (size_t factor = static_cast<size_t>(p * p); factor < max + 1; factor += p) {
                prime[factor] = 0;
            }
        }

        for (T p = maxFactor + 1; p < max + 1; p++) {
            if (prime[p] == 1) result.push_back(p);
        }

        knownPrimes = result;
        return result;
    }
}