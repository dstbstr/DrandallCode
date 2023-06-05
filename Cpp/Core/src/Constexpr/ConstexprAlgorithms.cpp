#include "Constexpr/ConstexprAlgorithms.h"

namespace Constexpr {
    static_assert(JoinVec(" ", std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec(' ', std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec("Hello", std::vector<std::string>{ "a", "b", "c" }) == "aHellobHelloc");

    static_assert(FindMin(std::vector<size_t>{ 0, 1, 1 }) == 0);
    static_assert(FindMin(std::vector<size_t>{1, 2, 0}) == 0);
    static_assert(FindMin(std::vector<size_t>{1}) == 1);

    static_assert(FindMax(std::vector<size_t>{1, 2, 3}) == 3);
    static_assert(FindMax(std::array<size_t, 3>{3, 2, 1}) == 3);
    static_assert(FindMax(std::vector<int32_t>{-1}) == -1);

    static_assert(GetIndex<3, 3>(0, 0) == 0);
    static_assert(GetIndex<3, 3>(2, 2) == 8);
}