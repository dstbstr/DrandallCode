#include "2017/d3_SpiralMemory.h"

SOLUTION(2017, 3) {

    constexpr u32 FindTarget(u32 target) {
        auto root = static_cast<u32>(Constexpr::Sqrt(target));
        if (root % 2 == 0) {
            root--;
        }
        auto pos = target - (root * root);
        auto legLength = root + 1;
        while (pos > legLength) {
            pos -= legLength;
        }
        return pos;
    }

    constexpr Constexpr::SmallMap<u32, Coord> CreateSpiral(u32 maxValue) {
        Constexpr::SmallMap<u32, Coord> result;
        u32 run = 1;
        s32 delta = 1;
        Coord coord{ 0, 0 };
        result[1] = coord;

        u32 val = 2;
        auto shiftX = [&]() {
            for (u32 x = 0; x < run; x++) {
                coord.X += delta;
                result[val] = coord;
                val++;
            }
        };
        auto shiftY = [&]() {
            for (u32 y = 0; y < run; y++) {
                coord.Y += delta;
                result[val] = coord;
                val++;
            }
        };
        auto turnCorner = [&]() {
            run++;
            delta = -delta;
        };

        while (val <= maxValue) {
            shiftX();
            shiftY();

            turnCorner();

            shiftX();
            shiftY();

            turnCorner();
        }

        return result;
    }

    constexpr Constexpr::SmallMap<Coord, u32> WriteSummingSpiral(const Constexpr::SmallMap<u32, Coord>& spiral) {
        Coord min = { -1000, -1000 };
        Coord max = { 1000, 1000 };

        Constexpr::SmallMap<Coord, u32> result;

        result[spiral.at(1)] = 1;
        for (auto key = 2; key < spiral.size(); key++) {
            auto coord = spiral.at(key);
            auto neighbors = GetAllNeighbors(coord, max, min);
            u32 value = 0;
            for (auto neighbor : neighbors) {
                if(result.contains(neighbor)) {
                    value += result.at(neighbor);
                }
            }
            result[coord] = value;
        }

        return result;
    }

    PART_ONE() {
        u32 target;
        Constexpr::ParseNumber(Line, target);
        return Constexpr::ToString(FindTarget(target));
    }

    PART_TWO() {
        u32 target;
        Constexpr::ParseNumber(Line, target);

        auto spiral = CreateSpiral(1000);
        auto sumSpiral = WriteSummingSpiral(spiral);

        for (auto key = 1; key < 1000; key++) {
            auto coord = spiral[key];
            if (sumSpiral[coord] > target) {
                return Constexpr::ToString(sumSpiral[coord]);
            }
        }

        return "Not Found";
    }

    TESTS() {
        static_assert(FindTarget(12) == 3);
        static_assert(FindTarget(23) == 2);
        static_assert(FindTarget(1024) == 31);
        static_assert(FindTarget(347991) == 480);

        //if (Part2(20) != 23) return false;
        //if (Part2(122) != 133) return false;
        return true;
    }
}