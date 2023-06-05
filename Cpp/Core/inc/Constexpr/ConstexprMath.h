#pragma once

#include <vector>
#include <array>

namespace Constexpr {
    template<typename T>
    constexpr T Abs(T t) {
        if constexpr (std::is_signed_v<T>) {
            return t < 0 ? -t : t;
        }
        else {
            return t;
        }
    }

    static_assert(Abs(3) == 3);
    static_assert(Abs(-3) == 3);

    template<typename T>
    constexpr T AbsDistance(T lhs, T rhs) {
        return lhs < rhs ? rhs - lhs : lhs - rhs;
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

    static_assert(Sqrt(9) == 3.0);
    static_assert(Sqrt(1) == 1.0);
    static_assert(Sqrt(-1) != Sqrt(-1)); // no way to test for NaN, but NaN != NaN

    template<typename T>
    constexpr T Pow(T val, T pow) {
        if (pow == 0) return 1;

        T result = val;
        for (auto i = 1; i < pow; i++) {
            result *= val;
        }
        return result;
    }

    static_assert(Pow(2, 0) == 1);
    static_assert(Pow(2, 4) == 16);

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
    static_assert(MulMod(3u, 5u, 20u) == 15u);
    static_assert(MulMod(3u, 5u, 7u) == 1u);

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
    static_assert(ModPow(2, 3, 5) == 3); //2^3 == 8 % 5 == 3


    template<typename T>
    constexpr T EuclideanModulo(T value, T modulus) {
        if (modulus == 0) return 0;
        T remainder = value % modulus;
        return remainder >= 0 ? remainder : remainder + Abs(modulus);
    }

    static_assert(EuclideanModulo(-5, 3) == 1);

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

    static_assert(FromBase26(std::string_view("a")) == 0);
    static_assert(FromBase26(std::string_view("z")) == 25);
    static_assert(FromBase26(std::string_view("aa")) == 26);

    template<typename T>
    constexpr std::vector<T> GetPrimes(T max) {
        static_assert(!std::is_floating_point_v<T>);
        std::vector<bool> candidates{};
        candidates.reserve(max + 1);
        for (size_t i = 0; i < max + 1; i++) {
            candidates.push_back(true);
        }

        std::vector<T> result{};
        auto maxFactor = static_cast<T>(Sqrt(static_cast<double>(max) + 1));
        for (T prime = 2; prime <= maxFactor; prime++) {
            if (!candidates[prime]) continue;
            result.push_back(prime);
            for (size_t factor = static_cast<size_t>(prime * prime); factor < max + 1; factor += prime) {
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

    static_assert(GetPrimes<size_t>(10) == std::vector<size_t>{2, 3, 5, 7});
    static_assert(GetPrimes<size_t>(7) == std::vector<size_t>{2, 3, 5, 7});

    template<size_t Max, typename T>
    constexpr std::vector<T> GetPrimes() {
        static_assert(!std::is_floating_point_v<T>);

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

    static_assert(GetPrimes<10, size_t>() == std::vector<size_t>{2, 3, 5, 7});
    static_assert(GetPrimes<7, size_t>() == std::vector<size_t>{2, 3, 5, 7});

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

    static_assert(GetUniquePrimeFactors<size_t>(10) == std::vector<size_t>{2, 5});
    static_assert(GetUniquePrimeFactors<size_t>(7) == std::vector<size_t>{7});

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

    static_assert(GetUniquePrimeFactors<10, size_t>() == std::vector<size_t>{2, 5});
    static_assert(GetUniquePrimeFactors<7, size_t>() == std::vector<size_t>{7});

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

    static_assert(GetAllPrimeFactors<size_t>(16) == std::vector<size_t>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<size_t>(7) == std::vector<size_t>{7});

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

    static_assert(GetAllPrimeFactors<16, size_t>() == std::vector<size_t>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<7, size_t>() == std::vector<size_t>{7});

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

    static_assert(GetDivisors(24) == std::vector<int>{1, 24, 2, 12, 3, 8, 4, 6});
    static_assert(GetDivisors(9) == std::vector<int>{1, 9, 3});
    static_assert(GetDivisors(1) == std::vector<int>{1});

    namespace detail {
        template<typename T>
        constexpr T FindLcm(const std::vector<std::vector<T>>& factors) {
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

            auto findMin = [](const std::vector<T>& vec) {
                T min = std::numeric_limits<T>::max();
                for (const auto val : vec) {
                    min = std::min(min, val);
                }

                return min;
            };

            std::vector<T> values;
            while (!oneListDone()) {
                values.clear();
                for (auto i = 0; i < indexes.size(); i++) {
                    values.push_back(factors[i][indexes[i]]);
                }

                auto min = findMin(values);

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

    static_assert(FindLcm<size_t>(60, 90) == 180);
    static_assert(FindLcm<size_t>(90, 60) == 180);
    static_assert(FindLcm<size_t>(3, 5) == 15);
    static_assert(FindLcm<size_t>(5, 3) == 15);
    static_assert(FindLcm<size_t>(12, 16) == 48);
    static_assert(FindLcm<size_t>(16, 12) == 48);

    template<size_t Lhs, size_t Rhs, typename T>
    constexpr T FindLcm() {
        return detail::FindLcm<T>(GetAllPrimeFactors<Lhs, T>(), GetAllPrimeFactors<Rhs, T>());
    }

    static_assert(FindLcm<60, 90, size_t>() == 180);
    static_assert(FindLcm<3, 5, size_t>() == 15);
    static_assert(FindLcm<12, 16, size_t>() == 48);


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

    static_assert(Eval(20, 40, "<"));
    static_assert(Eval(40, 20, ">"));
    static_assert(Eval(20, 20, "<="));
    static_assert(Eval(20, 20, ">="));
    static_assert(Eval(20, 20, "=="));
    static_assert(Eval(20, 40, "!="));

    static_assert(!Eval(40, 20, "<"));
    static_assert(!Eval(20, 40, ">"));
    static_assert(!Eval(40, 20, "<="));
    static_assert(!Eval(20, 40, ">="));
    static_assert(!Eval(20, 40, "=="));
    static_assert(!Eval(20, 20, "!="));


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

    //ax + by = gcd(a,b)
    //ax + my = 1
    //ax = 1 (mod m)
    //a = 3, M = 11, output == 4
    //(4*3) % 11 = 1
    static_assert(MultiplicativeInverse(3, 5) == 2); //(3 * 2) % 5 == 1
    static_assert(MultiplicativeInverse(3, 11) == 4); //(3 * 4) % 11 = 1
    static_assert(MultiplicativeInverse(3, 7) == 5); //(3 * 5) % 7 == 1

}