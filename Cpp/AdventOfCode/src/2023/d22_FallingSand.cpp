#include "2023/d22_FallingSand.h"

SOLUTION(2023, 22) {
    struct Brick {
        Vec3<s32> Start, End;
    };

    constexpr Brick ParseBrick(const std::string& line) {
        auto s = Constexpr::Split(line, "~");
        return { Vec3<s32>(s[0]), Vec3<s32>(s[1]) };
    }

    constexpr bool OverlapsXy(const Brick& lhs, const Brick& rhs) {
        bool overlapX = false, overlapY = false;
        for (auto x = lhs.Start.X; x <= lhs.End.X; x++) {
            overlapX |= (rhs.Start.X <= x && x <= rhs.End.X);
        }
        for (auto y = lhs.Start.Y; y <= lhs.End.Y; y++) {
            overlapY |= (rhs.Start.Y <= y && y <= rhs.End.Y);
        }
        return overlapX && overlapY;
    }

    constexpr size_t CountSupports(const std::vector<Brick>& bricks, size_t index) {
        size_t result = 0;
        auto lhs = bricks[index];
        for (auto i = index; i > 0; --i) {
            auto rhs = bricks[i - 1];
            result += (OverlapsXy(lhs, rhs) && rhs.End.Z == lhs.Start.Z - 1);
        }
        return result;
    }

    constexpr void ForEachOverlap(const std::vector<Brick>& bricks, size_t index, auto Func) {
        size_t i = index;
        auto targetZ = bricks[index].End.Z + 1;
        for (; i < bricks.size(); i++) {
            if (bricks[i].Start.Z == targetZ) {
                break;
            }
        }
        while (i < bricks.size() && bricks[i].Start.Z == targetZ) {
            if (OverlapsXy(bricks[index], bricks[i])) {
                Func(i);
            }
            i++;
        }
    }

    constexpr bool CanDestroy(const std::vector<Brick>& bricks, size_t index) {
        bool result = true;
        ForEachOverlap(bricks, index, [&](size_t i) {
            if (CountSupports(bricks, i) <= 1) result = false;
            });
        return result;
    }

    constexpr void ChainRec(const std::vector<Brick>& bricks, Constexpr::Queue<size_t>& q, std::vector<bool>& fallen, size_t& outResult) {
        if (q.is_empty()) return;
        auto index = q.front(); q.pop();
        if (fallen[index]) return ChainRec(bricks, q, fallen, outResult);
        
        auto supported = false;
        const auto& brick = bricks[index];
        for (auto i = index; i > 0; --i) {
            const auto& other = bricks[i - 1];
            if (!fallen[i - 1] &&
                other.End.Z == brick.Start.Z - 1 &&
                OverlapsXy(brick, other)) {
                supported = true;
                break;
            }
        }
        if (supported) return ChainRec(bricks, q, fallen, outResult);
        fallen[index] = true;
        outResult++;

        ForEachOverlap(bricks, index, [&](size_t i) {
            q.push(i);
            });
        ChainRec(bricks, q, fallen, outResult);
    }

    constexpr size_t CountChainReaction(const std::vector<Brick>& bricks, size_t index) {
        size_t chain = 0;
        Constexpr::Queue<size_t> q;
        std::vector<bool> fallen(bricks.size(), false);
        fallen[index] = true;

        ForEachOverlap(bricks, index, [&](size_t i) {
            q.push(i);
            });

        ChainRec(bricks, q, fallen, chain);
        return chain;
    }

    constexpr void Compact(std::vector<Brick>& bricks) {
        std::sort(bricks.begin(), bricks.end(), [](const Brick& lhs, const Brick& rhs) {
            return lhs.End.Z < rhs.End.Z;
            });

        for (auto i = 0; i < bricks.size(); i++) {
            auto& brick = bricks[i];

            s32 maxZ = 0;
            for (auto j = 0; j < i; j++) {
                if (OverlapsXy(brick, bricks[j])) {
                    maxZ = std::max(maxZ, bricks[j].End.Z); 
                }
            }
            auto zHeight = brick.End.Z - brick.Start.Z;
            brick.Start.Z = maxZ + 1;
            brick.End.Z = brick.Start.Z + zHeight;
        }
        std::sort(bricks.begin(), bricks.end(), [](const Brick& lhs, const Brick& rhs) {
            return lhs.Start.Z < rhs.Start.Z;
            });
    }

    PART_ONE() {
        auto bricks = ParseLines(lines, ParseBrick);
        Compact(bricks);

        size_t result = 0;
        for (auto i = 0; i < bricks.size(); i++) {
            result += CanDestroy(bricks, i);
        }
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        size_t result = 0;
        auto bricks = ParseLines(lines, ParseBrick);
        Compact(bricks);

        for (auto i = 0; i < bricks.size(); i++) {
            result += CountChainReaction(bricks, i);
        }

        return Constexpr::ToString(result);
    }

    TESTS() {
        std::vector<std::string> example = {
            "1,0,1~1,2,1",
            "0,0,2~2,0,2",
            "0,2,3~2,2,3",
            "0,0,4~0,2,4",
            "2,0,5~2,2,5",
            "0,1,6~2,1,6",
            "1,1,8~1,1,9"
        };

        if (PartOne(example) != "5") return false;

        return true;
    }
}