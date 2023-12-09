#include "2023/d2_CubeGame.h"

SOLUTION(2023, 2) {
    struct Pull {
        u32 Red = 0;
        u32 Green = 0;
        u32 Blue = 0;
    };

    struct Game {
        u32 Id = 0;
        std::vector < Pull> Pulls;

        constexpr bool CouldMatch(Pull target) const {
            return !std::any_of(Pulls.begin(), Pulls.end(), [&](Pull p) {
                return p.Red > target.Red || p.Green > target.Green || p.Blue > target.Blue;
                });
        }

        constexpr u32 GetPower() const {
            Pull max{ 0, 0, 0 };

            for (auto pull : Pulls) {
                max.Red = std::max(max.Red, pull.Red);
                max.Green = std::max(max.Green, pull.Green);
                max.Blue = std::max(max.Blue, pull.Blue);
            }   

            return max.Red * max.Green * max.Blue;
        }
    };

    constexpr Pull ParsePull(std::string_view sv) {
        auto s = Constexpr::Split(sv, ", ");
        Pull result;
        for (auto p : s) {
            auto split = Constexpr::Split(p, " ");
            u32 val;
            Constexpr::ParseNumber(split[0], val);
            if (split[1] == "red") {
                result.Red = val;
            }
            else if (split[1] == "green") {
                result.Green = val;
            }
            else {
                result.Blue = val;
            }
        }

        return result;
    }

    //Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
    constexpr Game ParseGame(std::string_view sv) {
        auto s1 = Constexpr::Split(sv, ": ");
        auto gameId = Constexpr::Split(s1[0], " ")[1];
        auto pullSplit = Constexpr::Split(s1[1], "; ");
        Game result;
        Constexpr::ParseNumber(gameId, result.Id);

        for (auto p : pullSplit) {
            result.Pulls.push_back(ParsePull(p));
        }

        return result;
    }
    PART_ONE() {
        Pull target = { 12, 13, 14 };
        auto games = ParseLines(lines, ParseGame);

        u32 result = 0;
        for (const auto& game : games) {
            result += (game.Id * game.CouldMatch(target));
        }
        return Constexpr::ToString(result);
    }
    //2168 is too low
    PART_TWO() {
        auto games = ParseLines(lines, ParseGame);

        auto result = std::accumulate(games.begin(), games.end(), 0u, [](u32 running, const Game& game) {
            return running + game.GetPower();
            });
        return Constexpr::ToString(result);
    }

    TESTS() {
        return true;
    }
}