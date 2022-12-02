#pragma once

#include "Common.h"

//A - Rock
//B - Paper
//C - Scissors

enum struct Play : u32 { Rock = 1, Paper = 2, Scissors = 3 };
enum struct Outcome : u32 {Lose = 0, Draw = 3, Win = 6 };

constexpr Outcome RunGame(Play player, Play opponent) {
    if(player == opponent) return Outcome::Draw;
    if(player == Play::Rock) return opponent == Play::Paper ? Outcome::Lose : Outcome::Win;
    if(player == Play::Paper) return opponent == Play::Scissors ? Outcome::Lose : Outcome::Win;
    if(player == Play::Scissors) return opponent == Play::Rock ? Outcome::Lose : Outcome::Win;
    return Outcome::Lose; //should never reach this
}

constexpr u32 GetPoints(Play player, Play opponent) {
    return static_cast<u32>(player) + static_cast<u32>(RunGame(player, opponent));
}

static_assert(GetPoints(Play::Paper, Play::Rock) == 8);
static_assert(GetPoints(Play::Rock, Play::Paper) == 1);
static_assert(GetPoints(Play::Scissors, Play::Scissors) == 6);

constexpr Play WhatToPlay(Play opponent, Outcome target) {
    if(target == Outcome::Draw) {
        return opponent;
    }
    if(target == Outcome::Win) {
        switch(opponent) {
            case Play::Rock: return Play::Paper;
            case Play::Paper: return Play::Scissors;
            case Play::Scissors: return Play::Rock;
        }
    } else {
        switch(opponent) {
            case Play::Rock: return Play::Scissors;
            case Play::Paper: return Play::Rock;
            case Play::Scissors: return Play::Paper;
        }
    }

    return opponent; // should never happen
}

//X = Rock
//Y = Paper
//Z = Scissors
constexpr void ParseInput(const std::string& line, Play& opponent, Play& player) {
    auto op = line[0];
    auto pl = line[2];
    switch(op) {
        case 'A': opponent = Play::Rock; break;
        case 'B': opponent = Play::Paper; break;
        case 'C': opponent = Play::Scissors; break;
    }
    switch(pl) {
        case 'X': player = Play::Rock; break;
        case 'Y': player = Play::Paper; break;
        case 'Z': player = Play::Scissors; break;
    }
}

//X - Lose
//Y - Draw
//Z - Win
constexpr void ParseUpdatedInput(const std::string& line, Play& opponent, Play& player) {
    auto op = line[0];
    auto target = line[2];
    switch(op) {
        case 'A': opponent = Play::Rock; break;
        case 'B': opponent = Play::Paper; break;
        case 'C': opponent = Play::Scissors; break;
    }

    Outcome targetOutcome;
    switch(target) {
        case 'X': targetOutcome = Outcome::Lose; break;
        case 'Y': targetOutcome = Outcome::Draw; break;
        case 'Z': targetOutcome = Outcome::Win; break;
        default: targetOutcome = Outcome::Win; break; //should never happen
    }

    player = WhatToPlay(opponent, targetOutcome);
}

constexpr u32 CalculateTotal(const std::vector<std::string>& lines) {
    u32 result = 0;
    for(auto line: lines) {
        Play opponent;
        Play player;
        ParseInput(line, opponent, player);
        result += GetPoints(player, opponent);
    }

    return result;
}

constexpr u32 CalculateUpdatedTotal(const std::vector<std::string>& lines) {
    u32 result = 0;
    for(auto line: lines) {
        Play opponent;
        Play player;
        ParseUpdatedInput(line, opponent, player);
        result += GetPoints(player, opponent);
    }

    return result;
}