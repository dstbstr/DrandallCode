#include "2017/d19_Routing.h"
#include "Facing.h"

SOLUTION(2017, 19) {
    using Grid = std::vector<std::string>;

    constexpr RowCol FindStartPos(const std::string & firstLine) {
        for (size_t col = 0; col < firstLine.size(); col++) {
            if (firstLine[col] != ' ') {
                return RowCol{ 0, col };
            }
        }

        return { 0, 0 };
    }

    constexpr bool CanMove(RowCol & pos, const Grid & grid) {
        if (pos.Row >= grid.size() || pos.Col >= grid[0].size()) {
            return false;
        }
        return grid[pos.Row][pos.Col] != ' ';
    }

    constexpr std::array<RowCol, 4> GetNeighbors(RowCol pos) {
        RowCol upPos = { pos.Row - 1, pos.Col };
        RowCol downPos = { pos.Row + 1, pos.Col };
        RowCol leftPos = { pos.Row, pos.Col - 1 };
        RowCol rightPos = { pos.Row, pos.Col + 1 };

        std::array<RowCol, 4> result;
        result[Up] = upPos;
        result[Down] = downPos;
        result[Left] = leftPos;
        result[Right] = rightPos;

        return result;
    }

    constexpr void Turn(RowCol& pos, Facing& facing, const Grid& grid) {
        auto neighbors = GetNeighbors(pos);

        auto turn = [&](Facing first, Facing second, Facing third) {
            if (CanMove(neighbors[first], grid)) {
                facing = first;
            }
            else if (CanMove(neighbors[second], grid)) {
                facing = second;
            }
            else if (CanMove(neighbors[third], grid)) {
                facing = third;
            }
        };

        switch (facing) {
        case Up: turn(Up, Left, Right); break;
        case Down: turn(Down, Left, Right); break;
        case Left: turn(Left, Up, Down); break;
        case Right: turn(Right, Up, Down); break;
        }
    }

    constexpr bool Move(RowCol & pos, Facing& facing, const Grid & grid) {
        auto neighbors = GetNeighbors(pos);

        auto tryMove = [&](RowCol dest) {
            if (CanMove(dest, grid)) {
                pos = dest;
                return true;
            }
            else {
                return false;
            }
        };

        bool moved = false;
        switch (facing) {
        case Up: moved = tryMove(neighbors[Up]); break;
        case Down: moved = tryMove(neighbors[Down]); break;
        case Left: moved = tryMove(neighbors[Left]); break;
        case Right: moved = tryMove(neighbors[Right]); break;
        default: break;
        }

        if (!moved) {
            return false;
        }

        if (grid[pos.Row][pos.Col] == '+') {
            Turn(pos, facing, grid);
        }

        return true;
    }

    PART_ONE() {
        auto pos = FindStartPos(lines[0]);
        Facing facing = Down;
        std::string result;
        char currentSquare;

        while (Move(pos, facing, lines)) {
            currentSquare = lines[pos.Row][pos.Col];
            if (currentSquare >= 'A' && currentSquare <= 'Z') {
                result += currentSquare;
            }
        }

        return result;
    }

    PART_TWO() {
        auto pos = FindStartPos(lines[0]);
        Facing facing = Down;
        u32 steps = 1;
        while (Move(pos, facing, lines)) {
            steps++;
        }

        return Constexpr::ToString(steps);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "     |          ",
            "     |  +--+    ",
            "     A  |  C    ",
            " F---|----E|--+ ",
            "     |  |  |  D ",
            "     +B-+  +--+ "
        };

        if (PartOne(lines) != "ABCDEF") return false;
        if (PartTwo(lines) != "38") return false;

        return true;
    }
}