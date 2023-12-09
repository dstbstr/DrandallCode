#include "2020/d19_SeaMonster.h"
#include "Constexpr/ConstexprRegex.h"

SOLUTION(2020, 19) {
    using Entries = Constexpr::SmallMap<std::string, std::vector<std::string>>;

    constexpr Entries ParseEntries(const auto& lines) {
        Entries result;
        for (const auto& line : lines) {
            auto s1 = Constexpr::Split(line, ": ");
            auto s2 = Constexpr::Split(s1[1], " | ");
            std::vector<std::string> entries;
            for (const auto& entry : s2) {
                std::string e = std::string(entry);
                e.erase(std::remove(e.begin(), e.end(), '"'), e.end());
                entries.push_back(e);
            }
            auto key = std::string(s1[0]);
            result[key] = entries;
        }

        return result;
    }

    using Cache = Constexpr::SmallMap<std::string, std::string>;

    constexpr std::string Recurse(const std::string & key, const Entries & entries, Cache & cache) {
        if (cache.contains(key)) {
            return cache.at(key);
        }
        const auto& values = entries.at(key);
        if (values.size() == 1) {
            if (values[0] == "a" || values[0] == "b") {
                cache[key] = values[0];
            }
            else {
                auto split = Constexpr::Split(values[0], " ");
                std::string result = "";
                for (auto s : split) {
                    result += Recurse(std::string(s), entries, cache);
                }
                cache[key] = result;
            }
        }
        else {
            bool hasLoop = false;
            for (const auto& v : values) {
                for (auto s : Constexpr::Split(v, " ")) {
                    if (s == key) {
                        hasLoop = true;
                        break;
                    }
                }
                if (hasLoop) break;
            }

            if (hasLoop) {
                std::string fortyTwo = Recurse("42", entries, cache);
                std::string result = "";
                if (key == "8") {
                    result = "(" + fortyTwo + ")+";
                }
                else {
                    std::vector<std::string> choices;
                    std::string thirtyOne = Recurse("31", entries, cache);
                    for (auto i = 0; i < 4; i++) {
                        std::string choice;
                        for (auto j = 0; j < i; j++) {
                            choice += "(" + fortyTwo + ")";
                        }
                        choice += "(" + fortyTwo + thirtyOne + ")";
                        for (auto j = 0; j < i; j++) {
                            choice += "(" + thirtyOne + ")";
                        }
                        choices.push_back(choice);
                    }
                    result = "(" + Constexpr::JoinVec('|', choices) + ")";
                }
                cache[key] = result;
            }
            else {
                std::vector<std::string> toJoin;
                for (const auto& value : values) {
                    auto split = Constexpr::Split(value, " ");
                    std::string result = "";
                    for (auto s : split) {
                        result += Recurse(std::string(s), entries, cache);
                    }
                    toJoin.push_back(result);
                }
                auto result = "(" + Constexpr::JoinVec('|', toJoin) + ")";
                cache[key] = result;
            }
        }

        return cache[key];
    }

    constexpr std::string ParseRules(const std::vector<std::string>&lines, bool isPartTwo = false) {
        auto entries = ParseEntries(lines);
        if (isPartTwo) {
            entries["8"].push_back("42 8");
            entries["11"].push_back("42 11 31");
        }
        Cache cache;
        cache["a"] = "a";
        cache["b"] = "b";
        return Recurse("0", entries, cache);
    }

    constexpr size_t Solve(const auto& lines, bool isPartTwo) {
        auto groups = SplitInputIntoGroups(lines);
        auto rules = ParseRules(groups[0], isPartTwo);

        Constexpr::Regex::Re re(rules);
        return std::count_if(groups[1].begin(), groups[1].end(), [&](const std::string& line) {
            return re.Matches(line);
            });
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, false));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, true));
    }

    TESTS() {
        std::vector<std::string> lines = {
           "0: 4 1 5",
           "1: 2 3 | 3 2",
           "2: 4 4 | 5 5",
           "3: 4 5 | 5 4",
           "4: \"a\"",
           "5: \"b\"",
           "",
           "ababbb",
           "bababa",
           "abbbab",
           "aaabbb",
           "aaaabbb"
        };

        if (Solve(lines, false) != 2) return false;

        lines = {
            "42: 9 14 | 10 1",
            "9: 14 27 | 1 26",
            "10: 23 14 | 28 1",
            "1: \"a\"",
            "11: 42 31",
            "5: 1 14 | 15 1",
            "19: 14 1 | 14 14",
            "12: 24 14 | 19 1",
            "16: 15 1 | 14 14",
            "31: 14 17 | 1 13",
            "6: 14 14 | 1 14",
            "2: 1 24 | 14 4",
            "0: 8 11",
            "13: 14 3 | 1 12",
            "15: 1 | 14",
            "17: 14 2 | 1 7",
            "23: 25 1 | 22 14",
            "28: 16 1",
            "4: 1 1",
            "20: 14 14 | 1 15",
            "3: 5 14 | 16 1",
            "27: 1 6 | 14 18",
            "14: \"b\"",
            "21: 14 1 | 1 14",
            "25: 1 1 | 1 14",
            "22: 14 14",
            "8: 42",
            "26: 14 22 | 1 20",
            "18: 15 15",
            "7: 14 5 | 1 21",
            "24: 14 1",
            "",
            "abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa",
            "bbabbbbaabaabba",
            "babbbbaabbbbbabbbbbbaabaaabaaa",
            "aaabbbbbbaaaabaababaabababbabaaabbababababaaa",
            "bbbbbbbaaaabbbbaaabbabaaa",
            "bbbababbbbaaaaaaaabbababaaababaabab",
            "ababaaaaaabaaab",
            "ababaaaaabbbaba",
            "baabbaaaabbaaaababbaababb",
            "abbbbabbbbaaaababbbbbbaaaababb",
            "aaaaabbaabaaaaababaa",
            "aaaabbaaaabbaaa",
            "aaaabbaabbaaaaaaabbbabbbaaabbaabaaa",
            "babaaabbbaaabaababbaabababaaab",
            "aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba"
        };

        if (Solve(lines, false) != 3) return false;
        if (Solve(lines, true) != 12) return false;
        
        return true;
    }
}