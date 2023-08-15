#include "Constexpr/ConstexprMath.h"

namespace Constexpr {
    static_assert(Abs(3) == 3);
    static_assert(Abs(-3) == 3);

    static_assert(Floor(3.14) == 3.0);
    static_assert(Floor(-3.14) == -3.0);

    static_assert(Log2I(2) == 1, "Log2I(2) should == 1");
    static_assert(Log2I(32) == 5, "Log2I(32) should == 5");
    static_assert(Log2I(33) == 5, "Log2I(33) should == 5");
    static_assert(Log2I(63) == 5, "Log2I(63) should == 5");
    static_assert(Log2I(64) == 6, "Log2I(64) should == 6");
    static_assert(Log2I(3012210) == 21, "Log2I(3012210) should == 21");

    static_assert(Sqrt(9) == 3.0);
    static_assert(Sqrt(1) == 1.0);
    static_assert(Sqrt(-1) != Sqrt(-1)); // no way to test for NaN, but NaN != NaN

    static_assert(Pow(2, 0) == 1);
    static_assert(Pow(2, 4) == 16);

    static_assert(Factorial(5) == 120, "5! should == 120");

    static_assert(MulMod(3u, 5u, 20u) == 15u);
    static_assert(MulMod(3u, 5u, 7u) == 1u);

    static_assert(ModPow(2, 3, 5) == 3); //2^3 == 8 % 5 == 3

    static_assert(EuclideanModulo(-5, 3) == 1);
    static_assert(EuclideanModulo(-2, 6) == 4);

    static_assert(FromBase26(std::string_view("a")) == 0);
    static_assert(FromBase26(std::string_view("z")) == 25);
    static_assert(FromBase26(std::string_view("aa")) == 26);

    static_assert(GetPrimes<size_t>(10) == std::vector<size_t>{2, 3, 5, 7});
    static_assert(GetPrimes<size_t>(7) == std::vector<size_t>{2, 3, 5, 7});

    static_assert(GetPrimes<10, size_t>() == std::vector<size_t>{2, 3, 5, 7});
    static_assert(GetPrimes<7, size_t>() == std::vector<size_t>{2, 3, 5, 7});

    static_assert(GetUniquePrimeFactors<size_t>(10) == std::vector<size_t>{2, 5});
    static_assert(GetUniquePrimeFactors<size_t>(7) == std::vector<size_t>{7});

    static_assert(GetUniquePrimeFactors<10, size_t>() == std::vector<size_t>{2, 5});
    static_assert(GetUniquePrimeFactors<7, size_t>() == std::vector<size_t>{7});

    static_assert(GetAllPrimeFactors<size_t>(16) == std::vector<size_t>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<size_t>(7) == std::vector<size_t>{7});

    static_assert(GetAllPrimeFactors<16, size_t>() == std::vector<size_t>{2, 2, 2, 2});
    static_assert(GetAllPrimeFactors<7, size_t>() == std::vector<size_t>{7});

    static_assert(GetDivisors(24) == std::vector<int>{1, 24, 2, 12, 3, 8, 4, 6});
    static_assert(GetDivisors(9) == std::vector<int>{1, 9, 3});
    static_assert(GetDivisors(1) == std::vector<int>{1});

    /*
    static_assert(FindLcm<size_t>(60, 90) == 180);
    static_assert(FindLcm<size_t>(90, 60) == 180);
    static_assert(FindLcm<size_t>(3, 5) == 15);
    static_assert(FindLcm<size_t>(5, 3) == 15);
    static_assert(FindLcm<size_t>(12, 16) == 48);
    static_assert(FindLcm<size_t>(16, 12) == 48);
    static_assert(FindLcm<size_t>(2, 12) == 12);

    static_assert(FindLcm<60, 90, size_t>() == 180);
    static_assert(FindLcm<3, 5, size_t>() == 15);
    static_assert(FindLcm<12, 16, size_t>() == 48);
    static_assert(FindLcm<2, 12, size_t>() == 12);

    static_assert(FindGcd(15, 70) == 5);
    */

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

    //ax + by = gcd(a,b)
    //ax + my = 1
    //ax = 1 (mod m)
    //a = 3, M = 11, output == 4
    //(4*3) % 11 = 1
    static_assert(MultiplicativeInverse(3, 5) == 2); //(3 * 2) % 5 == 1
    static_assert(MultiplicativeInverse(3, 11) == 4); //(3 * 4) % 11 = 1
    static_assert(MultiplicativeInverse(3, 7) == 5); //(3 * 5) % 7 == 1

    namespace ConstexprMathTests {
        bool RunTests() {
            if(FindLcm<size_t>(60, 90) != 180) return false;
            if(FindLcm<size_t>(90, 60) != 180) return false;
            if(FindLcm<size_t>(3, 5) != 15) return false;
            if(FindLcm<size_t>(5, 3) != 15) return false;
            if(FindLcm<size_t>(12, 16) != 48) return false;
            if(FindLcm<size_t>(16, 12) != 48) return false;
            if(FindLcm<size_t>(2, 12) != 12) return false;

            if(FindLcm<60, 90, size_t>() != 180) return false;
            if(FindLcm<3, 5, size_t>() != 15) return false;
            if(FindLcm<12, 16, size_t>() != 48) return false;
            if(FindLcm<2, 12, size_t>() != 12) return false;

            if(FindGcd(15, 70) != 5) return false;

            return true;
        }
    }

}