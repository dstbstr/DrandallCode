#pragma once

#include "Common.h"

struct Probe {
    Coord Velocity{ 0, 0 };
    Coord Position{ 0, 0 };

    constexpr void Update() {
        Position += Velocity;
        if (Velocity.X > 0) {
            Velocity.X--;
        }
        else if (Velocity.X < 0) {
            Velocity.X++;
        }
        Velocity.Y--;
    }
};

struct Target {
    Coord TopLeft{ 0, 0 };
    Coord BottomRight{ 0, 0 };

    constexpr Target(const std::string& line) {
        auto s1 = Constexpr::Split(line, ": ");
        auto s2 = Constexpr::Split(s1[1], ", ");
        auto s3 = Constexpr::Split(s2[0], "..");
        auto s4 = Constexpr::Split(s2[1], "..");
        s32 x1, x2, y1, y2;
        Constexpr::ParseNumber(s3[0].substr(2), x1);
        Constexpr::ParseNumber(s3[1], x2);
        Constexpr::ParseNumber(s4[0].substr(2), y1);
        Constexpr::ParseNumber(s4[1], y2);

        TopLeft.X = x1;
        BottomRight.X = x2;
        if (x1 > x2) std::swap(TopLeft.X, BottomRight.X);
        
        TopLeft.Y = y1;
        BottomRight.Y = y2;
        if (y1 < y2) std::swap(TopLeft.Y, BottomRight.Y);
    }

    constexpr bool Contains(const Probe& probe) const {
        auto pos = probe.Position;
        return pos.X >= TopLeft.X &&
            pos.X <= BottomRight.X &&
            pos.Y <= TopLeft.Y &&
            pos.Y >= BottomRight.Y;
    }

    constexpr bool HasMissed(const Probe& probe) const {
        auto pos = probe.Position;
        return pos.Y < BottomRight.Y || pos.X > BottomRight.X;
    }
};

constexpr bool TestTargetConstructor() {
    auto target = Target("target area: x=20..30, y=-10..-5");
    if (target.TopLeft != Coord{ 20, -5 }) return false;
    if (target.BottomRight != Coord{ 30, -10 }) return false;
    return true;
}

static_assert(TestTargetConstructor());

constexpr std::vector<s32> FindValidXs(const Target target) {
    std::vector<s32> result;
    for (s32 x = 1; x <= target.BottomRight.X; x++) {
        auto xPos = 0;
        for (s32 step = x; step > 0; step--) {
            xPos += step;
            if (xPos >= target.TopLeft.X && xPos <= target.BottomRight.X) {
                result.push_back(x);
                break;
            }
            else if (xPos >= target.BottomRight.X) break;
        }
    }

    return result;
}

constexpr std::vector<s32> FindValidYs(const Target target) {
    std::vector<s32> result;
    for (s32 y = target.BottomRight.Y; y < 150; y++) {
        auto yPos = 0;
        auto step = y;
        while (true) {
            yPos += step;
            step--;
            if (yPos <= target.TopLeft.Y && yPos >= target.BottomRight.Y) {
                result.push_back(y);
                break;
            }
            else if (yPos <= target.BottomRight.Y) break;
        }
    }
    return result;
}

constexpr auto PartOne(const std::string& line) {
    auto target = Target(line);
    auto ys = FindValidYs(target);

    auto biggest = Constexpr::FindMax(ys);
    return (biggest * (biggest + 1)) / 2;
}

static_assert(PartOne("target area: x=20..30, y=-10..-5") == 45);

constexpr auto PartTwo(const std::string& line) {
    auto target = Target(line);
    auto xs = FindValidXs(target);
    auto ys = FindValidYs(target);

    size_t result = 0;
    for (auto x : xs) {
        for (auto y : ys) {
            Probe probe;
            probe.Velocity = { x, y };
            while (true) {
                if (target.Contains(probe)) {
                    result++;
                    break;
                }
                else if (target.HasMissed(probe)) {
                    break;
                }
                probe.Update();
            }
        }
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    if(PartTwo("target area: x=20..30, y=-10..-5") != 112) return false;
    return true;
}