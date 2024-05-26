#include "2023/d21_GardenSteps.h"
#include "Core/Algorithms/AStar.h"

SOLUTION(2023, 21) {

    constexpr std::vector<size_t> GetDistances(const std::vector<std::string>&lines) {
        Constexpr::BigMap<RowCol, size_t, 100000> result;
        result.SetSentinel(RowCol{ 9999, 9999 });

        RowCol center = { lines.size() / 2, lines[0].size() / 2 };
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };

        std::vector<RowCol> next{ center };
        size_t steps = 0;
        while (!next.empty()) {
            std::vector<RowCol> staging;
            for (auto pos : next) {
                result[pos] = steps;
            }

            for (auto pos : next) {
                auto ns = GetDirectNeighbors(pos, limits);
                for (auto n : ns) {
                    if (lines[n.Row][n.Col] != '#' && !result.contains(n)) {
                        staging.push_back(n);
                    }
                }
            }

            next.clear();
            std::sort(staging.begin(), staging.end());
            std::unique_copy(staging.begin(), staging.end(), std::back_inserter(next));
            staging.clear();
            steps++;
        }

        return result.GetValues();
    }

    constexpr size_t Solve(const std::vector<std::string>&lines, size_t steps) {
        
        auto distances = GetDistances(lines);
        size_t cornerDistance = lines.size() / 2;
        
        size_t evenPlots = 0, oddPlots = 0, evenCornerPlots = 0, oddCornerPlots = 0;
        for (auto d : distances) {
            if (d % 2 == 0) {
                evenPlots++;
                evenCornerPlots += (d > cornerDistance);
            }
            else {
                oddPlots++;
                oddCornerPlots += (d > cornerDistance);
            }
        }

        size_t n = (steps - cornerDistance) / lines.size();

        auto evenCount = (n + 1) * (n + 1);
        auto oddCount = n * n;
        
        if (n % 2 == 0) std::swap(evenCount, oddCount);

        auto fullSquares = (oddCount * oddPlots) + (evenCount * evenPlots);
        auto minusDiag = n + 1;
        auto addDiag = n;
        if (n % 2 == 0) {
            minusDiag *= oddCornerPlots;
            addDiag *= evenCornerPlots;
        }
        else {
            minusDiag *= evenCornerPlots;
            addDiag *= oddCornerPlots;
        }

        return fullSquares + addDiag - minusDiag;        
    }

    PART_ONE() {
        auto distances = GetDistances(lines);
        auto result = std::count_if(distances.begin(), distances.end(), [](size_t d) {
            return d < 65 && d % 2 == 0;
            });
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, 26501365));
    }

    TESTS() {
        return true;
    }
}