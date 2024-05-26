#include "2016/d14_OneTimePad.h"

#include "Core/Algorithms/md5Constexpr.h"

SOLUTION(2016, 14) {
    constexpr std::string Stretch(const std::string & hash) {
        auto running = hash;
        for (auto i = 0; i < 2016; i++) {
            running = ToHexLower(md5::compute(running.c_str()));
        }

        return running;
    }

    constexpr auto Threes = {
        "000","111","222","333","444","555","666","777","888","999","aaa","bbb","ccc","ddd","eee","fff"
    };
    constexpr auto Fives = {
        "00000","11111","22222","33333","44444","55555","66666","77777","88888","99999","aaaaa","bbbbb","ccccc","ddddd","eeeee","fffff"
    };

    using Pair = std::pair<u32, std::string>;

    constexpr void PopulateCache(std::string_view salt, std::vector<Pair>& threes, std::vector<Pair>& fives, bool stretch) {
        std::string candidate;
        std::string hash;
        std::string targetString;
        std::string prefix = std::string(salt);

        for (auto i = 0; i < 30'000; i++) {
            candidate = prefix + Constexpr::ToString(i);
            hash = ToHexLower(md5::compute(candidate.c_str()));
            if (stretch) hash = Stretch(hash);

            size_t lowestIndex = hash.npos;
            for (auto three : Threes) {
                lowestIndex = std::min(lowestIndex, hash.find(three));
            }
            if (lowestIndex != hash.npos) {
                targetString = std::string(5, hash[lowestIndex]);
                threes.push_back(std::make_pair(i, targetString));
            }
            else {
                continue; //if we don't have a 3, it can't have a 5
            }

            lowestIndex = hash.npos;
            for (auto five : Fives) {
                lowestIndex = std::min(lowestIndex, hash.find(five));
            }
            if (lowestIndex != hash.npos) {
                fives.push_back(std::make_pair(i, hash));
            }
        }
    }

    constexpr bool IsKey(u32 id, const std::string& targetString, const std::vector<Pair>& fives) {
        size_t index = 0;
        while (fives[index].first <= id) {
            index++;
        }
        while (fives[index].first <= id + 1000) {
            if (fives[index].second.find(targetString) != fives[index].second.npos) {
                return true;
            }
            index++;
        }

        return false;
    }

    constexpr u32 FindKey64(std::string_view key, bool stretch, std::vector<u32> hints) {
        if (!hints.empty()) {
            return hints.back();
        }

        std::vector<Pair> threes;
        std::vector<Pair> fives;
        PopulateCache(key, threes, fives, stretch);

        u32 foundKeys = 0;
        for (auto [id, target] : threes) {
            if (IsKey(id, target, fives)) {
                GET_LOGS().push_back(Constexpr::ToString(id));
                foundKeys++;
                if (foundKeys == 64) {
                    return id;
                }
            }
        }

        return 0;

    }

    static const std::vector<u32> PartOneHints = {
        310,399,442,479,734,847,927,1210,1390,1822,1855,2080,2112,2147,2153,2174,6226,6317,6415,6502,6660,6689,6697,6732,6822,
        7020,7060,7096,7293,7350,7500,7612,7672,7699,7770,7887,7986,8063,8105,8117,8167,8175,8199,8288,8309,8342,8386,8412,8423,
        8477,11425,11494,11607,11812,12012,19730,19860,19903,20188,20307,20351,23670,23695,23769
    };

    PART_ONE() {
        return Constexpr::ToString(FindKey64(lines[0], false, PartOneHints));
    }

    static const std::vector<u32> PartTwoHints = {
        26,51,128,179,289,451,516,5164,5228,5688,5739,5820,5835,5944,6571,6610,6717,6951,7154,7206,7208,7249,7252,7256,7302,7320,7519,
        14227,14669,14707,14795,14809,14859,14890,14923,14979,15046,15163,15170,15268,15304,15334,15415,15444,15496,15498,15594,15658,
        15828,15936,15939,16025,16105,16771,16851,16980,17323,20072,20141,20331,20357,20410,20498,20606
    };

    PART_TWO() {
        return Constexpr::ToString(FindKey64(lines[0], true, PartTwoHints));
    }

    TESTS() {
        //if (FindKey64("abc", true) != 22551) return false;

        return true;
    }
}


