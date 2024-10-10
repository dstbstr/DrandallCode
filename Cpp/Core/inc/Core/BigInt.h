#pragma once

#include <string>
#include <vector>
#include <array>
#include <ostream>
#include <algorithm>

#include "Constexpr/ConstexprMath.h"
#include "Constexpr/ConstexprStrUtils.h"

struct BigInt {
private:
    std::string digits;
    bool negative = false;

    constexpr bool IsZero() const;
    constexpr bool IsOne() const;
    constexpr bool IsEven() const;

public:
    //Ctor/Dtors

    constexpr BigInt();
    constexpr explicit BigInt(const std::string& number);
    constexpr explicit BigInt(const char* number);
    constexpr explicit BigInt(bool val);

    template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
    constexpr BigInt(T number = 0);

    constexpr BigInt(const BigInt& other);
    constexpr BigInt(BigInt&& other);
    constexpr BigInt& operator=(const BigInt& other);
    constexpr BigInt& operator=(const BigInt&& other);

    ~BigInt() = default;

    // Comparisons

    friend constexpr bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator==(const BigInt& lhs, bool rhs);
    friend constexpr bool operator==(const BigInt& lhs, char rhs);
    friend constexpr bool operator==(const BigInt& lhs, int rhs);
    friend constexpr bool operator==(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator==(bool lhs, const BigInt& rhs);
    friend constexpr bool operator==(char lhs, const BigInt& rhs);
    friend constexpr bool operator==(int lhs, const BigInt& rhs);
    friend constexpr bool operator==(size_t lhs, const BigInt& rhs);

    friend constexpr bool operator!=(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator!=(const BigInt& lhs, bool rhs);
    friend constexpr bool operator!=(const BigInt& lhs, char rhs);
    friend constexpr bool operator!=(const BigInt& lhs, int rhs);
    friend constexpr bool operator!=(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator!=(bool lhs, const BigInt& rhs);
    friend constexpr bool operator!=(char lhs, const BigInt& rhs);
    friend constexpr bool operator!=(int lhs, const BigInt& rhs);
    friend constexpr bool operator!=(size_t lhs, const BigInt& rhs);

    friend constexpr bool operator<(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator<(size_t lhs, const BigInt& rhs);
    friend constexpr bool operator<(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator<(long long lhs, const BigInt& rhs);
    friend constexpr bool operator<(const BigInt& lhs, int rhs);
    friend constexpr bool operator<(int lhs, const BigInt& rhs);

    friend constexpr bool operator<=(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator<=(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator<=(const BigInt& lhs, long long rhs);
    friend constexpr bool operator<=(const BigInt& lhs, int rhs);
    friend constexpr bool operator<=(int lhs, const BigInt& rhs);

    friend constexpr bool operator>(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator>(const BigInt& lhs, int rhs);
    friend constexpr bool operator>(int lhs, const BigInt& rhs);
    friend constexpr bool operator>(size_t lhs, const BigInt& rhs);
    friend constexpr bool operator>(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator>(long long lhs, const BigInt& rhs);
    friend constexpr bool operator>(const BigInt& lhs, long long rhs);

    friend constexpr bool operator>=(const BigInt& lhs, const BigInt& rhs);
    friend constexpr bool operator>=(size_t lhs, const BigInt& rhs);
    friend constexpr bool operator>=(const BigInt& lhs, size_t rhs);
    friend constexpr bool operator>=(long long lhs, const BigInt& rhs);
    friend constexpr bool operator>=(const BigInt& lhs, long long rhs);
    friend constexpr bool operator>=(const BigInt& lhs, int rhs);
    friend constexpr bool operator>=(int lhs, const BigInt& rhs);

    // Unary Operations

    friend constexpr BigInt operator-(BigInt num);
    constexpr BigInt& operator++();
    constexpr BigInt operator++(int);
    constexpr BigInt& operator--();
    constexpr BigInt operator--(int);

    // Binary Operations

    friend constexpr BigInt operator+(BigInt lhs, const BigInt& rhs);
    friend constexpr BigInt operator+(BigInt lhs, bool rhs);
    friend constexpr BigInt operator+(BigInt lhs, char rhs);
    friend constexpr BigInt operator+(BigInt lhs, int rhs);
    friend constexpr BigInt operator+(BigInt lhs, size_t rhs);
    friend constexpr BigInt operator+(bool lhs, BigInt rhs);
    friend constexpr BigInt operator+(char lhs, BigInt rhs);
    friend constexpr BigInt operator+(int lhs, BigInt rhs);
    friend constexpr BigInt operator+(size_t lhs, BigInt rhs);

    friend constexpr BigInt& operator+=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt& operator+=(BigInt& lhs, bool rhs);
    friend constexpr BigInt& operator+=(BigInt& lhs, int rhs);
    friend constexpr BigInt& operator+=(BigInt& lhs, char rhs);
    friend constexpr BigInt& operator+=(BigInt& lhs, size_t rhs);

    friend constexpr BigInt operator-(BigInt lhs, const BigInt& rhs);
    friend constexpr BigInt operator-(BigInt lhs, bool rhs);
    friend constexpr BigInt operator-(BigInt lhs, char rhs);
    friend constexpr BigInt operator-(BigInt lhs, int rhs);
    friend constexpr BigInt operator-(BigInt lhs, size_t rhs);
    friend constexpr BigInt operator-(bool lhs, BigInt rhs);
    friend constexpr BigInt operator-(char lhs, BigInt rhs);
    friend constexpr BigInt operator-(int lhs, BigInt rhs);
    friend constexpr BigInt operator-(size_t lhs, BigInt rhs);

    friend constexpr BigInt& operator-=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt& operator-=(BigInt& lhs, bool rhs);
    friend constexpr BigInt& operator-=(BigInt& lhs, char rhs);
    friend constexpr BigInt& operator-=(BigInt& lhs, int rhs);
    friend constexpr BigInt& operator-=(BigInt& lhs, size_t rhs);

    friend constexpr BigInt operator*(BigInt lhs, const BigInt& rhs);
    friend constexpr BigInt operator*(BigInt lhs, bool rhs);
    friend constexpr BigInt operator*(BigInt lhs, char rhs);
    friend constexpr BigInt operator*(BigInt lhs, int rhs);
    friend constexpr BigInt operator*(BigInt lhs, size_t rhs);
    friend constexpr BigInt operator*(bool lhs, BigInt rhs);
    friend constexpr BigInt operator*(char lhs, BigInt rhs);
    friend constexpr BigInt operator*(int lhs, BigInt rhs);
    friend constexpr BigInt operator*(size_t lhs, BigInt rhs);

    friend constexpr BigInt& operator*=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt& operator*=(BigInt& lhs, int rhs);
    friend constexpr BigInt& operator*=(BigInt& lhs, char rhs);
    friend constexpr BigInt& operator*=(BigInt& lhs, bool rhs);
    friend constexpr BigInt& operator*=(BigInt& lhs, size_t rhs);

    friend constexpr BigInt& operator/=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt& operator/=(BigInt& lhs, bool rhs);
    friend constexpr BigInt& operator/=(BigInt& lhs, char rhs);
    friend constexpr BigInt& operator/=(BigInt& lhs, int rhs);
    friend constexpr BigInt& operator/=(BigInt& lhs, size_t rhs);

    friend constexpr BigInt operator/(BigInt lhs, const BigInt& rhs);
    friend constexpr BigInt operator/(BigInt lhs, bool rhs) = delete; //let's not divide by 0
    friend constexpr BigInt operator/(BigInt lhs, char rhs);
    friend constexpr BigInt operator/(BigInt lhs, int rhs);
    friend constexpr BigInt operator/(BigInt lhs, size_t rhs);
    friend constexpr BigInt operator/(bool lhs, const BigInt& rhs) = delete; // BigInt doesn't support decimals
    friend constexpr BigInt operator/(char lhs, const BigInt& rhs);
    friend constexpr BigInt operator/(int lhs, const BigInt& rhs);
    friend constexpr BigInt operator/(size_t lhs, const BigInt& rhs);

    friend constexpr BigInt& operator%=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt& operator%=(BigInt& lhs, bool rhs) = delete; //avoiding divide by 0
    friend constexpr BigInt& operator%=(BigInt& lhs, char rhs);
    friend constexpr BigInt& operator%=(BigInt& lhs, int rhs);
    friend constexpr BigInt& operator%=(BigInt& lhs, size_t rhs);

    friend constexpr BigInt operator%(BigInt lhs, const BigInt& rhs);
    friend constexpr BigInt operator%(BigInt lhs, bool rhs) = delete; //avoiding divide by 0
    friend constexpr BigInt operator%(BigInt lhs, char rhs);
    friend constexpr BigInt operator%(BigInt lhs, int rhs);
    friend constexpr BigInt operator%(BigInt lhs, size_t rhs);
    friend constexpr BigInt operator%(bool lhs, const BigInt& rhs);
    friend constexpr BigInt operator%(char lhs, const BigInt& rhs);
    friend constexpr BigInt operator%(int lhs, const BigInt& rhs);
    friend constexpr BigInt operator%(size_t lhs, const BigInt& rhs);

    friend constexpr BigInt& operator>>=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt operator>>(BigInt lhs, const BigInt& rhs);

    friend constexpr BigInt& operator<<=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt operator<<(BigInt lhs, const BigInt& rhs);

    // String Operations

    inline friend std::ostream& operator<<(std::ostream& stream, const BigInt& val);
    constexpr std::string ToString() const;

    //Bit Manipulation

    constexpr std::string ToBinary() const;
    static constexpr BigInt FromBinary(const std::string& bin);
    constexpr BigInt GetBitCount() const;

    friend constexpr BigInt& operator&=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt operator&(BigInt lhs, const BigInt& rhs);

    friend constexpr BigInt& operator|=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt operator|(BigInt lhs, const BigInt& rhs);

    friend constexpr BigInt operator^=(BigInt& lhs, const BigInt& rhs);
    friend constexpr BigInt operator^(BigInt lhs, const BigInt& rhs);

    //Type conversions

    constexpr explicit operator bool() const;
    constexpr explicit operator char() const;
    constexpr explicit operator long long() const;
    constexpr explicit operator unsigned long long() const;

    constexpr bool is_ull() const;
    constexpr bool is_ll() const;

    constexpr unsigned long long to_ull() const;
    constexpr long long to_ll() const;
};

namespace BigIntTests {
    bool Run();
}

#include "BigInt.inl"