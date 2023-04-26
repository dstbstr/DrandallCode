#pragma once

#include "Common.h"
#include "Comp.h"
#include <optional>

enum struct TileType {Empty, Wall, Block, Paddle, Ball};

struct Game {
    std::unordered_map<Coord, TileType> Board;
    s64 Score = 0;
};

std::pair<Coord, s64> Read(std::vector<s64>& instructions, Args& args) {
    static const s64 Consumed = -919;
    args.Output = Consumed;
    u32 readCount = 0;
    s64 x = 0, y = 0, z = 0;
    while (Apply(instructions, args)) {
        if (args.Output != Consumed) {
            switch (readCount) {
            case 0: x = args.Output; break;
            case 1: y = args.Output; break;
            case 2: z = args.Output; break;
            }
            readCount++;
            args.Output = Consumed;
            if (readCount == 3) break;
        }
    }
    return std::make_pair(Coord{ static_cast<s32>(x), static_cast<s32>(y) }, z);
}

constexpr Coord BoardSize{ 41, 24 };
constexpr Coord ScorePos{ -1, 0 };

void UpdateGame(Game& game, std::vector<s64>& instructions, Args& args) {
    while (!NeedsInput(instructions, args)) {
        auto [pos, val] = Read(instructions, args);
        if (pos == ScorePos) {
            game.Score = val;
        }
        else {
            game.Board[pos] = static_cast<TileType>(val);
        }

        RunUntilInteract(instructions, args);
        if (instructions[args.Ip] % 100 == OpCode::Halt) break;
    }
}

auto PartOne(const std::string& line) {
    auto instructions = ParseInstructions(line);
    Args args;
    Game game;
    UpdateGame(game, instructions, args);
    
    return std::count_if(game.Board.begin(), game.Board.end(), [](const auto& kvp) {
        return kvp.second == TileType::Block;
        });
}

#include <iostream>
void PrintGame(const Game& game) {
    for (s32 row = 0; row <= BoardSize.Y; row++) {
        for (s32 col = 0; col <= BoardSize.X; col++) {
            Coord pos = { col, row };
            char icon = ' ';
            if (game.Board.find(pos) != game.Board.end()) {
                switch (game.Board.at(pos)) {
                case TileType::Empty: icon = ' '; break;
                case TileType::Ball: icon = 'B'; break;
                case TileType::Block: icon = '%'; break;
                case TileType::Paddle: icon = '_'; break;
                case TileType::Wall: icon = '#'; break;
                }
            }
            std::cout << icon;
        }
        std::cout << '\n';
    }

    std::cout << "Score: " << game.Score << "\n";
}

Coord FindFirst(const Game& game, TileType targetType) {
    for (const auto& [pos, type] : game.Board) {
        if (type == targetType) {
            return pos;
        }
    }

    return { 0, 0 };
}

bool IsGameOver(const Game& game, const std::vector<s64>& instructions, const Args& args) {
    static const Coord origin = { 0, 0 };
    if (instructions[args.Ip] % 100 == OpCode::Halt) return true;
    return FindFirst(game, TileType::Block) == origin;
}

void MovePaddle(const Game& game, std::vector<s64>& instructions, Args& args) {
    auto ballPos = FindFirst(game, TileType::Ball);
    auto paddlePos = FindFirst(game, TileType::Paddle);

    s64 input = ballPos.X < paddlePos.X ? -1 :
        ballPos.X > paddlePos.X ? 1 : 0;
    args.Inputs.push_back(input);
    Apply(instructions, args);
}

auto PartTwo(const std::string& line) {
    auto instructions = ParseInstructions(line);
    instructions[0] = 2;
    Args args;
    Game game;
    while (true) {
        UpdateGame(game, instructions, args);
        //PrintGame(game);
        if (IsGameOver(game, instructions, args)) {
            break;
        }
        MovePaddle(game, instructions, args);
    }

    return game.Score;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    return true;
}