#include "2017/d20_ParticleSwarm.h"

SOLUTION(2017, 20) {
    struct Particle {
        u32 Index;
        Vec3<s64> Position;
        Vec3<s64> Velocity;
        Vec3<s64> Accel;

        constexpr void Tick() {
            Velocity += Accel;
            Position += Velocity;
        }
    };

    constexpr Particle ParseParticle(const std::string & line, u32 index) {
        auto split = Constexpr::Split(line, " ");
        auto posSplit = Constexpr::Split(split[0], ",");
        auto velSplit = Constexpr::Split(split[1], ",");
        auto accSplit = Constexpr::Split(split[2], ",");

        Particle result;
        result.Index = index;

        Constexpr::ParseNumber(posSplit[0].substr(3), result.Position.X);
        Constexpr::ParseNumber(posSplit[1], result.Position.Y);
        Constexpr::ParseNumber(posSplit[2].substr(0, posSplit[2].find('>')), result.Position.Z);

        Constexpr::ParseNumber(velSplit[0].substr(3), result.Velocity.X);
        Constexpr::ParseNumber(velSplit[1], result.Velocity.Y);
        Constexpr::ParseNumber(velSplit[2].substr(0, velSplit[2].find('>')), result.Velocity.Z);

        Constexpr::ParseNumber(accSplit[0].substr(3), result.Accel.X);
        Constexpr::ParseNumber(accSplit[1], result.Accel.Y);
        Constexpr::ParseNumber(accSplit[2].substr(0, accSplit[2].find('>')), result.Accel.Z);

        return result;
    }

    static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Position == Vec3<s64>{-317, 1413, 1507});
    static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Velocity == Vec3<s64>{19, -102, -108});
    static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Accel == Vec3<s64>{1, -3, -3});
    static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Index == 1);

    constexpr bool Collide(Particle lhs, Particle rhs) {
        return lhs.Position == rhs.Position;
    }

    constexpr void RemoveCollisions(std::vector<Particle>&particles) {
        std::vector<Vec3<s64>> seen;
        std::vector<Vec3<s64>> toRemove;
        for (const auto& particle : particles) {
            if (std::find(seen.cbegin(), seen.cend(), particle.Position) == seen.cend()) {
                seen.push_back(particle.Position);
            }
            else {
                toRemove.push_back(particle.Position);
            }
        }

        auto shouldRemove = [toRemove](const Particle& particle) {
            return std::find(toRemove.cbegin(), toRemove.cend(), particle.Position) != toRemove.cend();
        };

        auto newEnd = std::remove_if(particles.begin(), particles.end(), shouldRemove);
        if (newEnd != particles.end()) {
            particles.erase(newEnd, particles.end());
        }
    }

    auto Part1(const std::vector<std::string>&lines) {
        std::vector<Particle> particles;
        for (u32 index = 0; index < lines.size(); index++) {
            particles.push_back(ParseParticle(lines[index], index));
        }

        for (auto i = 0; i < 10000; i++) {
            for (auto& particle : particles) {
                particle.Tick();
            }
        }

        Vec3<s64> origin{ 0, 0, 0 };
        std::sort(particles.begin(), particles.end(), [origin](const Particle& lhs, const Particle& rhs) {
            return MDistance(lhs.Position, origin) < MDistance(rhs.Position, origin);
            });

        return particles[0].Index;
    }

    auto Part2(const std::vector<std::string>&lines) {
        std::vector<Particle> particles;
        for (u32 index = 0; index < lines.size(); index++) {
            particles.push_back(ParseParticle(lines[index], index));
        }

        for (auto i = 0; i < 10000; i++) {
            for (auto& particle : particles) {
                particle.Tick();
            }
            RemoveCollisions(particles);
        }

        return particles.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "p=<-6,0,0>, v=<3,0,0>, a=<0,0,0>",
            "p=<-4,0,0>, v=<2,0,0>, a=<0,0,0>",
            "p=<-2,0,0>, v=<1,0,0>, a=<0,0,0>",
            "p=<3,0,0>, v=<-1,0,0>, a=<0,0,0>"
        };

        if (Part2(lines) != 1) return false;

        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}