#include "2018/d25_TimePortal.h"

SOLUTION(2018, 25) {
    using Star = Vec4<s32>;

    constexpr Star ParseStar(std::string_view line) {
        Star result;
        auto split = Constexpr::Split(line, ",");

        Constexpr::ParseNumber(split[0], result.X);
        Constexpr::ParseNumber(split[1], result.Y);
        Constexpr::ParseNumber(split[2], result.Z);
        Constexpr::ParseNumber(split[3], result.W);

        return result;
    }

    static_assert(ParseStar("-1,2,-3,10") == Star{-1, 2, -3, 10});

    struct Constellation {
        std::vector<Star> Stars;

        constexpr bool CanJoin(const Constellation& other) const {
            for (const Star& star : Stars) {
                for (const Star& otherStar : other.Stars) {
                    if (MDistance(star, otherStar) <= 3) {
                        return true;
                    }
                }
            }

            return false;
        }
    };

    PART_ONE() {
        std::vector<Constellation> constellations;
        for (const auto& line : lines) {
            auto star = ParseStar(line);
            auto constellation = Constellation{};
            constellation.Stars.push_back(star);
            constellations.push_back(constellation);
        }

        std::vector<size_t> toRemove;
        while (true) {
            for (size_t i = 0; i < constellations.size(); i++) {
                if (std::find(toRemove.begin(), toRemove.end(), i) != toRemove.end()) continue;

                for (size_t j = i + 1; j < constellations.size(); j++) {
                    if (constellations[i].CanJoin(constellations[j])) {
                        std::copy(constellations[j].Stars.begin(), constellations[j].Stars.end(), std::back_inserter(constellations[i].Stars));
                        toRemove.push_back(j);
                    }
                }
            }

            if (toRemove.empty()) {
                break;
            }

            std::vector<Constellation> next;
            for (size_t i = 0; i < constellations.size(); i++) {
                if (std::find(toRemove.begin(), toRemove.end(), i) == toRemove.end()) {
                    next.push_back(constellations[i]);
                }
            }

            constellations = next;
            toRemove.clear();
        }

        return Constexpr::ToString(constellations.size());
    }

    PART_TWO() {
        (void)lines;
        return "Merry Christmas";
    }

    TESTS() {
        //std::vector<std::string> lines = {
        //    "1,-1,-1,-2",
        //    "-2,-2,0,1",
        //    "0,2,1,3",
        //    "-2,3,-2,1",
        //    "0,2,3,-2",
        //    "-1,-1,1,-2",
        //    "0,-2,-1,0",
        //    "-2,2,3,-1",
        //    "1,2,2,0",
        //    "-1,-2,0,-2"
        //};
        //
        //if (PartOne(lines) != "8") return false;

        return true;
    }
}