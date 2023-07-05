#include "2022/d19_GatherOre.h"
#include "Algorithms/AStar.h"

SOLUTION(2022, 19) {

    enum Mineral : u32 { Ore, Clay, Obsidian, Geode, COUNT };

    struct Robot {
        Mineral Produces = Mineral::Ore;
        std::array<u32, Mineral::COUNT> Costs{};
    };

    struct Bp {
        u32 Index = 0;

        std::array<Robot, Mineral::COUNT> Costs{};
    };

    struct Resources {
        std::array<u32, Mineral::COUNT> Minerals{};
        std::array<u32, Mineral::COUNT> Bots{};
        u32 RemainingTime = 0;

        void Tick() {
            for (u32 i = 0; i < Mineral::COUNT; i++) {
                Minerals[i] += Bots[i];
            }
            RemainingTime--;
        }

        constexpr bool operator==(const Resources& other) const {
            return Minerals == other.Minerals && Bots == other.Bots && RemainingTime == other.RemainingTime;
        }
    };

    constexpr bool CanAfford(const Robot & robot, const Resources & resources) {
        for (u32 i = 0; i < Mineral::COUNT; i++) {
            if (robot.Costs[i] > resources.Minerals[i]) return false;
        }

        return true;
    }

    constexpr void Purchase(const Robot & robot, Resources & resources) {
        for (u32 i = 0; i < Mineral::COUNT; i++) {
            resources.Minerals[i] -= robot.Costs[i];
        }
        resources.Bots[robot.Produces]++;
    }

    static constexpr std::array<u32, Mineral::COUNT> GetMaxCosts(const Bp & bp) {
        std::array<u32, Mineral::COUNT> result;
        for (auto mineral = 0; mineral < Mineral::COUNT; mineral++) {
            u32 most = 0;
            for (auto cost = 0; cost < Mineral::COUNT; cost++) {
                most = std::max(most, bp.Costs[cost].Costs[mineral]);
            }
            result[mineral] = most;
        }
        return result;
    }

    Bp ParseLine(const std::string & line) {
        static auto re = std::regex(R"(Blueprint (\d+): [\w ]+ (\d+) ore. [\w ]+ (\d+) ore. [\w ]+ (\d+) ore and (\d+) clay. [\w ]+ (\d+) ore and (\d+) obsidian.)");
        std::smatch match;
        if (!std::regex_match(line, match, re)) {
            throw std::logic_error("Failed to parse");
        }

        Bp bp;

        Constexpr::ParseNumber(match[1].str(), bp.Index);
        Constexpr::ParseNumber(match[2].str(), bp.Costs[Ore].Costs[Ore]);
        Constexpr::ParseNumber(match[3].str(), bp.Costs[Clay].Costs[Ore]);
        Constexpr::ParseNumber(match[4].str(), bp.Costs[Obsidian].Costs[Ore]);
        Constexpr::ParseNumber(match[5].str(), bp.Costs[Obsidian].Costs[Clay]);
        Constexpr::ParseNumber(match[6].str(), bp.Costs[Geode].Costs[Ore]);
        Constexpr::ParseNumber(match[7].str(), bp.Costs[Geode].Costs[Obsidian]);

        bp.Costs[Ore].Produces = Ore;
        bp.Costs[Clay].Produces = Clay;
        bp.Costs[Obsidian].Produces = Obsidian;
        bp.Costs[Geode].Produces = Geode;

        return bp;
    }


    u32 GetScore(const Bp&, const Resources & resource) {
        auto geodes = resource.Minerals[Geode];
        geodes += resource.Bots[Geode] * resource.RemainingTime;
        auto obs = resource.Bots[Obsidian];
        auto clay = resource.Bots[Clay];
        auto ore = resource.Bots[Ore];

        return geodes * 100000 + obs * 1000 + clay * 10 + ore;
    }

    u32 FindMostGeodes(const Bp & bp, u32 minutes) {
        const auto maxCosts = GetMaxCosts(bp);

        Resources initial;
        initial.Bots[Ore] = 1;
        initial.RemainingTime = minutes;

        u32 minute = 1;
        const auto toKeep = 800000;
        auto sortResources = [&bp](const Resources& lhs, const Resources& rhs) {
            return GetScore(bp, lhs) > GetScore(bp, rhs);
        };

        std::vector<Resources> current{ initial };
        std::vector<Resources> next;

        for (; minute <= minutes; minute++) {
            for (auto& resource : current) {
                for (auto mineral = 0; mineral < Mineral::COUNT; mineral++) {
                    if (resource.Bots[mineral] >= maxCosts[mineral] && mineral != Geode) continue;
                    if (CanAfford(bp.Costs[mineral], resource)) {
                        Resources nextResources = resource;
                        nextResources.Tick();
                        Purchase(bp.Costs[mineral], nextResources);
                        next.push_back(nextResources);
                    }
                }
                resource.Tick();
                next.push_back(resource);
            }

            if (next.size() < toKeep) {
                current = next;
            }
            else {
                std::sort(next.begin(), next.end(), sortResources);
                current.clear();

                std::copy_n(next.begin(), toKeep, std::back_inserter(current));
            }

            next.clear();
        }

        std::sort(current.begin(), current.end(), sortResources);
        u32 best = 0;
        for (const auto& resource : current) {
            best = std::max(best, resource.Minerals[Geode]);
        }

        return best;
    }


    u32 SumQualityLevels(const std::vector<std::string>&lines) {
        std::vector<Bp> bps;
        for (const auto& line : lines) {
            bps.push_back(ParseLine(line));
        }

        u32 result = 0;
        for (const auto& bp : bps) {
            result += FindMostGeodes(bp, 24) * bp.Index;
        }

        return result;
    }

    u32 MultiplyQualityLevels(const std::vector<std::string>&lines) {
        std::vector<Bp> bps;
        for (auto i = 0; i < 3; i++) {
            bps.push_back(ParseLine(lines[i]));
        }

        u32 result = 1;
        for (const auto& bp : bps) {
            result *= FindMostGeodes(bp, 32);
        }

        return result;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(SumQualityLevels(lines));
        return Constexpr::ToString(MultiplyQualityLevels(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.",
            "Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian."
        };

        std::vector<Bp> bps{};
        for (const auto& line : lines) {
            bps.push_back(ParseLine(line));
        }

        Resources initial{};
        initial.Bots[Ore] = 1;
        initial.RemainingTime = 24;
        if (FindMostGeodes(bps[0], 24) != 9) return false;
        if (FindMostGeodes(bps[1], 24) != 12) return false;

        if (FindMostGeodes(bps[0], 32) != 56) return false;
        if (FindMostGeodes(bps[1], 32) != 62) return false;

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