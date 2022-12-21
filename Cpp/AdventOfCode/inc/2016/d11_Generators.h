#pragma once

#include "Common.h"

//4 floors
//RTG & Chips
//Chip + RTG is safe
//Chip without RTG can be fried by any other RTG
//Elevator stops on each floor
//Elevator holds at least 1, up to 2 objects

constexpr bool IsFloorStable(const std::string& str) {
    std::vector<char> generators;
    std::vector<char> chips;
    if (str.size() < 2) {
        return true;
    }
    for (size_t i = 0; i < str.size(); i += 2) {
        if (str[i + 1] == 'G') {
            generators.push_back(str[i]);
        }
        else {
            chips.push_back(str[i]);
        }
    }

    for (const auto& chip : chips) {
        bool needsPower = false;
        bool hasPower = false;
        for (const auto& generator : generators) {
            if (generator == chip) {
                hasPower = true;
            }
            else {
                needsPower = true;
            }
        }

        if (needsPower && !hasPower) {
            return false;
        }
    }

    return true;
}
static_assert(IsFloorStable("HGHC"));
static_assert(IsFloorStable("HGLGHC"));
static_assert(IsFloorStable("HGLG"));
static_assert(IsFloorStable("HCLC"));
static_assert(IsFloorStable("HGLGHCLC"));
static_assert(IsFloorStable(" "));

constexpr bool IsStable(const std::string& str) {
    size_t index = 1;
    while (index < str.size()) {
        auto next = str.find(' ', index);
        if (!IsFloorStable(str.substr(index, next - index))) {
            return false;
        }
        if (next == str.npos) {
            break;
        }
        
        index = next + 1;
    }

    return true;
}

static_assert(IsStable("4HMLM HG LG "));
static_assert(IsStable("4LM HGHM LG "));
static_assert(IsStable("4LM  HGHMLG "));
static_assert(IsStable("4HMLM  HGLG "));
static_assert(IsStable("4 HMLM HGLG "));
static_assert(IsStable("4  HGHMLGLM "));
static_assert(IsStable("4  HGLG HMLM"));
static_assert(IsStable("4  HGHMLG LM"));
static_assert(IsStable("4  HM HGLGLM"));
static_assert(IsStable("4  HMLM HGLG"));
static_assert(IsStable("4   HGHMLGLM"));

constexpr bool IsDone(const std::string& floors) {
    return floors[1] == ' ' && floors[2] == ' ' && floors[3] == ' ';
}

static_assert(!IsDone("2HMLM HG LG "));
static_assert(!IsDone("3  HGLG HMLM"));
static_assert(IsDone("4   HGHMLGLM"));

constexpr size_t GetFloorStart(const std::string& floors, u32 floor) {
    u32 currentFloor = 0;
    size_t charIndex = 1;
    while (currentFloor != floor) {
        if (floors[charIndex] == ' ') {
            currentFloor++;
        }
        charIndex++;
    }

    return charIndex;
}

static_assert(GetFloorStart("1AA BB", 0) == 1);
static_assert(GetFloorStart("1AA BB", 1) == 4);

constexpr size_t GetFloorEnd(const std::string& floors, u32 floor) {
    return floors.find(' ', GetFloorStart(floors, floor));
}

constexpr std::string GetFloorString(const std::string& floors, u32 floor) {
    auto start = GetFloorStart(floors, floor);
    auto end = GetFloorEnd(floors, floor);
    return floors.substr(start, end - start);
}

static_assert(GetFloorString("1A B C", 0) == "A");
static_assert(GetFloorString("1A B C", 1) == "B");
static_assert(GetFloorString("1A B C", 2) == "C");
static_assert(GetFloorString("1  A", 2) == "A");
static_assert(GetFloorString("1ABC DEF", 1) == "DEF");
static_assert(GetFloorString("1A B ", 2) == "");

constexpr std::string SortBuilding(const std::string& floors) {
    std::string result;
    std::vector<std::string> pieces;
    for (u32 floor = 0; floor < 4; floor++) {
        auto unsorted = GetFloorString(floors, floor);
        std::vector<std::string> components;
        for (auto i = 0; i < unsorted.size(); i += 2) {
            components.push_back(unsorted.substr(i, 2));
        }

        std::sort(components.begin(), components.end());
        pieces.push_back(Constexpr::JoinVec("", components));
    }

    return floors.substr(0, 1) + Constexpr::JoinVec(' ', pieces);
}

static_assert(SortBuilding("1AA BB CC DD") == "1AA BB CC DD");
static_assert(SortBuilding("1BBAA CCBB DDCC EEDD") == "1AABB BBCC CCDD DDEE");

constexpr std::vector<std::pair<std::string, std::string>> GenerateFloorMoves(const std::string& floor) {
    std::vector<std::pair<std::string, std::string>> result;
    for (size_t i = 0; i < floor.size(); i += 2) {
        std::string current = floor.substr(0, i) + floor.substr(i + 2);
        std::string thing = floor.substr(i, 2);
        result.push_back(std::make_pair(current, thing));

        for (size_t j = i + 2; j < floor.size(); j += 2) {
            auto current2 = floor.substr(0, i) + floor.substr(i + 2, j - i - 2) + floor.substr(j + 2);
            
            std::string thing2 = floor.substr(j, 2);
            result.push_back(std::make_pair(current2, thing + thing2));
        }
    }

    return result;
}

static_assert(GenerateFloorMoves("ABCDEF").size() == 6); //AB ABCD ABEF CD CDEF EF
static_assert(GenerateFloorMoves("ABCDEF")[0].first == "CDEF"); //ABCDEF - AB
static_assert(GenerateFloorMoves("ABCDEF")[0].second == "AB");
static_assert(GenerateFloorMoves("AB").size() == 1); //AB

constexpr std::vector<std::string> GenerateUpMoves(const std::string& floors) {
    u32 santaFloor = floors[0] - '0';
    if (santaFloor > 2) return {};
    auto floorStr = GetFloorString(floors, santaFloor);
    if (floorStr.empty()) return {};

    auto nextFloor = GetFloorString(floors, santaFloor + 1);
    std::string prefix = ToString(santaFloor + 1) + floors.substr(1, GetFloorStart(floors, santaFloor) - 1);
    std::string postfix = "";
    if (GetFloorEnd(floors, santaFloor + 1) != floors.npos) {
        postfix = floors.substr(GetFloorEnd(floors, santaFloor + 1));
    }

    std::vector<std::string> result;
    auto moves = GenerateFloorMoves(floorStr);
    for (const auto& move : moves) {
        auto permutation = SortBuilding(prefix + move.first + " " + nextFloor + move.second + postfix);
        if (IsStable(permutation)) {
            result.push_back(permutation);
        }
    }

    return result;
}

constexpr std::vector<std::string> GenerateDownMoves(const std::string& floors) {
    u32 santaFloor = floors[0] - '0';
    if (santaFloor < 1) return {};
    auto floorStr = GetFloorString(floors, santaFloor);
    if (floorStr.empty()) return {};

    auto nextFloor = GetFloorString(floors, santaFloor - 1);
    std::string prefix = ToString(santaFloor - 1) + floors.substr(1, GetFloorStart(floors, santaFloor - 1) - 1);
    std::string postfix = "";
    if (GetFloorEnd(floors, santaFloor) != floors.npos) {
        postfix = floors.substr(GetFloorEnd(floors, santaFloor));
    }

    std::vector<std::string> result;
    auto moves = GenerateFloorMoves(floorStr);
    for (const auto& move : moves) {
        auto permutation = SortBuilding(prefix + nextFloor + move.second + " " + move.first + postfix);
        if (IsStable(permutation)) {
            result.push_back(permutation);
        }
    }

    return result;
}


u32 Bfs(const std::string& initial, const std::string& target) {
    u32 depth = 0;
    std::unordered_set<std::string> row;
    std::unordered_set<std::string> seen;

    std::vector<std::string> upMoves;
    std::vector<std::string> downMoves;

    auto dedup = [&seen](const std::string& move) -> bool {
        return seen.find(move) == seen.end();
    };

    row.insert(initial);
    while (true) {
        if (row.empty()) {
            return 0; //fail case
        }

        if (row.find(target) != row.end()) {
            return depth;
        }

        depth++;
        std::vector<std::string> currentRow(row.begin(), row.end());
        row.clear();
        for (const auto& e : currentRow) {
            auto generatedUp = GenerateUpMoves(e);
            upMoves.clear();
            std::copy_if(generatedUp.begin(), generatedUp.end(), std::back_inserter(upMoves), dedup);

            auto generatedDown = GenerateDownMoves(e);
            downMoves.clear();
            std::copy_if(generatedDown.begin(), generatedDown.end(), std::back_inserter(downMoves), dedup);

            row.insert(upMoves.begin(), upMoves.end());
            seen.insert(upMoves.begin(), upMoves.end());

            row.insert(downMoves.begin(), downMoves.end());
            seen.insert(downMoves.begin(), downMoves.end());
        }
    }

    return 0;
}

std::string Run(const std::vector<std::string>&) {
    std::string initial = SortBuilding("0SGSMPGPMEGEMDGDM TGRGRMCGCM TM ");
    std::string target = SortBuilding("3   SGSMPGPMTGRGRMCGCMTMEGEMDGDM");

    return ToString(Bfs(initial, target));
}

bool RunTests() {
    std::string initial = SortBuilding("0HMLM HG LG ");
    std::string target =  SortBuilding("3   HMLMHGLG");
    if (Bfs(initial, target) != 11) return false;

    return true;
}