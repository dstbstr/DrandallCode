#pragma once

#include "2015/d22_BossBattle2.h"

SOLUTION(2015, 22) {
    struct Player { s32 Hp{ 0 }; s32 Mana{ 0 }; s32 Armor{ 0 }; };
    struct Boss { s32 Hp{ 0 }; s32 Atk{ 0 }; };
    constexpr Boss ParseBoss(const auto& lines) {
        Boss result;
        auto hp = Constexpr::Split(lines[0], ": ")[1];
        auto dmg = Constexpr::Split(lines[1], ": ")[1];

        Constexpr::ParseNumber(hp, result.Hp);
        Constexpr::ParseNumber(dmg, result.Atk);
        return result;
    }

    enum struct Spell {
        BossTurn, //hack?
        MagicMissle,
        Drain,
        Shield,
        Poison,
        Recharge
    };

    constexpr size_t GetDuration(Spell spell) {
        switch (spell) {
        case Spell::Shield:
        case Spell::Poison: return 6;
        case Spell::Recharge: return 5;
        default: return 1;
        }
    }
    constexpr s32 GetManaCost(Spell spell) {
        switch (spell) {
        case Spell::MagicMissle: return 53;
        case Spell::Drain: return 73;
        case Spell::Shield: return 113;
        case Spell::Poison: return 173;
        case Spell::Recharge: return 229;
        default: return 0;
        }
    }
    struct State {
        Player Player;
        Boss Boss;
        std::vector<Spell> CastSpells;
    };

    constexpr bool CanCast(const State& state, Spell spell) {
        if (GetManaCost(spell) > state.Player.Mana) return false;
        if (GetDuration(spell) == 1) return true;

        auto offset = std::min(state.CastSpells.size(), GetDuration(spell) - 1);
        return std::find(state.CastSpells.cend() - offset, state.CastSpells.cend(), spell) == state.CastSpells.cend();
    }

    constexpr bool TestCanCast() {
        State state{ {10, 1000, 0}, {14, 8}, {} };
        if (!CanCast(state, Spell::Shield)) return false;
        state.CastSpells.push_back(Spell::BossTurn);
        state.CastSpells.push_back(Spell::Shield); //5

        if (CanCast(state, Spell::Shield)) return false;
        state.CastSpells.push_back(Spell::BossTurn); //4
        state.CastSpells.push_back(Spell::MagicMissle); //3

        if (CanCast(state, Spell::Shield)) return false;
        state.CastSpells.push_back(Spell::BossTurn); //2
        state.CastSpells.push_back(Spell::MagicMissle); //1

        if (CanCast(state, Spell::Shield)) return false;
        state.CastSpells.push_back(Spell::BossTurn); //0

        if (!CanCast(state, Spell::Shield)) return false;

        return true;
    }

    constexpr s32 CalculateMana(const State& state) {
        s32 result = 0;
        for (const auto& spell : state.CastSpells) {
            result += GetManaCost(spell);
        }
        return result;
    }

    constexpr void TickSpell(Spell spell, State& state) {
        switch (spell) {
        case Spell::MagicMissle: state.Boss.Hp -= 4; return;
        case Spell::Drain: state.Player.Hp += 2; state.Boss.Hp -= 2; return;
        case Spell::Shield: state.Player.Armor = 7; return;
        case Spell::Poison: state.Boss.Hp -= 3; return;
        case Spell::Recharge: state.Player.Mana += 101; return;
        }
    }
    constexpr void TickSpells(State& state) {
        state.Player.Armor = 0;
        for (size_t i = 0; i < state.CastSpells.size(); i++) {
            auto delta = state.CastSpells.size() - i;
            auto spell = state.CastSpells[i];
            auto duration = GetDuration(spell);
            if (duration < delta) continue;
            TickSpell(spell, state);
        }
    }

    constexpr bool TestTickSpells() {
        State state{ {10, 0, 0}, {14, 8}, {} };
        
        // Turn 1
        state.CastSpells.push_back(Spell::Recharge);
        TickSpells(state);
        if (state.Player.Mana != 101) return false;
        
        state.CastSpells.push_back(Spell::BossTurn);
        TickSpells(state);
        if (state.Player.Mana != 202) return false;

        // Turn 2
        state.CastSpells.push_back(Spell::Shield);
        TickSpells(state);
        if (state.Player.Mana != 303) return false;
        if (state.Player.Armor != 7) return false;

        state.CastSpells.push_back(Spell::BossTurn);
        TickSpells(state);
        if (state.Player.Mana != 404) return false;
        if (state.Player.Armor != 7) return false;

        // Turn 3
        state.CastSpells.push_back(Spell::Drain);
        TickSpells(state);
        if (state.Player.Mana != 505) return false;
        if (state.Player.Armor != 7) return false;
        if (state.Player.Hp != 12) return false;

        state.CastSpells.push_back(Spell::BossTurn);
        TickSpells(state);
        if (state.Player.Mana != 505) return false;
        if (state.Player.Armor != 7) return false;
        if (state.Player.Hp != 12) return false; //don't invoke drain twice

        // Turn 4
        state.CastSpells.push_back(Spell::Poison);
        TickSpells(state);
        if (state.Player.Armor != 7) return false;
        
        state.CastSpells.push_back(Spell::BossTurn);
        TickSpells(state);
        if (state.Player.Armor != 7) return false;

        // Turn 5
        state.CastSpells.push_back(Spell::MagicMissle);
        TickSpells(state);
        if (state.Player.Armor != 0) return false;
        return true;
    }

    constexpr s32 FindBestMana(const State& initialState, bool hardMode) {
        std::vector<State> q{ initialState };
        std::vector<State> nextQ;
        std::vector<Spell> allSpells = { Spell::MagicMissle, Spell::Drain, Spell::Shield, Spell::Poison, Spell::Recharge };
        s32 bestMana = 99999;

        while (!q.empty()) {
            for (const auto& state : q) {
                auto currentManaCost = CalculateMana(state);
                if (currentManaCost > bestMana) continue;

                State current = state;
                if (hardMode) {
                    current.Player.Hp--;
                    if (current.Player.Hp == 0) continue;
                }
                
                current.CastSpells.push_back(Spell::BossTurn);
                TickSpells(current);
                if (current.Boss.Hp <= 0) {
                    bestMana = std::min(bestMana, currentManaCost);
                    continue;
                }

                for (auto spell : allSpells) {
                    if (!CanCast(current, spell)) continue;
                    State next = current;
                    next.CastSpells.push_back(spell);
                    next.Player.Mana -= GetManaCost(spell);

                    //Boss Turn
                    TickSpells(next);
                    if (next.Boss.Hp <= 0) {
                        bestMana = std::min(bestMana, currentManaCost + GetManaCost(spell));
                        continue;
                    }
                    next.Player.Hp -= std::max(1, next.Boss.Atk - next.Player.Armor);
                    if (next.Player.Hp <= 0) continue;

                    nextQ.push_back(next);
                }
            }
            q.clear();
            std::copy(nextQ.cbegin(), nextQ.cend(), std::back_inserter(q));
            nextQ.clear();
        }

        return bestMana;
    }
    PART_ONE() {
        auto boss = ParseBoss(lines);
        Player player = { 50, 500, 0 };
        State initialState{ player, boss, {} };
        return Constexpr::ToString(FindBestMana(initialState, false));
    }
    PART_TWO() {
        auto boss = ParseBoss(lines);
        Player player = { 50, 500, 0 };
        State initialState{ player, boss, {} };
        return Constexpr::ToString(FindBestMana(initialState, true));
    }
    TESTS() {
        if (!TestCanCast()) return false;
        if(!TestTickSpells()) return false;

        Player player = { 10, 250, 0 };
        Boss boss = { 13, 8 };
        State initialState{ player, boss, {} };
        if (FindBestMana(initialState, false) != 226) return false;

        player = { 10, 250, 0 };
        boss = { 14, 8 };
        initialState = { player, boss, {} };
        if (FindBestMana(initialState, false) != 641) return false;

        return true;
    }
}