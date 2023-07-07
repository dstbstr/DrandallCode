#include "BigInt.h"
#include <sstream>

std::string ToString(const BigInt& bigInt) {
    std::stringstream stream;
    stream << bigInt;
    return stream.str();
}

static_assert(BigInt(1) == BigInt("1"), "1 != 1");
static_assert(BigInt(0) == BigInt(0), "0 != 0");
static_assert(BigInt(0) == 0, "0 != 0");
static_assert(0 == BigInt(0), "0 != 0");
static_assert(BigInt("123'456") == BigInt("123456"), "123'456 != 123456");
static_assert(BigInt(12345) == BigInt("12345"), "12345 != 12345");

static_assert(BigInt(123).ToString() == "123");
static_assert(BigInt("-123").ToString() == "-123");

static_assert(BigInt(7).ToBinary() == "111");
static_assert(BigInt(8).ToBinary() == "1000");

static_assert(BigInt::FromBinary("111") == 7);
static_assert(BigInt::FromBinary("1000") == 8);

static_assert((BigInt::FromBinary("101") & BigInt::FromBinary("111")).ToBinary() == "101");
static_assert((BigInt::FromBinary("101") | BigInt::FromBinary("111")).ToBinary() == "111");
static_assert((BigInt::FromBinary("101") ^ BigInt::FromBinary("111")) == BigInt::FromBinary("010"));

static_assert(BigInt(1) != BigInt(0), "1 == 0");
static_assert(BigInt(1) != 0, "1 == 0");
static_assert(0 != BigInt(1), "0 == 1");

static_assert(BigInt(0) < BigInt(1), "0 >= 1");
static_assert(BigInt(0) < 1, "0 >= 1");
static_assert(0 < BigInt(1), "0 >= 1");
static_assert(BigInt(2) < BigInt(10), "2 >= 10");

static_assert(BigInt(0) <= BigInt(1), "0 > 1");
static_assert(BigInt(0) <= 1, "0 > 1");
static_assert(0 <= BigInt(1), "0 > 1");
static_assert(BigInt(0) <= BigInt(0), "0 > 0");

static_assert(BigInt(1) > BigInt(0), "1 <= 0");
static_assert(BigInt(1) > 0, "1 <= 0");
static_assert(1 > BigInt(0), "1 <= 0");

static_assert(BigInt(1) >= BigInt(0), "1 < 0");
static_assert(BigInt(1) >= 0, "1 < 0");
static_assert(1 >= BigInt(0), "1 < 0");
static_assert(BigInt(1) >= BigInt(1), "1 < 1");

static_assert(++BigInt(1) == BigInt(2), "++1 != 2");
static_assert(BigInt(1)++ == BigInt(1), "1++ != 1");
static_assert(--BigInt(2) == BigInt(1), "--2 != 1");
static_assert(BigInt(2)-- == BigInt(2), "2-- != 2");

static_assert(++BigInt(9) == BigInt(10), "++9 != 10");
static_assert(++BigInt(99) == BigInt(100), "++99 != 100");
static_assert(++BigInt("909") == BigInt(910), "++909 != 910");

static_assert(--BigInt(10) == BigInt(9), "--10 != 9");
static_assert(--BigInt(100) == BigInt(99), "--100 != 99");
static_assert(--BigInt("910") == BigInt(909), "--910 != 909");

static_assert(BigInt(2) + BigInt(2) == BigInt(4), "2 + 2 != 4");
static_assert(BigInt(9) + BigInt(2) == 11, "9 + 2 != 11");
static_assert(BigInt(30) + 6 == 36, "30 + 6 != 36");
static_assert(BigInt(6) + 30 == 36, "6 + 30 != 36");

static_assert(BigInt("123456") + BigInt("654321") == BigInt("777777"), "123456 + 654321 != 777777");

static_assert(BigInt(10) - BigInt(2) == BigInt(8), "10 - 2 != 8");
static_assert(BigInt(2) - BigInt(2) == 0, "2 - 2 != 0");

static_assert(BigInt(3) * 0 == 0, "3 * 0 != 0");
static_assert(BigInt(0) * 3 == 0, "0 * 3 != 0");
static_assert(BigInt(1) * 3 == 3, "3 * 1 != 3");
static_assert(BigInt(3) * 1 == 3, "1 * 3 != 3");
static_assert(BigInt(3)* BigInt(4) == BigInt(12), "3 * 4 != 12");
static_assert(BigInt(123) * 3 == 369, "123 * 3 != 369");
static_assert(3 * BigInt(123) == 369, "3 * 123 != 369");
static_assert(BigInt("999999999999")* BigInt("999999999999") == BigInt("999999999998000000000001"), "999999999999 * 999999999999 != 999999999998000000000001");

static_assert(BigInt(1) / 3 == 0, "1/3 != 0");
static_assert(BigInt(3) / 1 == 3, "3/1 != 3");
static_assert(BigInt(0) / 3 == 0, "0/3 != 0");
static_assert(BigInt(9) / BigInt(3) == BigInt(3), "9 / 3 != 3");
static_assert(BigInt(125) / 5 == 25, "125 / 5 != 25");
static_assert(BigInt(10) / 3 == 3, "10 / 3 != 3");

static_assert(BigInt(9) % BigInt(3) == BigInt(0), "9 % 3 != 0");
static_assert(BigInt(10) % 3 == 1, "10 % 3 != 1");
static_assert(BigInt(111111111111) % 2 == 1, "111111111111 % 2 != 1");


static_assert(BigInt(12) >> 1 == 6, "12 >> 1 != 6");
static_assert(BigInt(6) << 1 == 12, "6 << 1 != 12");
static_assert(BigInt(2) >> 5 == 2 >> 5, "2 >> 5 != 2 >> 5");
static_assert(BigInt(12345) << 5 == 12345 << 5, "12345 << 5 != 12345 << 5");


// Negative tests
static_assert(BigInt(-1) == -1, "-1 != -1");
static_assert(BigInt(-1) != 1, "-1 == 1");
static_assert(BigInt(-1) < 0, "-1 >= 0");
static_assert(BigInt(-1) < 2, "-1 >= 2");
static_assert(BigInt(-2) < -1, "-2 >= -1");

static_assert(++BigInt(-2) == -1, "++(-2) != -1");
static_assert(--BigInt(-1) == -2, "--(-1) != -2");
static_assert(++BigInt(-1) == 0, "++(-1) != 0");
static_assert(--BigInt(0) == -1, "--0 != -1");

static_assert(BigInt(-3) + 3 == 0, "-3 + 3 != 0");
static_assert(3 + BigInt(-3) == 0, "3 + (-3) != 0");
static_assert(BigInt(-3) + BigInt(-3) == -6, "-3 + (-3) != -6");
static_assert(BigInt(3) + BigInt(-5) == -2, "3 + (-5) != -2");

static_assert(BigInt(-3) - 3 == -6, "-3 - 3 != -6");
static_assert(BigInt(-3) - BigInt(-3) == 0, "(-3) - (-3) != 0");
static_assert(BigInt(-3) - BigInt(-5) == 2, "-3 - (-5) != 2");

static_assert(BigInt(-3) * 0 == 0, "3 * 0 != 0");
static_assert(BigInt(0) * -3 == 0, "0 * 3 != 0");
static_assert(BigInt(1) * -3 == -3, "-1 * 3 != -3");
static_assert(BigInt(3) * -1 == -3, "3 * -1 != -3");
static_assert(BigInt(-1) * 3 == -3, "3 * -1 != -3");
static_assert(BigInt(3) * -1 == -3, "-1 * 3 != -3");
static_assert(BigInt(3)* BigInt(-4) == BigInt(-12), "3 * -4 != -12");
static_assert(BigInt(-3) * -4 == 12, "-3 * -4 != 12");

static_assert(BigInt(-1) / 3 == 0, "-1/3 != 0");
static_assert(BigInt(-3) / 1 == -3, "-3/1 != -3");
static_assert(BigInt(-3) / -1 == 3, "-3/-1 != 3");
static_assert(BigInt(3) / -1 == -3, "3/-1 != -3");
static_assert(BigInt(0) / -3 == 0, "0/-3 != 0");
static_assert(BigInt(-9) / 3 == -3, "-9 / 3 != -3");
static_assert(BigInt(9) / -3 == -3, "9 / -3 != -3");
static_assert(BigInt(-9) / -3 == 3, "-9 / -3 != 3");
static_assert(BigInt(-10) / 3 == -3, "-10 / 3 != -3");

static_assert(BigInt(68) % 12 == 8, "68 % 12 != 8"); // 12 * 5 = 60 + 8 == 68
static_assert(BigInt(-68) % 12 == 4, "-68 % 12 != 4"); //12 * -6 = -72 + 4 == -68
static_assert(BigInt(68) % -12 == 8, "68 % -12 != 8"); //-12 * -5 = 60 + 8 == 68
static_assert(BigInt(-68) % -12 == 4, "-68 % -12 != 4"); //-12 * -6 = -72 + 4 = -68

//static_assert(BigInt(-3) + 5 == 2, "-3 + 5 != 2"); //not sure why this fails the static_assert

namespace BigIntTests {
    bool Run() {
        if (BigInt(-3) + 5 != 2) return false;
        return true;
    }
}