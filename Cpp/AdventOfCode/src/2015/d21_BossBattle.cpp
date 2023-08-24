#pragma once

#include "2015/d21_BossBattle.h"

SOLUTION(2015, 21) {
    struct Weapon {
        u32 Cost;
        u32 Attack;
    };

    static std::vector<Weapon> weapons = {
        {8, 4},
        {10, 5},
        {25, 6},
        {40, 7},
        {74, 8}
    };

    struct Armor {
        u32 Cost;
        u32 Defense;
    };

    static std::vector<Armor> armors = {
        {13, 1},
        {31, 2},
        {53, 3},
        {75, 4},
        {102, 5}
    };

    struct Ring {
        u32 Cost;
        u32 Attack;
        u32 Defense;
    };
    static std::vector<Ring> rings = {
        {25, 1, 0},
        {50, 2, 0},
        {100, 3, 0},
        {20, 0, 1},
        {40, 0, 2},
        {80, 0, 3}
    };

    struct Character {
        u32 Hp;
        u32 Attack;
        u32 Defense;
    };

    constexpr u32 CountRounds(const Character & attacker, const Character & defender) {
        s32 dmg = attacker.Attack - defender.Defense;
        if (dmg <= 0) {
            dmg = 1;
        }
        u32 dmgPerRound = static_cast<u32>(dmg);
        if (defender.Hp % dmgPerRound == 0) {
            return defender.Hp / dmgPerRound;
        }
        else {
            return defender.Hp / dmgPerRound + 1;
        }
    }

    constexpr bool DoesPlayerWin(const Character & player, const Character & boss) {
        return CountRounds(player, boss) <= CountRounds(boss, player);
    }

    struct EquipmentIndexes {
        size_t Weapon;
        size_t Armor;
        size_t Ring1;
        size_t Ring2;
    };

    constexpr Character BuildPlayer(const EquipmentIndexes & e) {
        Character player = { 100, 0, 0 };

        player.Attack += weapons[e.Weapon].Attack;
        if (e.Armor < armors.size()) {
            player.Defense += armors[e.Armor].Defense;
        }
        if (e.Ring1 < rings.size()) {
            auto ring1 = rings[e.Ring1];
            player.Attack += ring1.Attack;
            player.Defense += ring1.Defense;
        }
        if (e.Ring2 < rings.size()) {
            auto ring2 = rings[e.Ring2];
            player.Attack += ring2.Attack;
            player.Defense += ring2.Defense;
        }

        return player;
    }

    constexpr u32 CalculateCost(const EquipmentIndexes & e) {
        u32 cost = weapons[e.Weapon].Cost;
        if (e.Armor < armors.size()) {
            cost += armors[e.Armor].Cost;
        }
        if (e.Ring1 < rings.size()) {
            cost += rings[e.Ring1].Cost;
        }
        if (e.Ring2 < rings.size()) {
            cost += rings[e.Ring2].Cost;
        }

        return cost;
    }

    constexpr u32 FindBestBuild(const Character& boss) {
        u32 bestCost = 999;

        for (size_t weaponIndex = 0; weaponIndex < weapons.size(); weaponIndex++) {
            for (size_t armorIndex = 0; armorIndex <= armors.size(); armorIndex++) {
                for (size_t ring1Index = 0; ring1Index <= rings.size() + 1; ring1Index++) {
                    for (size_t ring2Index = 0; ring2Index < ring1Index; ring2Index++) {
                        auto equipment = EquipmentIndexes{ weaponIndex, armorIndex, ring1Index, ring2Index };
                        if (DoesPlayerWin(BuildPlayer(equipment), boss)) {
                            bestCost = std::min(bestCost, CalculateCost(equipment));
                        }
                    }
                }
            }
        }

        return bestCost;
    }

    constexpr u32 FindWorstBuild(const Character& boss) {
        u32 worstCost = 0;

        for (size_t weaponIndex = 0; weaponIndex < weapons.size(); weaponIndex++) {
            for (size_t armorIndex = 0; armorIndex <= armors.size(); armorIndex++) {
                for (size_t ring1Index = 0; ring1Index <= rings.size() + 1; ring1Index++) {
                    for (size_t ring2Index = 0; ring2Index < ring1Index; ring2Index++) {
                        auto equipment = EquipmentIndexes{ weaponIndex, armorIndex, ring1Index, ring2Index };
                        if (!DoesPlayerWin(BuildPlayer(equipment), boss)) {
                            worstCost = std::max(worstCost, CalculateCost(equipment));
                        }
                    }
                }
            }
        }

        return worstCost;
    }

    constexpr Character ParseBoss(const auto& lines) {
        Character result;
        auto hp = Constexpr::Split(lines[0], ": ")[1];
        auto dmg = Constexpr::Split(lines[1], ": ")[1];
        auto armor = Constexpr::Split(lines[2], ": ")[1];

        Constexpr::ParseNumber(hp, result.Hp);
        Constexpr::ParseNumber(dmg, result.Attack);
        Constexpr::ParseNumber(armor, result.Defense);
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(FindBestBuild(ParseBoss(Lines)));
    }
    PART_TWO() {
        return Constexpr::ToString(FindWorstBuild(ParseBoss(Lines)));
    }
    TESTS() {
        static_assert(CountRounds({ 8, 5, 5 }, { 12, 7, 2 }) == 4);
        static_assert(CountRounds({ 12, 7, 2 }, { 8, 5, 5 }) == 4);
        
        static_assert(DoesPlayerWin({ 8, 5, 5 }, { 12, 7, 2 }) == true);
        
        return true;
    }
}