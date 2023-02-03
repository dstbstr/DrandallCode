#pragma once

#include "Common.h"

struct Moon {
    Vec3<s32> Pos;
    Vec3<s32> Velocity;

    constexpr bool operator==(const Moon& other) const {
        return Pos == other.Pos && Velocity == other.Velocity;
    }
};

constexpr void ApplyGravity(Moon& lhs, Moon& rhs) {
    if (lhs.Pos.X < rhs.Pos.X) {
        lhs.Velocity.X++;
        rhs.Velocity.X--;
    }
    else if (lhs.Pos.X > rhs.Pos.X) {
        lhs.Velocity.X--;
        rhs.Velocity.X++;
    }

    if (lhs.Pos.Y < rhs.Pos.Y) {
        lhs.Velocity.Y++;
        rhs.Velocity.Y--;
    }
    else if (lhs.Pos.Y > rhs.Pos.Y) {
        lhs.Velocity.Y--;
        rhs.Velocity.Y++;
    }

    if (lhs.Pos.Z < rhs.Pos.Z) {
        lhs.Velocity.Z++;
        rhs.Velocity.Z--;
    }
    else if (lhs.Pos.Z > rhs.Pos.Z) {
        lhs.Velocity.Z--;
        rhs.Velocity.Z++;
    }
}

constexpr void Tick(std::vector<Moon>& moons) {
    for (size_t i = 0; i < moons.size(); i++) {
        for (size_t j = i + 1; j < moons.size(); j++) {
            ApplyGravity(moons[i], moons[j]);
        }
    }

    for (auto& moon : moons) {
        moon.Pos += moon.Velocity;
    }
}

constexpr u32 GetEnergy(const Moon& moon) {
    u32 potential = 0;
    potential += Constexpr::Abs(moon.Pos.X);
    potential += Constexpr::Abs(moon.Pos.Y);
    potential += Constexpr::Abs(moon.Pos.Z);

    u32 kinetic = 0;
    kinetic += Constexpr::Abs(moon.Velocity.X);
    kinetic += Constexpr::Abs(moon.Velocity.Y);
    kinetic += Constexpr::Abs(moon.Velocity.Z);

    return potential * kinetic;
}

constexpr std::vector<Moon> ParseLines(const std::vector<std::string>& lines) {
    std::vector<Moon> result;
    for (const auto& line : lines) {
        auto split = Constexpr::Split(line, ", ");
        Moon moon;
        auto x = Constexpr::Split(split[0], "=")[1];
        auto y = Constexpr::Split(split[1], "=")[1];
        auto z = Constexpr::Split(split[2], "=")[1];
        z = z.substr(0, z.size() - 1);

        Constexpr::ParseNumber(x, moon.Pos.X);
        Constexpr::ParseNumber(y, moon.Pos.Y);
        Constexpr::ParseNumber(z, moon.Pos.Z);
        
        result.push_back(moon);
    }
    return result;
}

std::string ToString(const std::vector<Moon>& moons) {
    return StrUtil::Format("%d,%d,%d %d,%d,%d\n%d,%d,%d %d,%d,%d\n%d,%d,%d %d,%d,%d\n%d,%d,%d %d,%d,%d\n", 
        moons[0].Pos.X, moons[0].Pos.Y, moons[0].Pos.Z, moons[0].Velocity.X, moons[0].Velocity.Y, moons[0].Velocity.Z,
        moons[1].Pos.X, moons[1].Pos.Y, moons[1].Pos.Z, moons[1].Velocity.X, moons[1].Velocity.Y, moons[1].Velocity.Z,
        moons[2].Pos.X, moons[2].Pos.Y, moons[2].Pos.Z, moons[2].Velocity.X, moons[2].Velocity.Y, moons[2].Velocity.Z,
        moons[3].Pos.X, moons[3].Pos.Y, moons[3].Pos.Z, moons[3].Velocity.X, moons[3].Velocity.Y, moons[3].Velocity.Z
    );
}

auto PartOne(const std::vector<std::string>& lines, u32 steps) {
    auto moons = ParseLines(lines);
    for (u32 i = 0; i < steps; i++) {
        Tick(moons);
    }

    u32 energy = 0;
    for (const auto& moon : moons) {
        energy += GetEnergy(moon);
    }

    return energy;
}

#include <iostream>

auto PartTwo(const std::vector<std::string>& lines) {
    auto moons = ParseLines(lines);
    u64 step = 0;

    u64 pX = 0;
    u64 pY = 0;
    u64 pZ = 0;
    while (true) {
        if (pX == 0) {
            if (std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.X == 0; })) {
                pX = step;
            }
        }
        if (pY == 0) {
            if( std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.Y == 0; }) ) {
                pY = step;
            }
        }
        if (pZ == 0) {
            if (std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.Z == 0; })) {
                pZ = step;
            }
        }

        if (pX > 0 && pY > 0 && pZ > 0) {
            break;
        }

        step++;
        Tick(moons);
    }

    auto lcm = Constexpr::FindLcm<u64>(pX, pY, pZ);
    return lcm * 2;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines, 1000));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "<x=-1, y=0, z=2>",
        "<x=2, y=-10, z=-7>",
        "<x=4, y=-8, z=8>",
        "<x=3, y=5, z=-1>"
    };

    if (PartOne(lines, 10) != 179) return false;
    //if (PartTwo(lines) != 2772) return false;

    lines = {
        "<x=-8, y=-10, z=0>",
        "<x=5, y=5, z=10>",
        "<x=2, y=-7, z=3>",
        "<x=9, y=-8, z=-3>"
    };

    if (PartOne(lines, 100) != 1940) return false;
    if (PartTwo(lines) != 4686774924) return false;

    return true;
}