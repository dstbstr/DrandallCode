#pragma once

#include "2015/d21_BossBattle.h"

SOLUTION(2015, 21) {
    struct Equipment { s32 Cost; s32 Attack; s32 Defense; };
    struct Player { 
        s32 Hp, Attack, Defense, TotalCost;
        constexpr Player() : Hp(0), Attack(0), Defense(0), TotalCost(0) {}
        constexpr Player(s32 hp, s32 attack, s32 defense) : Hp(hp), Attack(attack), Defense(defense), TotalCost(0) {}
        constexpr Player(Equipment weapon, Equipment armor, Equipment ring1, Equipment ring2) : Hp(100) {
            Attack = weapon.Attack + ring1.Attack + ring2.Attack;
            Defense = armor.Defense + ring1.Defense + ring2.Defense;
            TotalCost = weapon.Cost + armor.Cost + ring1.Cost + ring2.Cost;
        }
    };

    constexpr u32 CountRounds(const Player& attacker, const Player& defender) {
        auto dmg = std::max(1, attacker.Attack - defender.Defense);
        return defender.Hp / dmg + (defender.Hp % dmg > 0);
    }

    static std::vector<Equipment> Weapons = { {8, 4, 0}, {10,5,0}, {25,6,0}, {40,7,0}, {74,8,0} };
    static std::vector<Equipment> Armors = { {0, 0, 0}, {13, 0, 1}, {31, 0, 2}, {53, 0, 3}, {75, 0, 4}, {102, 0, 5} };
    static std::vector<Equipment> Rings = { {0, 0, 0}, {0, 0, 0}, {25, 1, 0}, {50, 2, 0}, {100, 3, 0}, {20, 0, 1}, {40, 0, 2}, {80, 0, 3} };

    constexpr std::vector<Player> GetBuilds() {
        std::vector<Player> result;
        for (const auto& weapon : Weapons) {
            for (const auto& armor : Armors) {
                for (auto r1 = 0; r1 < Rings.size(); r1++) {
                    for (auto r2 = r1 + 1; r2 < Rings.size(); r2++) {
                        result.push_back(Player{ weapon, armor, Rings[r1], Rings[r2] });
                    }
                }
            }
        }
        return result;
    }

    constexpr Player ParseBoss(const std::vector<std::string>& lines) {
        Player result;
        Constexpr::ParseNumber(Constexpr::Split(lines[0], " ").back(), result.Hp);
        Constexpr::ParseNumber(Constexpr::Split(lines[1], " ").back(), result.Attack);
        Constexpr::ParseNumber(Constexpr::Split(lines[2], " ").back(), result.Defense);
        return result;
    }

    PART_ONE() {
        auto boss = ParseBoss(lines);
        auto builds = GetBuilds();
        std::sort(builds.begin(), builds.end(), [](const Player& lhs, const Player& rhs) {
            return lhs.TotalCost < rhs.TotalCost;
        });
        for (const auto& build : builds) {
            if (CountRounds(build, boss) <= CountRounds(boss, build)) {
                return Constexpr::ToString(build.TotalCost);
            }
        }
        return "Not Found";
    }

    PART_TWO() {
        auto boss = ParseBoss(lines);
        auto builds = GetBuilds();
        std::sort(builds.begin(), builds.end(), [](const Player& lhs, const Player& rhs) {
            return rhs.TotalCost < lhs.TotalCost;
            });
        for (const auto& build : builds) {
            if (CountRounds(boss, build) < CountRounds(build, boss)) {
                return Constexpr::ToString(build.TotalCost);
            }
        }
        return "Not Found";
    }

    TESTS() {
        static_assert(CountRounds({ 8, 5, 5 }, { 12, 7, 2 }) == 4);
        static_assert(CountRounds({ 12, 7, 2 }, { 8, 5, 5 }) == 4);
        return true;
    }
}
