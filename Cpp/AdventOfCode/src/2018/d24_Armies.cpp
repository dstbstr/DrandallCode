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

    Group ParseGroup(const std::string & line, Team team) {
        static const auto numRegex = std::regex(R"(\d+)");
        static const auto immunityRegex = std::regex(R"(immune to ([\w, ]+))");
        static const auto weaknessRegex = std::regex(R"(weak to ([\w, ]+))");
        static const auto damageTypeRegex = std::regex(R"((\w+) damage)");
        static u32 groupId = 1;

        Group result;
        result.Id = groupId++;
        result.Team = team;
        result.IsTargeted = false;
        result.CurrentTarget = 0;

        std::vector<std::string> numbers;
        for (auto it = std::sregex_iterator(line.begin(), line.end(), numRegex); it != std::sregex_iterator(); ++it) {
            numbers.push_back(it->str());
        }

        Constexpr::ParseNumber(numbers[0], result.UnitCount);
        Constexpr::ParseNumber(numbers[1], result.UnitHp);
        Constexpr::ParseNumber(numbers[2], result.AttackDamage);
        Constexpr::ParseNumber(numbers[3], result.Initiative);

        std::smatch match;
        if (std::regex_search(line, match, immunityRegex)) {
            std::string immunities = match[1].str();
            auto split = Constexpr::Split(immunities, ", ");
            for (const auto& str : split) {
                result.Immunities.push_back(ParseDamageType(str));
            }
        }
        if (std::regex_search(line, match, weaknessRegex)) {
            std::string weaknesses = match[1].str();
            auto split = Constexpr::Split(weaknesses, ", ");
            for (const auto& str : split) {
                result.Weaknesses.push_back(ParseDamageType(str));
            }
        }

        std::regex_search(line, match, damageTypeRegex);
        result.AttackType = ParseDamageType(match[1].str());

        return result;
    }

    bool IsBattleRunning(const std::unordered_map<u32, Group>&battle) {
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

    Team FindWinner(const std::unordered_map<u32, Group>&battle) {
        for (const auto& [id, group] : battle) {
            if (group.IsAlive()) {
                return group.Team;
            }
        }

        return Team::Infection;
    }

    void SelectTargets(std::vector<Group>&groups, std::unordered_map<u32, Group>&battle) {
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

    void AttackTargets(const std::vector<Group>&groups, std::unordered_map<u32, Group>&battle) {
        for (const auto& group : groups) {
            auto targetId = battle.at(group.Id).CurrentTarget;
            if (targetId == 0) continue;
            battle.at(targetId).TakeDamage(battle.at(group.Id));
        }
    }

    void ResolveRound(std::unordered_map<u32, Group>&battle) {
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

    std::unordered_map<u32, Group> GetBattle(const std::vector<std::string>&lines) {
        std::unordered_map<u32, Group> battle;
        Team team = Team::Immune;
        for (auto i = 1; i < lines.size(); i++) {
            if (lines[i] == "") {
                team = Team::Infection;
                i++;
                continue;
            }
            auto group = ParseGroup(lines[i], team);
            battle[group.Id] = group;
        }

        return battle;
    }

    u32 CountRemainingUnits(const std::unordered_map<u32, Group>&battle) {
        u32 result = 0;
        for (const auto& [id, group] : battle) {
            result += group.UnitCount;
        }
        return result;
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto battle = GetBattle(lines);
        while (IsBattleRunning(battle)) {
            ResolveRound(battle);
        }

        return CountRemainingUnits(battle);
    }

    auto Part2(const std::vector<std::string>&lines) {
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
                return CountRemainingUnits(battle);
            }
        }

        return 0u;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "Immune System:",
            "17 units each with 5390 hit points (weak to radiation, bludgeoning) with an attack that does 4507 fire damage at initiative 2",
            "989 units each with 1274 hit points (immune to fire; weak to bludgeoning, slashing) with an attack that does 25 slashing damage at initiative 3",
            "",
            "Infection:",
            "801 units each with 4706 hit points (weak to radiation) with an attack that does 116 bludgeoning damage at initiative 1",
            "4485 units each with 2961 hit points (immune to radiation; weak to fire, cold) with an attack that does 12 slashing damage at initiative 4"
        };

        //if (Part1(lines) != 5216) return false;
        if (Part2(lines) != 51) return false;
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