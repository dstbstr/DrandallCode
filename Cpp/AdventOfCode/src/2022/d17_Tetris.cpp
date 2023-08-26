#include "2022/d17_Tetris.h"

SOLUTION(2022, 17) {

    struct Piece {
        std::array<std::array<bool, 4>, 4> Stones{};
        Coord TopLeft{};
    };

    using Grid = std::array<std::array<bool, 7>, 10000>;

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

    constexpr Coord ToPieceSpace(const Piece & piece, Coord gridSpace) {
        auto x = gridSpace.X - piece.TopLeft.X;
        auto y = piece.TopLeft.Y - gridSpace.Y;
        return { x, y };
    }

    constexpr Coord ToGridSpace(const Piece & piece, Coord pieceSpace) {
        auto x = pieceSpace.X + piece.TopLeft.X;
        auto y = piece.TopLeft.Y - pieceSpace.Y;

        return { x, y };
    }

    constexpr std::vector<bool> ParseInput(std::string_view line) {
        std::vector<bool> result;
        for (auto c : line) {
            result.push_back(c == '<');
        }

        return result;
    }

    constexpr void StopPiece(const Piece & piece, Grid & grid, s64 & maxHeight) {
        bool heightSet = false;

        for (auto row = 0; row < 4; row++) {
            for (auto col = 0; col < 4; col++) {
                if (piece.Stones[row][col]) {
                    auto gridSpace = ToGridSpace(piece, { col, row });
                    if (!heightSet) {
                        maxHeight = std::max(maxHeight, gridSpace.Y);
                        heightSet = true;
                    }
                    grid[gridSpace.Y][gridSpace.X] = true;
                }
            }
        }
    }

    constexpr bool TryMoveDown(Piece & piece, Grid & grid, s64 & maxHeight) {
        for (s32 row = 3; row >= 0; row--) {
            for (s32 col = 0; col < 4; col++) {
                if (piece.Stones[row][col]) {
                    auto gridSpace = ToGridSpace(piece, { col, row });
                    if (gridSpace.Y == 1) {
                        StopPiece(piece, grid, maxHeight);
                        return false;
                    }
                    if (grid[gridSpace.Y - 1][gridSpace.X]) {
                        StopPiece(piece, grid, maxHeight);
                        return false;
                    }
                }
            }
        }

        piece.TopLeft.Y--;
        return true;
    }

    constexpr void MoveSideways(Piece & piece, const Grid & grid, bool moveLeft) {
        if (moveLeft) {
            for (s32 row = 0; row < 4; row++) {
                for (s32 col = 0; col < 4; col++) {
                    if (piece.Stones[row][col]) {
                        auto gridSpace = ToGridSpace(piece, { col, row });

                        if (gridSpace.X == 0) return;
                        if (grid[gridSpace.Y][gridSpace.X - 1]) return;
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
                        auto gridSpace = ToGridSpace(piece, { col, row });
                        if (gridSpace.X > 5) return;
                        if (grid[gridSpace.Y][gridSpace.X + 1]) return;
                        break;
                    }
                }
            }
            //No obstructions
            piece.TopLeft.X++;
        }
    }

    constexpr u32 FindPeriod(const std::vector<bool>&directions, const std::vector<Piece>&pieces, size_t & offset, size_t & prePeriod, std::vector<size_t>&outAmounts) {
        //Grid grid{};
        auto* grid = new Grid();

        size_t horizontalIndex = 0;
        size_t pieceIndex = 0;
        u64 pieceCount = 0;
        s64 top = 0;
        u32 period = 0;

        Constexpr::BigSet<std::string> seen{};
        std::vector<size_t> recentDeltas{};

        std::string mostRecentAdd = "";
        while (true) {
            offset++;
            auto oldTop = top;
            pieceCount++;
            auto piece = pieces[(pieceIndex++) % pieces.size()];
            piece.TopLeft = { 2, top + 7 };
            while (true) {
                MoveSideways(piece, *grid, directions[(horizontalIndex++) % directions.size()]);
                if (!TryMoveDown(piece, *grid, top)) break;
            }
            auto deltaY = std::max(0ll, top - oldTop);
            prePeriod += deltaY;

            if (recentDeltas.size() == 20) {
                recentDeltas.erase(recentDeltas.begin());
            }
            recentDeltas.push_back(deltaY);
            auto checkStr = Constexpr::JoinVec(',', recentDeltas);
            if (seen.insert(checkStr)) {
                mostRecentAdd = checkStr;
                outAmounts.clear();
            }
            else {
                outAmounts.push_back(deltaY);
                if (checkStr == mostRecentAdd) break;
            }
        }
        period = static_cast<u32>(outAmounts.size());
        offset -= period;

        delete grid;
        return period;
    }

    constexpr u64 FindTowerHeight(std::string_view line, u64 maxPieces) {
        auto directions = ParseInput(line);
        auto pieces = GetPieces();
        std::vector<size_t> increaseAmounts;
        size_t offset = 0;
        size_t prePeriod = 0;

        auto period = FindPeriod(directions, pieces, offset, prePeriod, increaseAmounts);

        //u64 amountPerPeriod = Constexpr::Sum(increaseAmounts);
        u64 amountPerPeriod = std::accumulate(increaseAmounts.begin(), increaseAmounts.end(), 0ull);
        bool firstTime = true;
        if (firstTime) {
            prePeriod -= static_cast<u32>(amountPerPeriod); //this amount contains 1 full cycle
            firstTime = false;
        }

        auto remainingPieces = maxPieces - offset;
        auto periods = remainingPieces / period;
        u64 result = prePeriod + (periods * amountPerPeriod);

        for (auto i = 0; i < remainingPieces % period; i++) {
            result += increaseAmounts[i];
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(FindTowerHeight(Line, 2022));
    }

    PART_TWO() {
        return Constexpr::ToString(FindTowerHeight(Line, 1'000'000'000'000));
    }

    TESTS() {
        std::string line = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";

        if (FindTowerHeight(line, 2022) != 3068) return false;
        if (FindTowerHeight(line, 1'000'000'000'000) != 1'514'285'714'288) return false;
        return true;
    }
}