#include "2019/d13_Comp6.h"
#include "2019/Comp.h"
#include <optional>

SOLUTION(2019, 13) {
    enum struct TileType { Empty, Wall, Block, Paddle, Ball };

    struct Game {
        Constexpr::SmallMap<Coord, TileType> Board;
        s64 Score = 0;
    };

    constexpr std::pair<Coord, s64> Read(std::vector<s64>&instructions, Args & args) {
        args.Output = Unset;
        u32 readCount = 0;
        s64 x = 0, y = 0, z = 0;
        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                switch (readCount) {
                case 0: x = args.Output; break;
                case 1: y = args.Output; break;
                case 2: z = args.Output; break;
                }
                readCount++;
                args.Output = Unset;
                if (readCount == 3) break;
            }
        }
        return std::make_pair(Coord{ static_cast<s32>(x), static_cast<s32>(y) }, z);
    }

    constexpr Coord BoardSize{ 41, 24 };
    constexpr Coord ScorePos{ -1, 0 };

    constexpr void UpdateGame(Game & game, std::vector<s64>&instructions, Args & args) {
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

    constexpr Coord origin = { 0, 0 };
    Coord FindFirst(const Game & game, TileType targetType) {
        for (const auto& [pos, type] : game.Board) {
            if (type == targetType) {
                return pos;
            }
        }

        return origin;
    }

    constexpr bool IsGameOver(const Game & game, const std::vector<s64>&instructions, const Args & args) {
        if (instructions[args.Ip] % 100 == OpCode::Halt) return true;
        return FindFirst(game, TileType::Block) == origin;
    }

    void MovePaddle(const Game & game, std::vector<s64>&instructions, Args & args) {
        auto ballPos = FindFirst(game, TileType::Ball);
        auto paddlePos = FindFirst(game, TileType::Paddle);

        s64 input = ballPos.X < paddlePos.X ? -1 :
            ballPos.X > paddlePos.X ? 1 : 0;
        args.Inputs.push_back(input);
        Apply(instructions, args);
    }


    PART_ONE() {
        auto instructions = ParseInstructions(Line);
        Args args;
        Game game;
        UpdateGame(game, instructions, args);

        
        auto blackTiles = std::count_if(game.Board.begin(), game.Board.end(), [](const auto& kvp) {
            return kvp.second == TileType::Block;
            });

        return Constexpr::ToString(blackTiles);
    }

    PART_TWO() {
        auto instructions = ParseInstructions(Line);
        instructions[0] = 2;
        Args args;
        Game game;
        while (true) {
            UpdateGame(game, instructions, args);
            if (IsGameOver(game, instructions, args)) {
                break;
            }
            MovePaddle(game, instructions, args);
        }

        return Constexpr::ToString(game.Score);
    }

    TESTS() {
        return true;
    }
}