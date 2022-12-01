#pragma once

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <regex>

#include "Platform/Types.h"
#include "Utils.h"

class Solution {
    std::unordered_map<std::string, std::unordered_map<std::string, s32>> Data;
    std::unordered_set<std::string> AllPeople;

public:
    void AddPerson(std::string line) {
        static auto re = std::regex(R"((\w+) would (\w+) (\d+) .+?(\w+)\.)");

        auto match = std::smatch{};
        std::regex_search(line, match, re);

        auto lhs = match[1].str();
        auto direction = match[2].str();
        auto amountStr = match[3].str();
        auto rhs = match[4].str();

        s32 amount;
        ParseNumber(amountStr, amount);

        Data[lhs][rhs] = amount * (direction == "gain" ? 1 : -1);
        AllPeople.insert(lhs);
        AllPeople.insert(rhs);
    }

    void AddSelf() {
        for(const auto& person : AllPeople) {
            Data["You"][person] = 0;
            Data[person]["You"] = 0;
        }

        AllPeople.insert("You");
    }

    s32 CalculateHappiness(std::vector<std::string>& people) {
        s32 result = 0;
        std::string current;
        std::string next;

        for(auto i = 0; i < people.size() -1; i++) {
            current = people[i];
            next = people[i + 1];
            result += Data[current][next];
            result += Data[next][current];
        }

        auto first = people[0];
        auto last = people[people.size() -1];
        
        result += Data[first][last];
        result += Data[last][first];

        return result;
    }

    u32 FindBestSeating() {
        auto arrangement = std::vector<std::string>{AllPeople.begin(), AllPeople.end()};
        std::sort(arrangement.begin(), arrangement.end());
        s32 result = -99999;

        do {
            result = std::max(result, CalculateHappiness(arrangement));
        } while(std::next_permutation(arrangement.begin(), arrangement.end()));

        return result;
    }

};

bool RunTests() {
    Solution sol;
    sol.AddPerson("Alice would gain 54 happiness units by sitting next to Bob.");
    sol.AddPerson("Alice would lose 79 happiness units by sitting next to Carol.");
    sol.AddPerson("Alice would lose 2 happiness units by sitting next to David.");
    sol.AddPerson("Bob would gain 83 happiness units by sitting next to Alice.");
    sol.AddPerson("Bob would lose 7 happiness units by sitting next to Carol.");
    sol.AddPerson("Bob would lose 63 happiness units by sitting next to David.");
    sol.AddPerson("Carol would lose 62 happiness units by sitting next to Alice.");
    sol.AddPerson("Carol would gain 60 happiness units by sitting next to Bob.");
    sol.AddPerson("Carol would gain 55 happiness units by sitting next to David.");
    sol.AddPerson("David would gain 46 happiness units by sitting next to Alice.");
    sol.AddPerson("David would lose 7 happiness units by sitting next to Bob.");
    sol.AddPerson("David would gain 41 happiness units by sitting next to Carol.");

    if(sol.FindBestSeating() != 330) {
        return false;
    }
    return true;
}