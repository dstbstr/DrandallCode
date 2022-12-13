#pragma once

#include <vector>
#include <array>

namespace Constexpr {
    template<typename T>
    constexpr auto Sum(const T& container) {
        auto result = 0;
        for (auto e : container) {
            result += e;
        }

        return result;
    }

    static_assert(Sum(std::vector{ 1,2,3 }) == 6);
    static_assert(Sum(std::array{ -1, -2, -3 }) == -6);

    template<typename T>
    constexpr T Abs(T t) {
        return t < 0 ? -t : t;
    }

    static_assert(Abs(3) == 3);
    static_assert(Abs(-3) == 3);

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

    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, Collection&& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, const Collection& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(char&& delimiter, Collection const& input) {
        return JoinVec(std::string(1, delimiter), input);
    }

    static_assert(JoinVec(" ", std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec(' ', std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec("Hello", std::vector<std::string>{ "a", "b", "c" }) == "aHellobHelloc");


    constexpr std::vector<std::string_view> Split(std::string_view input, std::string_view delimiter) {
        size_t last = 0;
        size_t next = 0;
        std::vector<std::string_view> result;

        while ((next = input.find(delimiter, last)) != std::string::npos) {
            if (next - last > 0) {
                result.push_back(input.substr(last, next - last));
            }
            last = next + delimiter.size();
        }

        auto lastEntry = input.substr(last);
        if (lastEntry.length() > 0) {
            result.push_back(lastEntry);
        }
        return result;
    }

    static_assert(Split("a b c", " ").size() == 3);
    static_assert(Split("abc", " ").size() == 1);

    template<typename T>
    constexpr bool ParseNumber(std::string_view input, T& result, typename std::enable_if<std::is_signed<T>::value>::type* = 0) {
        u32 place = 1;
        u32 pos = input[0] == '-' ? 1 : 0;

        result = 0;
        for (size_t i = input.size() - 1; i != pos; i--) {
            if (input[i] < '0' || input[i] > '9') {
                return false;
            }
            result += (input[i] - '0') * place;
            place *= 10;
        }
        result += (input[pos] - '0') * place;
#pragma warning(push)
#pragma warning(disable:4146)
        if (input[0] == '-') {
            result = -result;
        }
#pragma warning(pop)
        return true;
    }

    template<typename T>
    constexpr bool ParseNumber(std::string_view input, T& result, typename std::enable_if<!std::is_signed<T>::value>::type* = 0) {
        u32 place = 1;
        if (input[0] == '-') {
            return false;
        }

        result = 0;
        for (size_t i = input.size() - 1; i != 0; i--) {
            if (input[i] < '0' || input[i] > '9') {
                return false;
            }
            result += (input[i] - '0') * place;
            place *= 10;
        }
        result += (input[0] - '0' * place);

        return true;
    }

    template<typename T>
    constexpr std::vector<T> GetPrimes(T max) {
        static_assert(!std::is_floating_point_v<T>);
        std::vector<bool> candidates{};
        candidates.reserve(max + 1);
        for (size_t i = 0; i < max + 1; i++) {
            candidates.push_back(true);
        }

        std::vector<T> result{};
        auto maxFactor = static_cast<T>(Sqrt(max + 1));
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

    static_assert(GetPrimes<u32>(10) == std::vector<u32>{2, 3, 5, 7});
    static_assert(GetPrimes<u32>(7) == std::vector<u32>{2, 3, 5, 7});

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

    static_assert(GetPrimes<10, u32>() == std::vector<u32>{2, 3, 5, 7});
    static_assert(GetPrimes<7, u32>() == std::vector<u32>{2, 3, 5, 7});

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

    static_assert(GetUniquePrimeFactors<u32>(10) == std::vector<u32>{2, 5});
    static_assert(GetUniquePrimeFactors<u32>(7) == std::vector<u32>{7});

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

    static_assert(GetUniquePrimeFactors<10, u32>() == std::vector<u32>{2, 5});
    static_assert(GetUniquePrimeFactors<7, u32>() == std::vector<u32>{7});

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

    static_assert(GetAllPrimeFactors<u32>(16) == std::vector<u32>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<u32>(7) == std::vector<u32>{7});

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

    static_assert(GetAllPrimeFactors<16, u32>() == std::vector<u32>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<7, u32>() == std::vector<u32>{7});

    namespace detail {
        template<typename T>
        constexpr T FindLcm(std::vector<T> leftFactors, std::vector<T> rightFactors) {
            T result = 1;
            size_t leftIndex = 0;
            size_t rightIndex = 0;

            while (leftIndex < leftFactors.size() && rightIndex < rightFactors.size()) {
                auto left = leftFactors[leftIndex];
                auto right = rightFactors[rightIndex];
                if (left == right) {
                    result *= left;
                    leftIndex++;
                    rightIndex++;
                }
                else if (left < right) {
                    result *= left;
                    leftIndex++;
                }
                else {
                    result *= right;
                    rightIndex++;
                }
            }

            if (leftIndex < leftFactors.size()) {
                for (; leftIndex < leftFactors.size(); leftIndex++) {
                    result *= leftFactors[leftIndex];
                }
            }
            else if (rightIndex < rightFactors.size()) {
                for (; rightIndex < rightFactors.size(); rightIndex++) {
                    result *= rightFactors[rightIndex];
                }
            }

            return result;
        }
    }

    template<typename T>
    constexpr T FindLcm(T lhs, T rhs) {
        return detail::FindLcm(GetAllPrimeFactors(lhs), GetAllPrimeFactors(rhs));
    }

    static_assert(FindLcm<u32>(60, 90) == 180);
    static_assert(FindLcm<u32>(90, 60) == 180);
    static_assert(FindLcm<u32>(3, 5) == 15);
    static_assert(FindLcm<u32>(5, 3) == 15);
    static_assert(FindLcm<u32>(12, 16) == 48);
    static_assert(FindLcm<u32>(16, 12) == 48);

    template<size_t Lhs, size_t Rhs, typename T>
    constexpr T FindLcm() {
        return detail::FindLcm(GetAllPrimeFactors<Lhs, T>(), GetAllPrimeFactors<Rhs, T>());
    }
    
    static_assert(FindLcm<60, 90, u32>() == 180);
    static_assert(FindLcm<3, 5, u32>() == 15);
    static_assert(FindLcm<12, 16, u32>() == 48);

    namespace ConstexprTests {
        template<typename T>
        constexpr bool TestParseNumber(const std::string& input, T expectedOutput) {
            T actual;
            if (!ParseNumber<T>(input, actual)) return false;
            return expectedOutput == actual;
        }

        static_assert(TestParseNumber("42", 42));
        static_assert(TestParseNumber("-42", -42));
        static_assert(TestParseNumber("0", u32(0)));
        static_assert(!TestParseNumber("-1", s32(1)));
        static_assert(!TestParseNumber("abc", 0));
    }
}