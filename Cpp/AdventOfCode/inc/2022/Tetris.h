#pragma once

#include "Common.h"
#include <iostream>

struct Piece {
    std::array<std::array<bool, 4>, 4> Stones{};
    Coord TopLeft{};
};

//using Grid = std::array<u32, 7>;
using Grid = std::array<std::array<bool, 7>, 1000>;

constexpr std::vector<Piece> GetPieces() {
    Piece horizontalLine{};
    Piece cross{};
    Piece lShape{};
    Piece verticalLine{};
    Piece square{};

    horizontalLine.Stones[3][0] = true;
    horizontalLine.Stones[3][1] = true;
    horizontalLine.Stones[3][2] = true;
    horizontalLine.Stones[3][3] = true;

    cross.Stones[1][1] = true;
    cross.Stones[2][0] = true;
    cross.Stones[2][1] = true;
    cross.Stones[2][2] = true;
    cross.Stones[3][1] = true;

    lShape.Stones[1][2] = true;
    lShape.Stones[2][2] = true;
    lShape.Stones[3][0] = true;
    lShape.Stones[3][1] = true;
    lShape.Stones[3][2] = true;

    verticalLine.Stones[0][0] = true;
    verticalLine.Stones[1][0] = true;
    verticalLine.Stones[2][0] = true;
    verticalLine.Stones[3][0] = true;

    square.Stones[2][0] = true;
    square.Stones[2][1] = true;
    square.Stones[3][0] = true;
    square.Stones[3][1] = true;

    return { horizontalLine, cross, lShape, verticalLine, square };
}

//translate from grid to piece space
constexpr Coord ToPieceSpace(const Piece& piece, Coord gridSpace) {
    auto x = gridSpace.X - piece.TopLeft.X;
    auto y = gridSpace.Y - piece.TopLeft.Y;
    return { x, y };
}

constexpr Coord ToGridSpace(const Piece& piece, Coord pieceSpace) {
    auto x = pieceSpace.X + piece.TopLeft.X;
    auto y = pieceSpace.Y + piece.TopLeft.Y;

    return { x, y };
}

void PrintGrid(const Grid& grid, const Piece& piece, s32 maxHeight) {
    for (auto row = 0; row < maxHeight + 7; row++) {
        for (auto col = 0; col < 7; col++) {
            if (grid[row][col]) {
                std::cout << '#';
            }
            else {
                //print active piece with @
            }
            std::cout << (grid[row][col] ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::vector<bool> ParseInput(const std::string& line) {
    std::vector<bool> result;
    for (auto c : line) {
        result.push_back( c == '<');
    }

    return result;
}

constexpr void StopPiece(const Piece& piece, Grid& grid, s32& maxHeight) {
    bool heightSet = false;

    for (auto row = 0; row < 4; row++) {
        for (auto col = 0; col < 4; col++) {
            if (piece.Stones[row][col]) {
                if (!heightSet) {
                    maxHeight = piece.TopLeft.Y + 3 - row;
                    heightSet = true;
                }
                grid[piece.TopLeft.Y + row][piece.TopLeft.X + col] = true;
            }
        }
    }
}

constexpr bool TryMoveDown(Piece& piece, Grid& grid, s32& maxHeight) {
    for (s32 row = 3; row >= 0; row--) {
        for (s32 col = 0; col < 4; col++) {
            if (piece.Stones[row][col]) {
                auto yValue = row + piece.TopLeft.Y;
                if (yValue == 1) {
                    StopPiece(piece, grid, maxHeight);
                    return false;
                }
                if (grid[yValue - 1][col]) {
                    StopPiece(piece, grid, maxHeight);
                    return false;
                }
            }
        }
    }

    piece.TopLeft.Y--;
    return true;
}

constexpr void MoveSideways(Piece& piece, const Grid& grid, bool moveLeft) {
    if (moveLeft) {
        for (s32 row = 0; row < 4; row++) {
            for (s32 col = 0; col < 4; col++) {
                if (piece.Stones[row][col]) {
                    auto x = piece.TopLeft.X + col;
                    auto y = piece.TopLeft.Y + (3 - row);

                    if (x == 0) return;
                    if (grid[y][x - 1]) return;
                    break;
                }
            }
        }
        // No obstructions
        piece.TopLeft.X--;
    }
    else {
        for (auto row = 0; row < 4; row++) {
            for (s32 col = 3; col >= 0; col--) {
                if (piece.Stones[row][col]) {
                    auto x = piece.TopLeft.X + col;
                    auto y = piece.TopLeft.Y + (3 - row);
                    if (x > 5) return;
                    if (grid[y][x + 1]) return;
                    break;
                }
            }
        }
        //No obstructions
        piece.TopLeft.X++;
    }
}

std::string Run(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        (void)line;
    }
    return "";
}

bool RunTests() {
    std::string line = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
    auto directions = ParseInput(line);
    auto pieces = GetPieces();
    Grid grid{};

    size_t horizontalIndex = 0;
    size_t pieceIndex = 0;
    s32 maxHeight = 0;
    u32 pieceCount = 0;
    
    while (pieceCount < 2023) {
        pieceCount++;
        auto piece = pieces[pieceIndex];
        piece.TopLeft = { 2, maxHeight + 8 };
        while (true) {
            PrintGrid(grid, maxHeight);
            MoveSideways(piece, grid, directions[horizontalIndex++]);
            if (!TryMoveDown(piece, grid, maxHeight)) break;
        }
        
    }
    return true;
}