#include "2023/d6_BoatRace.h"

SOLUTION(2023, 6) {
    constexpr std::vector<std::pair<u64, u64>> ParseInput(const std::vector<std::string>&lines) {
        auto timeTrimmed = lines[0].substr(6);
        auto distTrimmed = lines[1].substr(10);
        auto timeStrs = Constexpr::Split(timeTrimmed, " ");
        auto distStrs = Constexpr::Split(distTrimmed, " ");

        std::vector<std::pair<u64, u64>> result;
        for (auto i = 0; i < timeStrs.size(); i++) {
            auto p = std::make_pair(0u, 0u);
            Constexpr::ParseNumber(timeStrs[i], p.first);
            Constexpr::ParseNumber(distStrs[i], p.second); 
            result.push_back(p);
        }

        return result;
    }

    constexpr std::pair<u64, u64> ParseAsSingle(const std::vector<std::string>& lines) {
        auto timeTrimmed = lines[0].substr(6);
        auto distTrimmed = lines[1].substr(10);
        Constexpr::ReplaceAll(timeTrimmed, " ", "");
        Constexpr::ReplaceAll(distTrimmed, " ", "");

        std::pair<u64, u64> result{};
        Constexpr::ParseNumber(timeTrimmed, result.first);
        Constexpr::ParseNumber(distTrimmed, result.second);

        return result;
    }

    constexpr u64 CountWins(u64 time, u64 distance) {
        auto t = 1;
        while (t * (time - t) <= distance) t++;
        return (time - t) - t + 1;
    }

    PART_ONE() {
        auto pairs = ParseInput(lines);
        u64 result = 1;
        for (auto [time, dist] : pairs) {
            result *= CountWins(time, dist);
        }
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto [time, dist] = ParseAsSingle(lines);

        return Constexpr::ToString(CountWins(time, dist));
    }

    TESTS() {
        std::vector < std::string> lines = {
            "Time:      7  15   30",
            "Distance:  9  40  200"
        };

        if (PartOne(lines) != "288") return false;
        return true;
    }
}