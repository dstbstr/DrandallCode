#include "2019/d12_Moons.h"

SOLUTION(2019, 12) {
    struct Moon {
        Vec3<s32> Pos;
        Vec3<s32> Velocity;

        constexpr bool operator==(const Moon& other) const {
            return Pos == other.Pos && Velocity == other.Velocity;
        }
    };

    constexpr void ApplyGravity(Moon & lhs, Moon & rhs) {
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

    constexpr void Tick(std::vector<Moon>&moons) {
        for (size_t i = 0; i < moons.size(); i++) {
            for (size_t j = i + 1; j < moons.size(); j++) {
                ApplyGravity(moons[i], moons[j]);
            }
        }

        for (auto& moon : moons) {
            moon.Pos += moon.Velocity;
        }
    }

    constexpr u32 GetEnergy(const Moon & moon) {
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

    constexpr Moon ParseMoon(std::string_view line) {
        auto split = Constexpr::Split(line, ", ");
        Moon moon;
        auto x = Constexpr::Split(split[0], "=")[1];
        auto y = Constexpr::Split(split[1], "=")[1];
        auto z = Constexpr::Split(split[2], "=")[1];
        z = z.substr(0, z.size() - 1);

        Constexpr::ParseNumber(x, moon.Pos.X);
        Constexpr::ParseNumber(y, moon.Pos.Y);
        Constexpr::ParseNumber(z, moon.Pos.Z);

        return moon;
    }

    PART_ONE() {
        auto moons = ParseLines(Lines, ParseMoon);
        for (u32 i = 0; i < 1000; i++) {
            Tick(moons);
        }

        return Constexpr::ToString(std::accumulate(moons.begin(), moons.end(), 0, [](u32 previous, const auto& moon) { return previous + GetEnergy(moon); }));
    }

    PART_TWO() {
        auto moons = ParseLines(Lines, ParseMoon);
        u64 step = 0;

        u64 pX = 0;
        u64 pY = 0;
        u64 pZ = 0;

        while (true) {
            if (pX == 0 && std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.X == 0; })) pX = step;
            if (pY == 0 && std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.Y == 0; })) pY = step;
            if (pZ == 0 && std::all_of(moons.begin(), moons.end(), [](const Moon& moon) { return moon.Velocity.Z == 0; })) pZ = step;

            if (pX > 0 && pY > 0 && pZ > 0) {
                break;
            }

            step++;
            Tick(moons);
        }

        auto lcm = Constexpr::FindLcm<u64>(pX, pY, pZ);
        return Constexpr::ToString(lcm * 2);
    }

    TESTS() {
        return true;
    }
}