#include "Constexpr/ConstexprStrUtils.h"

namespace Constexpr {
    namespace StrUtilsTests {
        template<typename T>
        constexpr bool TestParseNumber(const std::string& str, T expected) {
            T actual;
            auto success = ParseNumber<T>(str, actual);
            return success && actual == expected;
        }

        bool Run() {
            static_assert(Strlen("hello") == 5);
            static_assert(Strlen("a") == 1);
            static_assert(Strlen("") == 0);
            if(Strlen("hello") != 5) return false;
            if(Strlen("a") != 1) return false;
            if(Strlen("") != 0) return false;

            static_assert(IsDigit('3'));
            static_assert(!IsDigit('A'));
            static_assert(IsDigit('0'));
            static_assert(IsDigit('9'));
            if(!IsDigit('3')) return false;
            if(IsDigit('A')) return false;
            if(!IsDigit('0')) return false;
            if(!IsDigit('9')) return false;

            static_assert(Constexpr::ToString(10) == "10");
            static_assert(Constexpr::ToString(1234) == "1234");
            static_assert(Constexpr::ToString(-1234) == "-1234");
            if(Constexpr::ToString(10) != "10") return false;
            if(Constexpr::ToString(1234) != "1234") return false;
            if(Constexpr::ToString(-1234) != "-1234") return false;

            static_assert(Split("a b c", " ").size() == 3);
            static_assert(Split("abc", " ").size() == 1);
            if(Split("a b c", " ").size() != 3) return false;
            if(Split("abc", " ").size() != 1) return false;

            static_assert(TestParseNumber<unsigned int>("1234", 1234));
            static_assert(TestParseNumber<int>("-1234", -1234));
            static_assert(TestParseNumber<size_t>("123454321", 123454321));
            static_assert(TestParseNumber<long long>("1234567654321", 1234567654321));
            if(!TestParseNumber<unsigned int>("1234", 1234)) return false;
            if(!TestParseNumber<int>("-1234", -1234)) return false;
            if(!TestParseNumber<size_t>("123454321", 123454321)) return false;
            if(!TestParseNumber<long long>("1234567654321", 1234567654321)) return false;

            static_assert(TestParseNumber("42", 42));
            static_assert(TestParseNumber("-42", -42));
            static_assert(TestParseNumber("0", unsigned int(0)));
            static_assert(!TestParseNumber("-1", int(1)));
            static_assert(!TestParseNumber("abc", 0));
            if(!TestParseNumber("42", 42)) return false;
            if(!TestParseNumber("-42", -42)) return false;
            if(!TestParseNumber("0", unsigned int(0))) return false;
            if(TestParseNumber("-1", int(1))) return false;
            if(TestParseNumber("abc", 0)) return false;

            static_assert(HexToDec('0') == 0);
            static_assert(HexToDec('9') == 9);
            static_assert(HexToDec('a') == 10);
            static_assert(HexToDec('f') == 15);
            static_assert(HexToDec('A') == 10);
            static_assert(HexToDec('F') == 15);
            if(HexToDec('0') != 0) return false;
            if(HexToDec('9') != 9) return false;
            if(HexToDec('a') != 10) return false;
            if(HexToDec('f') != 15) return false;
            if(HexToDec('A') != 10) return false;
            if(HexToDec('F') != 15) return false;

            static_assert(ParseHex("FF") == 255);
            static_assert(ParseHex("2A") == 42);
            static_assert(ParseHex("2a") == 42);
            if(ParseHex("FF") != 255) return false;
            if(ParseHex("2A") != 42) return false;
            if(ParseHex("2a") != 42) return false;

            static_assert(ReplaceAllCopy("abc", "a", "") == "bc");
            static_assert(ReplaceAllCopy("aaa", "a", "b") == "bbb");
            static_assert(ReplaceAllCopy("abbabbabb", "bb", "c") == "acacac");
            if(ReplaceAllCopy("abc", "a", "") != "bc") return false;
            if(ReplaceAllCopy("aaa", "a", "b") != "bbb") return false;
            if(ReplaceAllCopy("abbabbabb", "bb", "c") != "acacac") return false;

            using namespace std::string_view_literals;

            static_assert(RemoveAllOfCopy("abcabcabc"sv, "cb") == "aaa");
            static_assert(RemoveAllOfCopy("abcabcabc"sv, std::string("cb")) == "aaa");
            static_assert(RemoveAllOfCopy("abcabcabc"sv, 'b') == "acacac");
            if(RemoveAllOfCopy("abcabcabc"sv, "cb") != "aaa") return false;
            if(RemoveAllOfCopy("abcabcabc"sv, std::string("cb")) != "aaa") return false;
            if(RemoveAllOfCopy("abcabcabc"sv, 'b') != "acacac") return false;

            return true;
        }
    }
}