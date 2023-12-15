#include "2023/d12_HotSprings.h"

SOLUTION(2023, 12) {
    using Cache = Constexpr::SmallMap<std::tuple<size_t, size_t, size_t>, size_t>;

    constexpr size_t Recurse(const std::string& line, const std::vector<u32>& counts, Cache& cache, size_t pos, size_t grpIdx, size_t currentLength) {
        if (pos == line.size()) {
            if (grpIdx >= counts.size()) {
                return 1;
            }
            else if (grpIdx == counts.size() - 1 && counts[grpIdx] == currentLength) {
                return 1;
            }
            return 0;
        }
        auto c = line[pos];
        if (c == '.') {
            if (currentLength == 0) return Recurse(line, counts, cache, pos + 1, grpIdx, 0);
            else if (grpIdx >= counts.size() || currentLength != counts[grpIdx]) return 0;
            else return Recurse(line, counts, cache, pos + 1, grpIdx + 1, 0);
        }
        else if (c == '#') {
            if (grpIdx >= counts.size() || currentLength + 1 > counts[grpIdx]) return 0;
            return Recurse(line, counts, cache, pos + 1, grpIdx, currentLength + 1);
        }
        else {
            auto key = std::tuple(pos, grpIdx, currentLength);
            if (cache.contains(key)) return cache.at(key);

            size_t result = 0;
            if (currentLength == 0) result += Recurse(line, counts, cache, pos + 1, grpIdx, 0);
            if (grpIdx < counts.size()) {
                if (currentLength < counts[grpIdx]) result += Recurse(line, counts, cache, pos + 1, grpIdx, currentLength + 1);
                else result += Recurse(line, counts, cache, pos + 1, grpIdx + 1, 0);
            }

            cache[key] = result;
            return result;
        }
    }

    constexpr size_t CountValid(const std::string line) {
        auto s = Constexpr::Split(line, " ");
        auto pattern = std::string(s[0]);
        auto counts = ParseLineAsNumbers<u32>(s[1]);
        Cache cache{};
        return Recurse(pattern, counts, cache, 0, 0, 0);
    }
    
    PART_ONE() {
        return Constexpr::ToString(std::accumulate(lines.begin(), lines.end(), 0ull, [](size_t running, const std::string& line) {
            return running + CountValid(line);
            }));
    }

    constexpr std::string Unfold(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        std::vector<std::string> pattern{ 5, std::string(s[0]) };
        std::vector<std::string> counts{ 5, std::string(s[1]) };
        
        return Constexpr::JoinVec("?", pattern) + " " + Constexpr::JoinVec(',', counts);
    }

    PART_TWO() {
        std::vector<std::string> unfolded;
        std::transform(lines.begin(), lines.end(), std::back_inserter(unfolded), Unfold);
        return PartOne(unfolded);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "???.### 1,1,3",
            ".??..??...?##. 1,1,3",
            "?#?#?#?#?#?#?#? 1,3,1,6",
            "????.#...#... 4,1,1",
            "????.######..#####. 1,6,5",
            "?###???????? 3,2,1"
        };

        if (CountValid("???.### 1,1,3") != 1) return false;
        if (CountValid(".??..??...?##. 1,1,3") != 4) return false;
        if (CountValid("?#?#?#?#?#?#?#? 1,3,1,6") != 1) return false;
        if (CountValid("????.#...#... 4,1,1") != 1) return false;
        if (CountValid("????.######..#####. 1,6,5") != 4) return false;
        if (CountValid("?###???????? 3,2,1") != 10) return false;

        if (PartOne(lines) != "21") return false;
        if (PartTwo(lines) != "525152") return false;

        return true;
    }
}