#include "2018/d24_Armies.h"

SOLUTION(2018, 24) {
    enum struct DamageType { Unknown, Fire, Cold, Slashing, Bludgeoning, Radiation };
    enum struct Team { Immune, Infection };
    constexpr DamageType ParseDamageType(std::string_view str) {
        if (str == "fire") return DamageType::Fire;
        else if (str == "cold") return DamageType::Cold;
        else if (str == "slashing") return DamageType::Slashing;
        else if (str == "bludgeoning") return DamageType::Bludgeoning;
        else if (str == "radiation") return DamageType::Radiation;
        else {
            return DamageType::Unknown;
        }
    }

    struct Group {
        u32 Id = 0;
        u32 UnitCount = 0;
        u32 UnitHp = 0;
        u32 AttackDamage = 0;
        u32 Initiative = 0;
        DamageType AttackType = DamageType::Fire;
        Team Team = Team::Immune;
        bool IsTargeted = false;
        u32 CurrentTarget = 0;
        std::vector<DamageType> Immunities;
        std::vector<DamageType> Weaknesses;

        constexpr bool IsAlive() const {
            return UnitCount > 0;
        }
        constexpr u32 EffectivePower() const {
            return UnitCount * AttackDamage;
        }
        constexpr std::vector<Group> GetPotentialTargets(const std::vector<Group>& groups) const {
            const auto OtherTeam = [&](const Group& group) {
                return Team != group.Team && !group.IsTargeted;
            };

            std::vector<Group> result;
            std::copy_if(groups.cbegin(), groups.cend(), std::back_inserter(result), OtherTeam);
            return result;
        }
        constexpr u32 GetPotentialDamage(const Group& defender) const {
            if (std::find(defender.Immunities.cbegin(), defender.Immunities.cend(), AttackType) != defender.Immunities.cend()) {
                return 0;
            }

            if (std::find(defender.Weaknesses.cbegin(), defender.Weaknesses.cend(), AttackType) != defender.Weaknesses.cend()) {
                return EffectivePower() * 2;
            }
            return EffectivePower();
        }

        void TakeDamage(const Group& attacker) {
            auto damage = attacker.EffectivePower();
            if (std::find(Immunities.cbegin(), Immunities.cend(), attacker.AttackType) != Immunities.cend()) {
                return;
            }
            else if (std::find(Weaknesses.cbegin(), Weaknesses.cend(), attacker.AttackType) != Weaknesses.cend()) {
                damage *= 2;
            }

            auto unitsKilled = damage / UnitHp;
            if (UnitCount < unitsKilled) {
                UnitCount = 0;
            }
            else {
                UnitCount -= unitsKilled;
            }
        }
    };

    /*
    Immune System:
5711 units each with 6662 hit points (immune to fire; weak to slashing) with an attack that does 9 bludgeoning damage at initiative 14
2108 units each with 8185 hit points (weak to radiation, bludgeoning) with an attack that does 36 slashing damage at initiative 13
1590 units each with 3940 hit points with an attack that does 24 cold damage at initiative 5
2546 units each with 6960 hit points with an attack that does 25 slashing damage at initiative 2
1084 units each with 3450 hit points (immune to bludgeoning) with an attack that does 27 slashing damage at initiative 11
265 units each with 8223 hit points (immune to radiation, bludgeoning, cold) with an attack that does 259 cold damage at initiative 12
6792 units each with 6242 hit points (immune to slashing; weak to bludgeoning, radiation) with an attack that does 9 slashing damage at initiative 18
3336 units each with 12681 hit points (weak to slashing) with an attack that does 28 fire damage at initiative 6
752 units each with 5272 hit points (immune to slashing; weak to bludgeoning, radiation) with an attack that does 69 radiation damage at initiative 4
96 units each with 7266 hit points (immune to fire) with an attack that does 738 bludgeoning damage at initiative 8
    */
    constexpr Group ParseGroup(const std::string& line, Team team) {
        std::string unitId = " units each with ";
        std::string atkId = " with an attack that does ";
        std::string initId = " damage at initiative ";

        Group result;
        result.Team = team;
        auto s0 = Constexpr::Split(line, unitId);
        Constexpr::ParseNumber(s0[0], result.UnitCount);
        auto s1 = Constexpr::Split(s0[1], " ");
        Constexpr::ParseNumber(s1[0], result.UnitHp);

        auto s2 = Constexpr::Split(line, atkId);
        auto s3 = Constexpr::Split(s2[1], " ");
        Constexpr::ParseNumber(s3[0], result.AttackDamage);
        result.AttackType = ParseDamageType(s3[1]);
        Constexpr::ParseNumber(s3.back(), result.Initiative);

        auto s4 = Constexpr::Split(line, "(");
        if (s4.size() == 1) return result;
        auto s5 = Constexpr::Split(s4[1], ")");

        auto s6 = Constexpr::Split(s5[0], ";");

        for (auto modifier : s6) {
            auto words = Constexpr::RemoveAllOfCopy(std::string(modifier), ",");
            auto s = Constexpr::Split(words, " ");
            std::vector<DamageType> damageTypes;
            for (auto i = 2; i < s.size(); i++) {
                damageTypes.push_back(ParseDamageType(s[i]));
            }
            if (s[0] == "immune") {
                std::copy(damageTypes.begin(), damageTypes.end(), std::back_inserter(result.Immunities));
            }
            else if (s[0] == "weak") {
                std::copy(damageTypes.begin(), damageTypes.end(), std::back_inserter(result.Weaknesses));
            }
        }

        return result;
    }
    using Battle = Constexpr::SmallMap<u32, Group>;

    constexpr bool IsBattleRunning(const Battle& battle) {
        bool hasImmune = false, hasInfection = false;
        for (const auto& [id, group] : battle) {
            if (group.IsAlive() && group.Team == Team::Immune) {
                hasImmune = true;
                if (hasInfection) return true;
            }
            else if (group.IsAlive() && group.Team == Team::Infection) {
                hasInfection = true;
                if (hasImmune) return true;
            }
        }

        return false;
    }

    constexpr Team FindWinner(const Battle& battle) {
        for (const auto& [id, group] : battle) {
            if (group.IsAlive()) {
                return group.Team;
            }
        }

        return Team::Infection;
    }

    constexpr void SelectTargets(std::vector<Group>&groups, Battle& battle) {
        for (const auto& group : groups) {
            auto TargetOrder = [group](const Group& lhs, const Group& rhs) {
                auto lhsDamage = group.GetPotentialDamage(lhs);
                auto rhsDamage = group.GetPotentialDamage(rhs);
                if (lhsDamage != rhsDamage) {
                    return rhsDamage < lhsDamage;
                }
                if (lhs.EffectivePower() != rhs.EffectivePower()) {
                    return rhs.EffectivePower() < lhs.EffectivePower();
                }
                return rhs.Initiative < lhs.Initiative;
            };

            auto targets = group.GetPotentialTargets(groups);
            if (targets.empty()) continue;
            std::sort(targets.begin(), targets.end(), TargetOrder);
            if (group.GetPotentialDamage(targets[0]) > 0) {
                battle.at(group.Id).CurrentTarget = targets[0].Id;
                battle.at(targets[0].Id).IsTargeted = true;

                for (auto& g : groups) {
                    if (g.Id == targets[0].Id) {
                        g.IsTargeted = true;
                        break;
                    }
                }
            }
        }
    }

    constexpr void AttackTargets(const std::vector<Group>&groups, Battle& battle) {
        for (const auto& group : groups) {
            auto targetId = battle.at(group.Id).CurrentTarget;
            if (targetId == 0) continue;
            battle.at(targetId).TakeDamage(battle.at(group.Id));
        }
    }

    constexpr void ResolveRound(Battle& battle) {
        auto selectionOrder = [](const Group& lhs, const Group& rhs) {
            if (lhs.EffectivePower() == rhs.EffectivePower()) {
                return rhs.Initiative < lhs.Initiative;
            }
            return rhs.EffectivePower() < lhs.EffectivePower();
        };
        auto actionOrder = [](const Group& lhs, const Group& rhs) {
            return rhs.Initiative < lhs.Initiative;
        };

        std::vector<Group> groups;
        for (auto& [id, group] : battle) {
            if (!group.IsAlive()) continue;
            group.CurrentTarget = 0;
            group.IsTargeted = false;
            groups.push_back(group);
        }

        std::sort(groups.begin(), groups.end(), selectionOrder);
        SelectTargets(groups, battle);

        std::sort(groups.begin(), groups.end(), actionOrder);
        AttackTargets(groups, battle);
    }

    constexpr Battle GetBattle(const std::vector<std::string>&lines) {
        Battle battle;
        Team team = Team::Immune;
        u32 groupId = 1;
        for (auto i = 1; i < lines.size(); i++) {
            if (lines[i] == "") {
                team = Team::Infection;
                i++;
                continue;
            }
            auto group = ParseGroup(lines[i], team);
            group.Id = groupId++;
            battle[group.Id] = group;
        }

        return battle;
    }

    constexpr u32 CountRemainingUnits(const Battle& battle) {
        return std::accumulate(battle.begin(), battle.end(), 0, [](u32 previous, auto& p) {return previous + p.second.UnitCount; });
    }

    PART_ONE() {
        auto battle = GetBattle(lines);
        while (IsBattleRunning(battle)) {
            ResolveRound(battle);
        }

        return Constexpr::ToString(CountRemainingUnits(battle));
    }

    PART_TWO() {
        const auto initialBattle = GetBattle(lines);

        const int maxRounds = 5000;

        u32 boost = 0;
        while (true) {
            boost++;
            auto battle = initialBattle;
            for (auto& [id, group] : battle) {
                if (group.Team == Team::Immune) {
                    group.AttackDamage += boost;
                }
            }

            for (auto i = 0; i < maxRounds; i++) {
                ResolveRound(battle);
                if (!IsBattleRunning(battle)) {
                    break;
                }
            }

            if (!IsBattleRunning(battle) && FindWinner(battle) == Team::Immune) {
                return Constexpr::ToString(CountRemainingUnits(battle));
            }
        }

        return 0u;
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Immune System:",
            "17 units each with 5390 hit points (weak to radiation, bludgeoning) with an attack that does 4507 fire damage at initiative 2",
            "989 units each with 1274 hit points (immune to fire; weak to bludgeoning, slashing) with an attack that does 25 slashing damage at initiative 3",
            "",
            "Infection:",
            "801 units each with 4706 hit points (weak to radiation) with an attack that does 116 bludgeoning damage at initiative 1",
            "4485 units each with 2961 hit points (immune to radiation; weak to fire, cold) with an attack that does 12 slashing damage at initiative 4"
        };

        if (PartOne(lines) != "5216") return false;
        if (PartTwo(lines) != "51") return false;

        return true;
    }
}