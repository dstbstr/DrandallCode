#include "2018/d11_PowerCell.h"

SOLUTION(2018, 11) {
    constexpr s16 CalculatePower(UCoord pos, u32 serialNumber) {
        s64 x = pos.X;
        s64 y = pos.Y;

        auto rackId = x + 10;
        auto power = (rackId * rackId * y) + (rackId * serialNumber);
        auto hundreds = static_cast<s16>(power % 1000);
        hundreds /= 100;
        return hundreds - 5;
    }

    using Grid = std::array<std::array<s16, 301>, 301>;

    constexpr Grid FillGrid(u32 serialNumber) {
        Grid result;
        for (u16 y = 0; y < result.size(); y++) {
            for (u16 x = 0; x < result[0].size(); x++) {
                result[y][x] = CalculatePower(UCoord{ x, y }, serialNumber);
            }
        }

        return result;
    }

    constexpr s32 CalculatePower(UCoord topLeft, UCoord bottomRight, const Grid & grid) {
        s32 result = 0;

        for (size_t x = topLeft.X; x <= bottomRight.X; x++) {
            for (size_t y = topLeft.Y; y <= bottomRight.Y; y++) {
                result += grid[y][x];
            }
        }

        return result;
    }

    constexpr void GetBest(const Grid & grid, u16 size, UCoord & outCoord, s32 & outBest) {
        outBest = -10000;
        outCoord = { 0, 0 };

        for (u32 x = 0; x < grid[0].size() - size; x++) {
            for (u32 y = 0; y < grid.size() - size; y++) {
                UCoord topLeft = { x, y };
                UCoord bottomRight = { x + (size - 1), y + (size - 1) };
                auto power = CalculatePower(topLeft, bottomRight, grid);
                if (outBest < power) {
                    outBest = power;
                    outCoord = topLeft;
                }
            }
        }
    }

    constexpr Grid GetGrid(const std::vector<std::string>& lines) {
        u32 serialNumber;
        Constexpr::ParseNumber(lines[0], serialNumber);
        return FillGrid(serialNumber);
    }

    PART_ONE() {
        auto grid = GetGrid(lines);
        s32 best;
        UCoord bestCoord;
        GetBest(grid, 3, bestCoord, best);

        return Constexpr::ToString(bestCoord);
    }

    PART_TWO() {
        auto grid = GetGrid(lines);

        s32 overallBest = 0;
        UCoord overallBestCoord{ 0, 0 };
        u32 bestSize = 0;

        for (u16 i = 1; i < 300; i++) {
            s32 best = 0;
            UCoord bestCoord;
            GetBest(grid, i, bestCoord, best);
            if (overallBest < best) {
                overallBest = best;
                overallBestCoord = bestCoord;
                bestSize = i;
            }
            else if (best < 0) {
                break; //not getting any better
            }
        }

        return Constexpr::ToString(Vec3<size_t>{overallBestCoord.X, overallBestCoord.Y, bestSize});
    }

    TESTS() {
        static_assert(CalculatePower({ 3, 5 }, 8) == 4);
        static_assert(CalculatePower({ 122, 79 }, 57) == -5);
        static_assert(CalculatePower({ 217,196 }, 39) == 0);
        
        return true;
    }
}