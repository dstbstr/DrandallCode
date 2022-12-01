#pragma once

#include "Common.h"

#include <iostream>

struct Player {
    s32 Hp;
    s32 Mana;
    s32 Armor;
};

/*
Hit Points: 58
Damage: 9
*/
struct Boss {
    s32 Hp;
    s32 Damage;

    void Attack(Player& player) {
        auto dmg = Damage - player.Armor;
        if(dmg < 1) dmg = 1;
        player.Hp -= dmg;
    }
};

enum class Spell {
    MagicMissle,
    Drain,
    Shield,
    Poison,
    Recharge
};

u32 GetManaCost(Spell spell) {
    switch(spell) {
        case Spell::MagicMissle: return 53;
        case Spell::Drain: return 73;
        case Spell::Shield: return 113;
        case Spell::Poison: return 173;
        case Spell::Recharge: return 229;
        default: return 0;
    }
}

void TickSpell(Spell spell, Player& player, Boss& boss) {
    switch(spell) {
        case Spell::MagicMissle: boss.Hp -= 4; break;
        case Spell::Drain: boss.Hp -= 2; player.Hp += 2; break;
        case Spell::Shield: player.Armor = 7; break;
        case Spell::Poison: boss.Hp -= 3; break;
        case Spell::Recharge: player.Mana += 101; break;
    }
}

u32 GetSpellDuration(Spell spell) {
    switch(spell) {
        case Spell::Shield: return 6;
        case Spell::Poison: return 6;
        case Spell::Recharge: return 5;
        default: return 1;
    }
}

void OnSpellEnd(Spell spell, Player& player, Boss&) {
    switch(spell) {
        case Spell::Shield: player.Armor = 0; break;
        default: break;
    }
}

void TickSpells(Player& player, Boss& boss, std::vector<std::pair<Spell, u32>>& activeSpells, std::vector<Spell>& availableSpells) {
    for(auto&[spell, duration] : activeSpells) {
        TickSpell(spell, player, boss);
        duration--;
        if(duration == 0) {
            OnSpellEnd(spell, player, boss);
        }
    }
    for(s32 i = static_cast<s32>(activeSpells.size()) -1; i >= 0; i--) {
        auto[spell, duration] = activeSpells[i];
        if(duration == 0) {
            activeSpells.erase(activeSpells.begin() + i);
            availableSpells.push_back(spell);
        }
    }
}

static std::vector<Spell> CastSpells{};

void BeforeCast(Player& player, Spell spell, std::vector<std::pair<Spell, u32>>& activeSpells, std::vector<Spell>& availableSpells) {
    player.Mana -= GetManaCost(spell);
    activeSpells.push_back(std::make_pair(spell, GetSpellDuration(spell)));
    availableSpells.erase(std::find(availableSpells.begin(), availableSpells.end(), spell));
    CastSpells.push_back(spell);
}

void AfterCast(Player& player, Spell spell, std::vector<std::pair<Spell, u32>>& activeSpells, std::vector<Spell>& availableSpells) {
    player.Mana += GetManaCost(spell);
    activeSpells.erase(std::find_if(activeSpells.begin(), activeSpells.end(), [spell](std::pair<Spell, u32> active){
        return active.first == spell;
    }));
    availableSpells.push_back(spell);
    CastSpells.pop_back();
}

u32 Recurse(Player player, Boss boss, std::vector<std::pair<Spell, u32>> activeSpells, std::vector<Spell> availableSpells, bool first = true) {
    //hard mode
    player.Hp--;
    if(player.Hp == 0) {
        return 1000000;
    }

    if(!first) {
        TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
        if(boss.Hp <= 0) {
            //std::cout << "Beat the boss with " << CastSpells.size() << " Spells\n";
            return 0; //Win
        }

        boss.Attack(player); // boss turn
        if(player.Hp <= 0) {
            return 1000000; //Dead
        }

        TickSpells(player, boss, activeSpells, availableSpells); // start player turn
    }

    u32 best = 1000000;
    auto nextAvailable = availableSpells;
    auto nextActive = activeSpells;

    if(CastSpells.size() > 25) {
        return 1000000; // let's hope we found something shorter
    }
    //choose a spell
    for(auto spell : availableSpells) {
        auto manaCost = GetManaCost(spell);
        if(static_cast<s32>(manaCost) > player.Mana) continue;

        BeforeCast(player, spell, nextActive, nextAvailable); // player turn
        best = std::min(best, manaCost + Recurse(player, boss, nextActive, nextAvailable, false));
        AfterCast(player, spell, nextActive, nextAvailable);
    }

    return best;
}

#define ASSERT(cond) if(!(cond)) return false;

u32 FindBestMana(const Player& player, const Boss& boss) {
    std::vector<std::pair<Spell, u32>> activeSpells;
    auto availableSpells = std::vector<Spell>{Spell::MagicMissle, Spell::Drain, Spell::Shield, Spell::Poison, Spell::Recharge};

    return Recurse(player, boss, activeSpells, availableSpells);
}

u32 RunBattles() {
    Player player = {50, 500, 0};
    Boss boss = {58, 9};

    return FindBestMana(player, boss);
}

bool RunTests() {
    Player player = {10, 250, 0};
    Boss boss = {13, 8};
    ASSERT(FindBestMana(player, boss) == 226);

    player = {10, 250, 0};
    boss = {14, 8};
    ASSERT(FindBestMana(player, boss) == 641);

    /*    
    std::vector<std::pair<Spell, u32>> activeSpells;
    auto availableSpells = std::vector<Spell>{Spell::MagicMissle, Spell::Drain, Spell::Shield, Spell::Poison, Spell::Recharge};

    ASSERT(player.Hp == 10);
    ASSERT(player.Armor == 0);
    ASSERT(player.Mana == 250);
    ASSERT(boss.Hp == 14);
    BeforeCast(player, Spell::Recharge, activeSpells, availableSpells); // player turn
    ASSERT(player.Mana == 21);

    TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
    ASSERT(player.Hp == 10);
    ASSERT(player.Armor == 0);
    ASSERT(player.Mana == 122);
    ASSERT(boss.Hp == 14);

    boss.Attack(player); //boss turn
    ASSERT(player.Hp == 2);

    TickSpells(player, boss, activeSpells, availableSpells); //start player turn
    ASSERT(player.Mana == 223);
    ASSERT(boss.Hp == 14);

    BeforeCast(player, Spell::Shield, activeSpells, availableSpells); //player turn
    ASSERT(player.Mana == 110);

    TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
    ASSERT(player.Mana == 211);
    ASSERT(player.Armor == 7);

    boss.Attack(player); //boss turn
    ASSERT(player.Hp == 1);

    TickSpells(player, boss, activeSpells, availableSpells); //start player turn
    ASSERT(player.Mana == 312);
    ASSERT(boss.Hp == 14);

    BeforeCast(player, Spell::Drain, activeSpells, availableSpells); //player turn
    ASSERT(player.Mana == 239);

    TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
    ASSERT(player.Mana == 340);
    ASSERT(player.Hp == 3);
    ASSERT(boss.Hp == 12);

    boss.Attack(player); //boss turn
    ASSERT(player.Hp == 2);

    TickSpells(player, boss, activeSpells, availableSpells); //start player turn
    ASSERT(player.Mana == 340);
    ASSERT(player.Armor == 7);

    BeforeCast(player, Spell::Poison, activeSpells, availableSpells); //player turn
    ASSERT(player.Mana == 167);

    TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
    ASSERT(boss.Hp == 9);

    boss.Attack(player); //boss turn
    ASSERT(player.Hp == 1);

    TickSpells(player, boss, activeSpells, availableSpells); //start player turn
    ASSERT(player.Armor == 0);
    ASSERT(player.Mana == 167);
    ASSERT(boss.Hp == 6);

    BeforeCast(player, Spell::MagicMissle, activeSpells, availableSpells); //player turn

    TickSpells(player, boss, activeSpells, availableSpells); //start boss turn
    ASSERT(boss.Hp == -1);
    */
    return true;
}

#undef ASSERT