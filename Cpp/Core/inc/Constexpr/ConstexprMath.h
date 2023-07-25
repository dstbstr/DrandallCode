#pragma once

#include <vector>
#include <array>
#include <string_view>
#include <algorithm> //std::copy_if
#include <iterator> //std::back_inserter

namespace Constexpr {
    template<typename T>
    constexpr T Constant_E = 2.718281828459045235360287471352;

    template<typename T>
    constexpr T Abs(T t) {
        if constexpr (std::is_signed_v<T>) {
            return t < 0 ? -t : t;
        }
        else {
            return t;
        }
    }

    template<typename T>
    constexpr T AbsDistance(T lhs, T rhs) {
        return lhs < rhs ? rhs - lhs : lhs - rhs;
    }

    template<typename T>
    constexpr T Floor(T val) {
        static_assert(std::is_floating_point_v<T>, "Floor must take a floating point value");
        return static_cast<T>(static_cast<int64_t>(val));
    }

    namespace Detail {
        constexpr double SqrtImpl(double x, double curr, double prev) {
            return curr == prev ? curr : SqrtImpl(x, 0.5 * (curr + x / curr), curr);
        }
    }
    constexpr double Sqrt(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity()
            ? Detail::SqrtImpl(x, x, 0)
            : std::numeric_limits<double>::quiet_NaN();
    }

    template<typename T, typename PowType>
    constexpr T Pow(T val, PowType pow) {
        static_assert(!std::is_floating_point_v<PowType>, "This power only works with integers");
        if (pow == 0) return 1;

        T result = val;
        for (PowType i = 1; i < pow; i++) {
            result *= val;
        }
        return result;
    }

    template<typename T>
    constexpr T Factorial(T val) {
        static_assert(!std::is_floating_point_v<T>, "Factorial works with integral types");
        T result = 1;
        while (val > 1) {
            result *= val--;
        }
        return result;
    }
    namespace Detail {
        static constexpr std::array<uint8_t, 32> DeBruijnBitPosition = { 0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31 };
    }
    //Lovingly stolen from here: https://stackoverflow.com/questions/15967240/fastest-implementation-of-log2int-and-log2float
    template<typename T>
    constexpr T Log2I(T v) {
        static_assert(!std::is_floating_point_v<T>, "Log2I must be given an integral type");
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        return static_cast<T>(Detail::DeBruijnBitPosition[static_cast<size_t>(v * 0x07C4ACDDU) >> 27]);
    }

    template<typename T>
    constexpr T MulMod(T lhs, T rhs, T mod) {
        static_assert(std::is_unsigned_v<T>, "MulMod depends on unsigned types");

        T result = 0;
        T temp;
        T two = 2;
        T one = 1;
        T zero = 0;
        if (rhs >= mod) {
            if (mod > std::numeric_limits<T>::max() / two) rhs -= mod;
            else rhs %= mod;
        }
        while (lhs != zero) {
            if (lhs % two == one) {
                if (rhs >= mod - result) result -= mod;
                result += rhs;
            }
            lhs >>= 1;

            temp = rhs;
            if (rhs >= mod - rhs) temp -= mod;
            rhs += temp;
        }

        return result;
    }

    template<typename T>
    constexpr T ModPow(T base, T exp, T mod) {
        static_assert(!std::is_floating_point_v<T>, "Type must not be floating point");
        const T zero = 0;
        const T one = 1;
        const T two = 2;
        if (mod <= zero) throw("Divide by zero");
        if (mod == one) return zero;

        T result = one;
        base %= mod;
        while (exp > zero) {
            if (exp % two == one) {
                if constexpr (std::is_unsigned_v<T>) {
                    result = MulMod(result, base, mod);
                }
                else {
                    result = (result * base) % mod;
                }
            }
            exp >>= one;
            if constexpr (std::is_unsigned_v<T>) {
                base = MulMod(base, base, mod);
            }
            else {
                base = (base * base) % mod;
            }
        }

        return result;
    }

    template<typename T>
    constexpr T EuclideanModulo(T value, T modulus) {
        if (modulus == 0) return 0;
        T remainder = value % modulus;
        return remainder >= 0 ? remainder : remainder + Abs(modulus);
    }

    constexpr size_t FromBase26(std::string_view str) {
        auto pow = 1;
        size_t result = 0;
        auto ToLower = [](char c) { return c <= 'Z' ? c + 32 : c; };
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            result += (ToLower(*it) - 'a' + 1) * pow;
            pow *= 26;
        }

        return result - 1;
    }

    template<typename T>
    constexpr auto KnownPrimes = std::array<T, 46>{ 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199 };

    template<typename T>
    constexpr std::vector<T> GetPrimes(T max) {
        static_assert(!std::is_floating_point_v<T>);
        if (max <= KnownPrimes<T>.back()) {
            std::vector<T> result;
            std::copy_if(KnownPrimes<T>.begin(), KnownPrimes<T>.end(), std::back_inserter(result), [max](auto p) { return p <= max; });
            return result;
        }
        std::vector<bool> candidates{};
        candidates.reserve(max + 1);
        for (T i = 0; i < max + 1; i++) {
            candidates.push_back(true);
        }

        std::vector<T> result{};
        auto maxFactor = static_cast<T>(Sqrt(static_cast<double>(max) + 1));
        for (T prime = 2; prime <= maxFactor; prime++) {
            if (!candidates[prime]) continue;
            result.push_back(prime);
            for (T factor = prime * prime; factor < max + 1; factor += prime) {
                candidates[factor] = false;
            }
        }

        for (T prime = maxFactor + 1; prime < max + 1; prime++) {
            if (candidates[prime]) {
                result.push_back(prime);
            }
        }

        return result;
    }

    template<size_t Max, typename T>
    constexpr std::vector<T> GetPrimes() {
        static_assert(!std::is_floating_point_v<T>);
        if constexpr (Max <= KnownPrimes<T>.back()) {
            std::vector<T> result;
            std::copy_if(KnownPrimes<T>.begin(), KnownPrimes<T>.end(), std::back_inserter(result), [](auto p) { return p <= Max; });
            return result;
        }
        else {
            std::array<bool, Max + 1> candidates{};
            std::fill(candidates.begin(), candidates.end(), true);

            std::vector<T> result{};
            auto maxFactor = static_cast<T>(Sqrt(Max + 1));
            for (T prime = 2; prime <= maxFactor; prime++) {
                if (!candidates[prime]) continue;
                result.push_back(prime);
                for (size_t factor = static_cast<size_t>(prime * prime); factor < Max + 1; factor += prime) {
                    candidates[factor] = false;
                }
            }

            for (T prime = maxFactor + 1; prime < Max + 1; prime++) {
                if (candidates[prime]) {
                    result.push_back(prime);
                }
            }

            return result;
        }
    }

    template<typename T>
    constexpr std::vector<T> GetUniquePrimeFactors(T value) {
        auto primes = GetPrimes<T>(value);

        std::vector<T> result{};

        for (auto factor : primes) {
            if (value % factor == 0) {
                result.push_back(factor);
            }
        }

        return result;
    }

    template<size_t Value, typename T>
    constexpr std::vector<T> GetUniquePrimeFactors() {
        static_assert(Value > 1);
        auto primes = GetPrimes<Value, T>();

        std::vector<T> result{};

        for (auto factor : primes) {
            if (Value % factor == 0) {
                result.push_back(factor);
            }
        }

        return result;
    }

    template<typename T>
    constexpr std::vector<T> GetAllPrimeFactors(T value) {
        auto primes = GetPrimes<T>(value);

        std::vector<T> result{};

        for (auto factor : primes) {
            auto running = value;
            while (running % factor == 0) {
                result.push_back(factor);
                running /= factor;
            }
        }

        return result;
    }

    template<size_t Value, typename T>
    constexpr std::vector<T> GetAllPrimeFactors() {
        static_assert(Value > 1);
        auto primes = GetPrimes<Value, T>();

        std::vector<T> result{};

        for (auto factor : primes) {
            auto running = Value;
            while (running % factor == 0) {
                result.push_back(factor);
                running /= factor;
            }
        }

        return result;
    }

    template<typename T>
    constexpr std::vector<T> GetDivisors(T input) {
        static_assert(std::is_integral_v<T>, "GetDivisors only works with integral types");

        auto last = static_cast<T>(Sqrt(input));
        auto result = std::vector<T>{};

        for (T i = 1; i < last; i++) {
            if (input % i == 0) {
                result.push_back(i);
                result.push_back(input / i);
            }
        }

        if (last * last == input) {
            result.push_back(last);
        }
        else if (input % last == 0) {
            result.push_back(last);
            result.push_back(input / last);
        }

        return result;
    }

    namespace detail {
        template<typename T>
        constexpr T FindLcm(const std::vector<std::vector<T>>& factors) {
            if (factors.size() == 2) {
                size_t lhsIndex = 0, rhsIndex = 0;
                T result = 1;
                while (lhsIndex < factors[0].size() && rhsIndex < factors[1].size()) {
                    auto lhs = factors[0][lhsIndex];
                    auto rhs = factors[1][rhsIndex];
                    if (lhs == rhs) {
                        result *= lhs;
                        lhsIndex++;
                        rhsIndex++;
                    }
                    else if (lhs < rhs) {
                        result *= lhs;
                        lhsIndex++;
                    }
                    else {
                        result *= rhs;
                        rhsIndex++;
                    }
                }
                if (lhsIndex < factors[0].size()) {
                    for (; lhsIndex < factors[0].size(); lhsIndex++) {
                        result *= factors[0][lhsIndex];
                    }
                }
                else if (rhsIndex < factors[1].size()) {
                    for (; rhsIndex < factors[1].size(); rhsIndex++) {
                        result *= factors[1][rhsIndex];
                    }
                }

                return result;
            }
            T result = 1;
            std::vector<size_t> indexes;
            for (auto i = 0; i < factors.size(); i++) {
                indexes.push_back(0);
            }

            auto oneListDone = [&]() {
                for (auto i = 0; i < indexes.size(); i++) {
                    if (indexes[i] == factors[i].size()) {
                        return true;
                    }
                }
                return false;
            };

            std::vector<T> values;
            while (!oneListDone()) {
                values.clear();
                for (auto i = 0; i < indexes.size(); i++) {
                    values.push_back(factors[i][indexes[i]]);
                }

                auto min = *std::min_element(values.begin(), values.end());

                result *= min;
                for (auto i = 0; i < indexes.size(); i++) {
                    if (values[i] == min) {
                        indexes[i]++;
                    }
                }
            }

            for (auto i = 0; i < indexes.size(); i++) {
                for (auto index = indexes[i]; index < factors[i].size(); index++) {
                    result *= factors[i][indexes[i]];
                }
            }

            return result;
        }

        template<typename T, typename... Args>
        constexpr T FindLcm(Args... args) {
            return detail::FindLcm<T>(std::vector<std::vector<T>>{std::forward<Args>(args)...});
        }
    }

    template<typename T, typename... Args>
    constexpr T FindLcm(Args&&... args) {
        return detail::FindLcm<T>(GetAllPrimeFactors<T>(std::forward<Args>(args))...);
    }

    template<size_t Lhs, size_t Rhs, typename T>
    constexpr T FindLcm() {
        return detail::FindLcm<T>(GetAllPrimeFactors<Lhs, T>(), GetAllPrimeFactors<Rhs, T>());
    }

    
    constexpr auto FindGcd(auto... args) {
        auto product = (args * ... * 1);
        auto lcm = FindLcm<decltype(product)>(args...);
        return product == 0 ? lcm : product / lcm;
    }

    template<typename T>
    constexpr bool Eval(T lhs, T rhs, std::string_view op) {
        if (op == "<") {
            return lhs < rhs;
        }
        else if (op == ">") {
            return lhs > rhs;
        }
        else if (op == "<=") {
            return lhs <= rhs;
        }
        else if (op == ">=") {
            return lhs >= rhs;
        }
        else if (op == "==") {
            return lhs == rhs;
        }
        else if (op == "!=") {
            return lhs != rhs;
        }

        return false;
    }

    template<typename T>
    constexpr T MultiplicativeInverse(T a, T n) {
        static_assert(T(-1) < T(0), "MultiplicativeInverse requires signed numbers");
        T t = 0;
        T newT = 1;
        T r = n;
        T newR = a;

        const T zero = 0;
        const T one = 1;

        while (newR != zero) {
            auto q = r / newR;
            t -= q * newT;
            r -= q * newR;
            std::swap(t, newT);
            std::swap(r, newR);
        }

        if (r != one) {
            return 0; //inverse does not exist
        }

        if (t < zero) {
            t += n;
        }

        return t;
    }

    namespace ConstexprMathTests {
        bool RunTests();
    }
}