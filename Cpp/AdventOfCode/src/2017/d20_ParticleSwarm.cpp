#include "2017/d20_ParticleSwarm.h"

SOLUTION(2017, 20) {
    struct Particle {
        size_t Index{ 0 };
        Vec3<s64> Position{0, 0, 0};
        Vec3<s64> Velocity{0, 0, 0};
        Vec3<s64> Accel{0, 0, 0};

        constexpr void Tick() {
            Velocity += Accel;
            Position += Velocity;
        }
    };

    constexpr Particle ParseParticle(std::string_view line, size_t index) {
        auto s = Constexpr::Split(line, ", ");
        Particle result;
        result.Index = index;
        result.Position = Vec3<s64>(s[0].substr(3, s[0].size() - 4));
        result.Velocity = Vec3<s64>(s[1].substr(3, s[1].size() - 4));
        result.Accel = Vec3<s64>(s[2].substr(3, s[2].size() - 4));
        return result;
    }

    constexpr bool Collide(Particle lhs, Particle rhs) {
        return lhs.Position == rhs.Position;
    }

    constexpr void RemoveCollisions(std::vector<Particle>& particles) {
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

    PART_ONE() {
        auto particles = ParseLinesWithIndex(Lines, ParseParticle);
        //Over 10k iterations, the Acceleration will be the most important factor
        std::sort(particles.begin(), particles.end(), [](const Particle& lhs, const Particle& rhs) {
            return MDistance(lhs.Accel) < MDistance(rhs.Accel);
            });
        return Constexpr::ToString(particles[0].Index);
    }

    PART_TWO() {
        auto particles = ParseLinesWithIndex(Lines, ParseParticle);

        for (auto i = 0; i < 100; i++) {
            for (auto& particle : particles) {
                particle.Tick();
            }
            RemoveCollisions(particles);
        }

        return Constexpr::ToString(particles.size());
    }

    TESTS() {
        static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Position == Vec3<s64>{-317, 1413, 1507});
        static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Velocity == Vec3<s64>{19, -102, -108});
        static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Accel == Vec3<s64>{1, -3, -3});
        static_assert(ParseParticle("p=<-317,1413,1507>, v=<19,-102,-108>, a=<1,-3,-3>", 1).Index == 1);
        
        //std::vector<std::string> lines = {
        //   "p=<-6,0,0>, v=<3,0,0>, a=<0,0,0>",
        //   "p=<-4,0,0>, v=<2,0,0>, a=<0,0,0>",
        //   "p=<-2,0,0>, v=<1,0,0>, a=<0,0,0>",
        //   "p=<3,0,0>, v=<-1,0,0>, a=<0,0,0>"
        //};
        //
        //if (PartTwo(lines) != "1") return false;
        
        return true;
    }
}