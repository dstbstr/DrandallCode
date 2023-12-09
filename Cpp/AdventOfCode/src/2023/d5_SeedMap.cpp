#include "2023/d5_SeedMap.h"

SOLUTION(2023, 5) {
    //destination, source, length

    struct Seeds {
        std::vector<std::pair<s64, s64>> Ranges;
        constexpr Seeds() {}
        constexpr Seeds(s64 start, s64 length) {
            Ranges.push_back(std::make_pair(start, start + length - 1));
        }

        constexpr s64 GetMin() const {
            s64 best = Ranges[0].first;
            for (const auto& [start, end] : Ranges) {
                best = std::min(best, start);
            }
            return best;
        }
    };

    struct Range {
        s64 Start;
        s64 End;
        s64 Offset;

        constexpr Range(s64 start, s64 length, s64 offset) : Start(start), End(start + length - 1), Offset(offset) {}

        constexpr bool Contains(s64 num) const {
            return Start <= num && num <= End;
        }
    };

    struct Map {
        std::vector<Range> Ranges;

        constexpr s64 Next(s64 current) const {
            for (const auto& range : Ranges) {
                if (range.Contains(current)) {
                    return current + range.Offset;
                }
            }
            return current;
        }

        constexpr Seeds Next(const Seeds& seeds) const {
            Seeds result;

            auto running = seeds.Ranges;
            auto next = running;
            while (!next.empty()) {
                running = next;
                next.clear();

                for (auto& [start, end] : running) {
                    bool done = false;
                    for (const auto& range : Ranges) {
                        if (end < range.Start || start > range.End) continue;
                        auto lhs = std::max(start, range.Start);
                        auto rhs = std::min(end, range.End);
                        result.Ranges.push_back(std::make_pair(lhs + range.Offset, rhs + range.Offset));

                        //update running
                        //overlap on left, overlap on right, in the middle
                        if (lhs > start && rhs < end) {
                            next.push_back(std::make_pair(start, lhs - 1));
                            next.push_back(std::make_pair(rhs + 1, end));
                            done = true;
                            break;
                        }
                        else if (lhs == start && rhs == end) {
                            done = true;
                            break;
                        }
                        else if (lhs == start) {
                            start = rhs + 1;
                        }
                        else if (rhs == end) {
                            end = lhs - 1;
                        }
                    }
                    if (!done) {
                        result.Ranges.push_back(std::make_pair(start, end));
                    }
                }
            }
            
            return result;
        }
    };

    constexpr Range ParseRange(std::string_view sv) {
        auto nums = ParseLineAsNumbers<s64>(sv, " ");
        return Range(nums[1], nums[2], nums[0] - nums[1]);
    }

    constexpr Map ParseMap(const std::vector<std::string>& lines) {
        Map result;
        for (auto i = 1; i < lines.size(); i++) {
            result.Ranges.push_back(ParseRange(lines[i]));
        }

        return result;
    }
    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);
        auto seeds = ParseLineAsNumbers<s64>(groups[0][0].substr(7), " ");

        std::vector<Map> maps;
        for (size_t group = 1; group < groups.size(); group++) {
            maps.push_back(ParseMap(groups[group]));
        }

        s64 best = 99999999999999999;
        for (auto seed : seeds) {
            auto running = seed;
            for (const auto& map : maps) {
                running = map.Next(running);
            }
            best = std::min(best, running);
        }
        return Constexpr::ToString(best);
    }

    //362940 too low
    //10114991 too high
    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);
        auto seedRanges = ParseLineAsNumbers<s64>(groups[0][0].substr(7), " ");
        std::vector<Seeds> seeds = {};
        for (auto i = 0; i < seedRanges.size(); i += 2) {
            seeds.emplace_back(seedRanges[i], seedRanges[i + 1]);
        }

        std::vector<Map> maps;
        for (size_t group = 1; group < groups.size(); group++) {
            maps.push_back(ParseMap(groups[group]));
        }

        s64 best = 99999999999999999;
        for (auto seed : seeds) {
            auto running = seed;
            for (const auto& map : maps) {
                running = map.Next(running);
            }
            best = std::min(best, running.GetMin());
        }
        return Constexpr::ToString(best);
    }

    TESTS() {
        std::vector<std::string> example = {
            "seeds: 79 14 55 13",
            "",
            "seed-to-soil map:",
            "50 98 2",
            "52 50 48",
            "",
            "soil-to-fertilizer map:",
            "0 15 37",
            "37 52 2",
            "39 0 15",
            "",
            "fertilizer-to-water map:",
            "49 53 8",
            "0 11 42",
            "42 0 7",
            "57 7 4",
            "",
            "water-to-light map:",
            "88 18 7",
            "18 25 70",
            "",
            "light-to-temperature map:",
            "45 77 23",
            "81 45 19",
            "68 64 13",
            "",
            "temperature-to-humidity map:",
            "0 69 1",
            "1 0 69",
            "",
            "humidity-to-location map:",
            "60 56 37",
            "56 93 4"
        };

        if (PartOne(example) != "35") return false;
        if (PartTwo(example) != "46") return false;

        return true;
    }
}