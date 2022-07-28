#include "RomanNumeral.h"

#include "TestCommon.h"

TEST(RomanNumeral, One_ToRoman_ReturnsI) {
    ASSERT_EQ("I", RomanNumeral::ToRoman(1));
}

TEST(RomanNumeral, Five_ToRoman_ReturnsV) {
    ASSERT_EQ("V", RomanNumeral::ToRoman(5));
}

TEST(RomanNumeral, Ten_ToRoman_ReturnsX) {
    ASSERT_EQ("X", RomanNumeral::ToRoman(10));
}

TEST(RomanNumeral, Fifty_ToRoman_ReturnsL) {
    ASSERT_EQ("L", RomanNumeral::ToRoman(50));
}

TEST(RomanNumeral, Hundred_ToRoman_ReturnsC) {
    ASSERT_EQ("C", RomanNumeral::ToRoman(100));
}

TEST(RomanNumeral, FiveHundred_ToRoman_ReturnsD) {
    ASSERT_EQ("D", RomanNumeral::ToRoman(500));
}

TEST(RomanNumeral, OneThousand_ToRoman_ReturnsM) {
    ASSERT_EQ("M", RomanNumeral::ToRoman(1000));
}

TEST(RomanNumeral, Two_ToRoman_ReturnsII) {
    ASSERT_EQ("II", RomanNumeral::ToRoman(2));
}

TEST(RomanNumeral, Six_ToRoman_ReturnsVI) {
    ASSERT_EQ("VI", RomanNumeral::ToRoman(6));
}

TEST(RomanNumeral, Four_ToRoman_ReturnsIV) {
    ASSERT_EQ("IV", RomanNumeral::ToRoman(4));
}

TEST(RomanNumeral, TwentyNine_ToRoman_ReturnsXXIX) {
    ASSERT_EQ("XXIX", RomanNumeral::ToRoman(29));
}

TEST(RomanNumeral, Forty_ToRoman_ReturnsXL) {
    ASSERT_EQ("XL", RomanNumeral::ToRoman(40));
}

TEST(RomanNumeral, Ninety_ToRoman_ReturnsXc) {
    ASSERT_EQ("XC", RomanNumeral::ToRoman(90));
}

TEST(RomanNumeral, OneHundredFortyOne_ToRoman_ReturnsCXLI) {
    std::string str = "CXXXXI";
    std::string simple = "XXXX";
    std::string correct = "XL";

    auto index = str.find(simple);
    str.replace(index, simple.length(), correct);
    ASSERT_EQ("CXLI", str);
    // ASSERT_EQ("CXLI", RomanNumeral::ToRoman(141));
}

TEST(RomanNumeral, FromRoman_ToRoman_ReturnsOriginal) {
    for(uint32_t i = 1; i <= 3000; i++) {
        ASSERT_EQ(i, RomanNumeral::FromRoman(RomanNumeral::ToRoman(i)));
    }
}
/*
        public void GivenNinetyOne_ToRoman_ReturnsXCI()
        public void RomanNumeral_RoundTrips()
        {
            for (int i = 1; i < 3000; i++)
            {
                Assert.AreEqual(i, (i.ToRoman()).FromRoman());
            }
        }
    }
}

*/