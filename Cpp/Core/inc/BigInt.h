#pragma once

#include <string>
#include <vector>
#include <array>
#include <ostream>

#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"

struct BigInt {
private:
    std::string digits;
    bool negative = false;

    constexpr bool IsZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }
    constexpr bool IsOne() const {
        return !negative && digits.size() == 1 && digits[0] == 1;
    }

public:
    constexpr BigInt() {
        digits.push_back(0);
    }

    template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    constexpr BigInt(T number = 0) {
        if constexpr (std::is_signed_v<T>) {
            if (number < 0) {
                negative = true;
                number = -number;
            }
        }

        do {
            digits.push_back(static_cast<char>(number % 10));
            number /= 10;
        } while (number > 0);
    }

    ~BigInt() = default;
    constexpr explicit BigInt(const std::string& number) {
        for (auto it = number.rbegin(); it != number.rend(); it++) {
            auto c = *it;
            if (c >= '0' && c <= '9') {
                digits.push_back(c - '0');
            }
            else {
                if (c == '-' && it == number.rend() - 1) {
                    negative = true;
                }
                else {
                    throw("Bad number");
                }
            }
        }
    }
    constexpr explicit BigInt(const char* number) {
        for (int i = static_cast<int>(std::char_traits<char>::length(number)) - 1; i >= 0; i--) {
            auto c = number[i];
            if (c >= '0' && c <= '9') {
                digits.push_back(c - '0');
            }
            else {
                if (c == '\'') continue;
                else if (i == 0 && c == '-') {
                    negative = true;
                }
                else {
                    throw("Bad number");
                }
            }
        }
    }

    constexpr BigInt(const BigInt& other) {
        digits = other.digits;
        negative = other.negative;
    }

    constexpr std::string ToBinary() const {
        auto running = *this;
        std::string result;
        while (running > 0) {
            result += running.IsEven() ? "0" : "1";
            running >>= 1;
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    static constexpr BigInt FromBinary(const std::string& bin) {
        BigInt result;
        if (bin.empty()) return result;

        BigInt factor = 1;
        for (size_t i = bin.size() - 1; i > 0; i--) {
            result += factor * (bin[i] == '1' ? 1 : 0);
            factor *= 2;
        }
        result += factor * (bin[0] == '1' ? 1 : 0);

        return result;
    }

    constexpr BigInt GetBitCount() const {
        auto bin = this->ToBinary();
        return std::count(bin.begin(), bin.end(), '1');
    }

    constexpr BigInt& operator=(const BigInt& other) {
        digits = other.digits;
        negative = other.negative;
        return *this;
    }
    constexpr BigInt& operator=(const BigInt&& other) {
        digits = other.digits;
        negative = other.negative;
        return *this;
    }

    friend constexpr BigInt operator-(BigInt num) {
        num.negative = !num.negative;
        return num;
    }

    friend constexpr bool operator==(const BigInt& lhs, const BigInt& rhs) {
        return lhs.negative == rhs.negative && lhs.digits == rhs.digits;
    }
    friend constexpr bool operator!=(const BigInt& lhs, const BigInt& rhs) {
        return !(lhs == rhs);
    }
    friend constexpr bool operator<(const BigInt& lhs, const BigInt& rhs) {
        if (lhs.negative != rhs.negative) return lhs.negative;

        if (lhs.digits.size() != rhs.digits.size()) {
            return lhs.negative ? rhs.digits.size() < lhs.digits.size() : lhs.digits.size() < rhs.digits.size();
        }
        auto i = lhs.digits.size();
        while (i-- > 0) {
            if (lhs.digits[i] != rhs.digits[i]) return lhs.negative ? rhs.digits[i] < lhs.digits[i] : lhs.digits[i] < rhs.digits[i];
        }
        return false;
    }

    friend constexpr bool operator>(const BigInt& lhs, const BigInt& rhs) {
        return rhs < lhs;
    }
    friend constexpr bool operator<=(const BigInt& lhs, const BigInt& rhs) {
        return !(rhs < lhs);
    }
    friend constexpr bool operator>=(const BigInt& lhs, const BigInt& rhs) {
        return !(lhs < rhs);
    }

    friend constexpr BigInt operator-(BigInt lhs, const BigInt& rhs) {
        return lhs -= rhs;
    }

    constexpr BigInt& operator++() {
        if (negative) {
            negative = false;
            --(*this);
            negative = !IsZero();
            return *this;
        }
        int i = 0;
        auto n = digits.size();
        for (i = 0; i < n && digits[i] == 9; i++) {
            digits[i] = 0;
        }

        if (i == n) digits.push_back(1);
        else digits[i]++;

        return *this;
    }
    constexpr BigInt operator++(int) {
        auto original = *this;
        ++(*this);
        return original;
    }

    constexpr BigInt& operator--() {
        int i = 0;
        auto n = digits.size();
        if (negative) {
            negative = false;
            ++(*this);
            negative = true;
            return *this;
        }
        if (IsZero()) {
            digits[0] = 1;
            negative = true;
            return *this;
        }
        for (i = 0; i < n && digits[i] == 0; i++) {
            digits[i] = 9;
        }

        digits[i]--;
        if (n > 1 && digits[n - 1] == 0) {
            digits.pop_back();
            n--;
        }

        return *this;
    }

    constexpr BigInt operator--(int) {
        auto original = *this;
        --(*this);
        return original;
    }

    friend constexpr BigInt& operator+=(BigInt& lhs, const BigInt& rhs) {
        if (lhs.negative != rhs.negative) {
            if (lhs.negative) {
                if (-lhs < rhs) { //-3 + 5 == 2
                    lhs = rhs + lhs;
                }
                else { //-5 + 3 == -2
                    lhs = -lhs - rhs;
                }
                return lhs;
            }
            else {
                lhs -= -rhs;
                return lhs;
            }
        }

        auto lhsSize = lhs.digits.size();
        auto rhsSize = rhs.digits.size();
        if (lhsSize < rhsSize) {
            lhs.digits.append(rhsSize - lhsSize, 0);
        }
        lhsSize = lhs.digits.size();

        char sum = 0;
        char carry = 0;
        for (auto i = 0; i < lhsSize; i++) {
            sum = lhs.digits[i] + carry;
            if (i < rhsSize) sum += rhs.digits[i];
            carry = sum / 10;
            lhs.digits[i] = sum % 10;
        }
        if (carry > 0) {
            lhs.digits.push_back(carry);
        }

        return lhs;
    }
    friend constexpr BigInt operator+(BigInt lhs, const BigInt& rhs) {
        return lhs += rhs;
    }

    friend constexpr BigInt& operator-=(BigInt& lhs, const BigInt& rhs) {
        if (lhs.negative != rhs.negative) {
            return lhs += -rhs;
        }

        if (rhs.negative) {
            return lhs += -rhs; //-3 - (-5) == -3 + 5 == 2
        }
        if (lhs < rhs) {
            lhs = rhs - lhs;
            lhs = -lhs;
            return lhs;
        }

        char carry = 0;
        int diff = 0;
        auto lhsSize = lhs.digits.size();
        auto rhsSize = rhs.digits.size();
        for (auto i = 0; i < lhsSize; i++) {
            diff = static_cast<int>(lhs.digits[i] + carry);
            if (i < rhsSize) {
                diff -= rhs.digits[i];
            }

            if (diff < 0) {
                diff += 10;
                carry = -1;
            }
            else {
                carry = 0;
            }
            lhs.digits[i] = static_cast<char>(diff);
        }

        while (lhs.digits.size() > 1 && lhs.digits.back() == 0) {
            lhs.digits.pop_back();
        }

        if (lhs.IsZero()) lhs.negative = false;
        return lhs;
    }

    friend constexpr BigInt& operator*=(BigInt& lhs, const BigInt& rhs) {
        if (lhs.IsZero() || rhs.IsZero()) {
            lhs = 0;
            return lhs;
        }

        if (lhs.IsOne() || rhs.IsOne()) {
            lhs = lhs.IsOne()  ? rhs : lhs;
            return lhs;
        }
        bool finalNegative = lhs.negative != rhs.negative;
        BigInt mul = rhs;
        mul.negative = false;
        lhs.negative = false;

        if (lhs.IsOne() || mul.IsOne()) {
            lhs = lhs.IsOne() ? rhs : lhs;
            lhs.negative = finalNegative;
            return lhs;
        }

        auto lhsSize = lhs.digits.size();
        auto rhsSize = rhs.digits.size();
        std::vector<int> v(lhsSize + rhsSize, 0);
        for (auto i = 0; i < lhsSize; i++) {
            for (auto j = 0; j < rhsSize; j++) {
                v[i + j] += lhs.digits[i] * rhs.digits[j];
            }
        }
        lhs.digits.resize(v.size());
        lhsSize = v.size();
        int carry = 0;
        int sum = 0;
        for (auto i = 0; i < lhsSize; i++) {
            sum = carry + v[i];
            v[i] = sum % 10;
            carry = sum / 10;
            lhs.digits[i] = static_cast<char>(v[i]);
        }
        for (auto i = lhsSize - 1; i > 0 && v[i] == 0; i--) {
            lhs.digits.pop_back();
        }

        lhs.negative = finalNegative;
        return lhs;
    }
    friend constexpr BigInt operator*(BigInt lhs, const BigInt& rhs) {
        return lhs *= rhs;
    }
    
    friend constexpr BigInt& operator/=(BigInt& lhs, const BigInt& rhs) {
        if (rhs.IsZero()) throw("Divide by zero");
        if (lhs.IsZero()) {
            lhs.negative = false;
            return lhs;
        }
        bool finalNegative = lhs.negative != rhs.negative;
        BigInt divisor = rhs;
        divisor.negative = false;
        lhs.negative = false;

        if (divisor.IsOne()) {
            lhs.negative = finalNegative;
            return lhs;
        }
        else if (lhs == divisor) {
            lhs = BigInt(1);
            lhs.negative = finalNegative;
            return lhs;
        }
        else if (lhs < divisor) {
            lhs = 0;
            return lhs;
        }
        
        int index = 0;
        int lgcat = 0;
        char cc = 0;
        const auto lhsSize = lhs.digits.size();
        std::vector<char> cat(lhsSize, 0);
        BigInt temp;

        for (index = static_cast<int>(lhsSize) - 1; temp * 10 + lhs.digits[index] < divisor; index--) {
            temp *= 10;
            temp += lhs.digits[index];
        }
        for (; index >= 0; index--) {
            temp = temp * 10 + lhs.digits[index];
            for (cc = 9; cc * divisor > temp; cc--);
            temp -= cc * divisor;
            cat[lgcat++] = cc;
        }
        lhs.digits.resize(cat.size());
        for (index = 0; index < lgcat; index++) {
            lhs.digits[index] = cat[lgcat - index - 1];
        }
        lhs.digits.resize(lgcat);

        lhs.negative = finalNegative;
        return lhs;
    }

    friend constexpr BigInt operator/(BigInt lhs, const BigInt& rhs) {
        return lhs /= rhs;
    }

    friend constexpr BigInt& operator%=(BigInt& lhs, const BigInt& rhs) {
        if (rhs.IsZero()) throw("Division by 0");
        if (rhs == 2) {
            lhs = lhs.IsEven() ? 0 : 1;
            return lhs;
        }
        if (lhs.negative) {
            if (rhs.negative) { //-68 % -12 == 4
                lhs = lhs - ((lhs + rhs) / rhs) * rhs;
            }
            else { //-68 % 12 == 4
                lhs = lhs - ((lhs - rhs) / rhs) * rhs;
            }
            return lhs;
        }
        else if (rhs.negative) { //68 % -12 == 8
            lhs = lhs - ((lhs / rhs) * rhs);
            return lhs;
        }
        if (lhs < rhs) return lhs;

        lhs = lhs - ((lhs / rhs) * rhs);
        return lhs;
    }
    friend constexpr BigInt operator%(BigInt lhs, const BigInt& rhs) {
        return lhs %= rhs;
    }

    friend constexpr BigInt& operator>>=(BigInt& lhs, const BigInt& rhs) {
        if (rhs < 0) {
            return lhs <<= -rhs;
        }
        else if (rhs < 31) {
            auto shifts = rhs.to_ull();
            auto divisor = 1ull << shifts;
            return lhs /= divisor;
        }
        else {
            auto divisor = BigInt(1) << rhs;
            return lhs /= divisor;
        }
    }

    friend constexpr BigInt operator>>(BigInt lhs, const BigInt& rhs) {
        return lhs >>= rhs;
    }

    friend constexpr BigInt& operator<<=(BigInt& lhs, const BigInt& rhs) {
        if (rhs < 0) {
            return lhs >>= -rhs;
        }
        else if (rhs < 31) {
            auto shifts = rhs.to_ull();
            auto mul = 1ull << shifts;
            return lhs *= mul;
        }
        else {
            for (long i = 0; i < rhs; i++) {
                lhs *= 2;
            }
            return lhs;
        }
    }

    friend constexpr BigInt operator<<(BigInt lhs, const BigInt& rhs) {
        return lhs <<= rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const BigInt& val) {
        for (auto it = val.digits.rbegin(); it != val.digits.rend(); it++) {
            stream << static_cast<short>(*it);
        }
        return stream;
    }

    friend constexpr BigInt& operator&=(BigInt& lhs, const BigInt& rhs) {
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;
        std::string result = std::string(maxSize - minSize, '0');

        for (auto i = 0; i < minSize; i++) {
            result += ((lBin[startA + i] == '1') && (rBin[startB + i] == '1')) ? '1' : '0';
        }
        lhs = BigInt::FromBinary(result);
        return lhs;
    }

    friend constexpr BigInt operator&(BigInt lhs, const BigInt& rhs) {
        return lhs &= rhs;
        /*
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;
        std::string result = std::string(maxSize - minSize, '0');

        for (auto i = 0; i < minSize; i++) {
            result += ((lBin[startA + i] == '1') && (rBin[startB + i] == '1')) ? '1' : '0';
        }
        return BigInt::FromBinary(result);
        */
    }
    friend constexpr BigInt& operator|=(BigInt& lhs, const BigInt& rhs) {
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;

        std::string result = lBin.size() == maxSize ? lBin.substr(0, maxSize - minSize) : rBin.substr(0, maxSize - minSize);

        for (auto i = 0; i < minSize; i++) {
            result += ((lBin[startA + i] == '1') || (rBin[startB + i] == '1')) ? '1' : '0';
        }
        lhs = BigInt::FromBinary(result);
        return lhs;
    }

    friend constexpr BigInt operator|(BigInt lhs, const BigInt& rhs) {
        return lhs |= rhs;
        /*
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;
        
        std::string result = lBin.size() == maxSize ? lBin.substr(0, maxSize - minSize) : rBin.substr(0, maxSize - minSize);

        for (auto i = 0; i < minSize; i++) {
            result += ((lBin[startA + i] == '1') || (rBin[startB + i] == '1')) ? '1' : '0';
        }
        return BigInt::FromBinary(result);
        */
    }

    friend constexpr BigInt operator^=(BigInt& lhs, const BigInt& rhs) {
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;
        std::string result = lBin.size() == maxSize ? lBin.substr(0, maxSize - minSize) : rBin.substr(0, maxSize - minSize);

        for (auto i = 0; i < minSize; i++) {
            result += (lBin[startA + i] != rBin[startB + i]) ? '1' : '0';
        }
        lhs = BigInt::FromBinary(result);
        return lhs;
    }
    friend constexpr BigInt operator^(BigInt lhs, const BigInt& rhs) {
        return lhs ^= rhs;
        /*
        auto lBin = lhs.ToBinary();
        auto rBin = rhs.ToBinary();
        auto minSize = std::min(lBin.size(), rBin.size());
        auto maxSize = std::max(lBin.size(), rBin.size());
        size_t startA = lBin.size() - minSize;
        size_t startB = rBin.size() - minSize;
        std::string result = lBin.size() == maxSize ? lBin.substr(0, maxSize - minSize) : rBin.substr(0, maxSize - minSize);

        for (auto i = 0; i < minSize; i++) {
            result += (lBin[startA + i] != rBin[startB + i]) ? '1' : '0';
        }
        return BigInt::FromBinary(result);
        */
    }

    //This needs to know how big the BigInt should be, but there's no way to pass that information in
    // I.E. ~0 shouldn't be 1
    
    //friend constexpr BigInt operator~(const BigInt& val) {
    //    std::string result = val.ToBinary();
    //    for (size_t i = 0; i < result.size(); i++) {
    //        result[i] = result[i] == '1' ? '0' : '1';
    //    }

    //    return BigInt::FromBinary(result);
    //}

    constexpr bool is_ull() const {
        return !negative && *this <= std::numeric_limits<unsigned long long>::max();
    }
    constexpr bool is_ll() const {
        return *this <= std::numeric_limits<long long>::max() && *this >= std::numeric_limits<long long>::min();
    }
    constexpr bool IsEven() const {
        return digits[0] % 2 == 0;
    }

    constexpr unsigned long long to_ull() const {
        if (!is_ull()) throw ("Overflow");
        unsigned long long result = 0;
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
            result *= 10;
            result += digits[i];
        }
        return result;
    }
    constexpr long long to_ll() const {
        if (!is_ll()) throw ("Overflow");
        long long result = 0;
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
            result *= 10;
            result += digits[i];
        }
        if (negative) {
            result = -result;
        }
        return result;
    }
};

std::string ToString(const BigInt& bigInt);

namespace BigIntTests {
    bool Run();
}