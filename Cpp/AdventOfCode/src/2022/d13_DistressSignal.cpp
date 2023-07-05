#include "2022/d13_DistressSignal.h"

SOLUTION(2022, 13) {
    constexpr size_t FindClose(const std::string & input) {
        int depth = 0;
        for (size_t i = 0; i < input.size(); i++) {
            if (input[i] == '[') {
                depth++;
            }
            else if (input[i] == ']') {
                depth--;
                if (depth == 0) {
                    return i;
                }
            }
        }

        return 0; //something went wrong
    }
    static_assert(FindClose("[abc]") == 4);
    static_assert(FindClose("[[a],[b]]") == 8);
    static_assert(FindClose("[a][b]") == 2);

    struct Pair {
        std::string AsString{ "" };
        bool IsNumber{ false };
        u32 Num{ 0 };
        std::vector<Pair> List{};

        constexpr static Pair ToList(u32 num) {
            return Pair("[" + Constexpr::ToString(num) + "]");
        }

        constexpr Pair(const std::string& line) {
            AsString = line;
            if (line[0] == '[') {
                //make list
                std::string numStr;
                char c;
                for (size_t i = 1; i < line.size(); i++) {
                    c = line[i];
                    if (c == '[') {
                        auto substr = line.substr(i);
                        auto close = FindClose(substr);
                        List.push_back(Pair(line.substr(i, close + 1)));
                        i += close + 1;
                    }
                    else if (c == ',' || c == ']') {
                        if (!numStr.empty()) {
                            List.push_back(Pair(numStr));
                            numStr.clear();
                        }
                    }
                    else {
                        numStr += c;
                    }
                }
                IsNumber = false;
            }
            else {
                Constexpr::ParseNumber(line, Num);
                IsNumber = true;
            }
        }

        /*
        constexpr auto operator<=>(const Pair& other) {
            if (IsNumber && other.IsNumber) {
                return Num <=> other.Num;
            }
            else if (IsNumber) {
                return Pair::ToList(Num) <=> other;
            }
            else if (other.IsNumber) {
                return *this <=> Pair::ToList(other.Num);
            }
            else {
                for (size_t i = 0; i < List.size(); i++) {
                    if (i >= other.List.size()) {
                        return std::strong_ordering::greater;
                    }
                    auto comp = List[i] <=> other.List[i];
                    if (comp != std::strong_ordering::equal) {
                        return comp;
                    }
                }

                return List.size() == other.List.size() ? std::strong_ordering::equal : std::strong_ordering::less;
            }
        }
        */

        constexpr auto operator==(const Pair& other) {
            if (IsNumber && other.IsNumber) {
                return Num == other.Num;
            }
            else if (IsNumber) {
                return Pair::ToList(Num) == other;
            }
            else if (other.IsNumber) {
                return this->operator==(Pair::ToList(other.Num));
                /*
                auto lhs = *this;
                auto rhs = Pair::ToList(other.Num);
                return lhs == rhs;
                */
                //return (*this) == Pair::ToList(other.Num);
            }
            else {
                for (size_t i = 0; i < List.size(); i++) {
                    if (i >= other.List.size()) {
                        return false;
                    }

                    if (!(List[i] == other.List[i])) {
                        return false;
                    }
                }
                return List.size() == other.List.size();
            }
        }
        constexpr auto operator<(const Pair& other) {
            if (IsNumber && other.IsNumber) {
                return Num < other.Num;
            }
            else if (IsNumber) {
                return Pair::ToList(Num) < other;
            }
            else if (other.IsNumber) {
                return *this < Pair::ToList(other.Num);
            }
            else {
                for (size_t i = 0; i < List.size(); i++) {
                    if (i >= other.List.size()) {
                        return false;
                    }
                    if (List[i] < other.List[i]) {
                        return true;
                    }
                }
            }
            return List.size() < other.List.size();
            //return (*this <=> other) < 0;
        }
    };

    bool IsOrdered(const std::string & lhs, const std::string & rhs) {
        auto left = Pair(lhs);
        auto right = Pair(rhs);

        //return (left <=> right) < 0;
        return left < right;
    }

    u32 SumOrderedPairs(const std::vector<std::string>&lines) {
        u32 result = 0;
        int pairIndex = 1;
        for (auto i = 0; i < lines.size(); i += 3) {
            if (IsOrdered(lines[i], lines[i + 1])) {
                result += pairIndex;
            }
            pairIndex++;
        }

        return result;
    }

    constexpr void ExtractKey(const std::string & str, size_t index, size_t & key1Index, size_t & key2Index) {
        auto key1 = std::string("[[2]]");
        auto key2 = std::string("[[6]]");

        if (key1Index == 0 && str == key1) key1Index = index + 1;
        if (key2Index == 0 && str == key2) key2Index = index + 1;
    }

    u32 FindDecoderKey(const std::vector<std::string>&lines) {
        std::vector<Pair> pairs;
        for (size_t i = 0; i < lines.size(); i += 3) {
            pairs.push_back(Pair(lines[i]));
            pairs.push_back(Pair(lines[i + 1]));
        }

        pairs.push_back(Pair("[[2]]"));
        pairs.push_back(Pair("[[6]]"));

        std::sort(pairs.begin(), pairs.end());
        size_t key1Index = 0;
        size_t key2Index = 0;

        for (size_t i = 0; i < pairs.size(); i++) {
            ExtractKey(pairs[i].AsString, i, key1Index, key2Index);
        }

        return static_cast<u32>(key1Index * key2Index);
    }
    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(SumOrderedPairs(lines));
        return Constexpr::ToString(FindDecoderKey(lines));
    }

    bool RunTests() {
        /*
        if (!IsOrdered("[1,1,3,1,1]", "[1,1,5,1,1]")) return false;
        if (!IsOrdered("[[1],[2,3,4]]", "[[1],4]")) return false;
        if (!IsOrdered("[[4,4],4,4]", "[[4,4],4,4,4]")) return false;
        if (!IsOrdered("[]", "[3]")) return false;
        if (!IsOrdered("[[[[],3],[5,[1],[8,5],10,[5,8]]],[],[1]]", "[[9,[4,9]]]")) return false;

        if (IsOrdered("[9]", "[[8,7,6]]")) return false;
        if (IsOrdered("[7,7,7,7]", "[7,7,7]")) return false;
        if (IsOrdered("[[[]]]", "[[]]")) return false;
        */

        std::vector<std::string> lines = {
            "[1,1,3,1,1]",
            "[1,1,5,1,1]",
            "",
            "[[1],[2,3,4]]",
            "[[1],4]",
            "",
            "[9]",
            "[[8,7,6]",
            "",
            "[[4,4],4,4]",
            "[[4,4],4,4,4]",
            "",
            "[7,7,7,7]",
            "[7,7,7]",
            "",
            "[]",
            "[3]",
            "",
            "[[[]]]",
            "[[]]",
            "",
            "[1,[2,[3,[4,[5,6,7]]]],8,9]",
            "[1,[2,[3,[4,[5,6,0]]]],8,9]",
        };

        if (FindDecoderKey(lines) != 140) return false;

        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}