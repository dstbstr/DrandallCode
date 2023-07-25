#include "2019/d10_Asteroid.h"

SOLUTION(2019, 10) {
    /*
    Angle between two points is just the slope, Rise/Run
    Two asteroids are on the same line if they share a slope (fully reduced)
    The closest is the asteroid with the relative position closest to the slope (5,15 and 1,3 are the same slope, but 1,3 doesn't need to be reduced)
    To find the 200th asteroid, sort the targets by quadrant, then by slope (Rise/Run)
    */
    using Map = std::vector<Coord>;
    using VisibilityMap = Constexpr::SmallMap<Coord, Constexpr::SmallMap<Coord, bool>>;

    constexpr Map ParseMap(const std::vector<std::string>&lines) {
        Map result;
        for (s32 row = 0; row < lines.size(); row++) {
            for (s32 col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.push_back({ col, row });
                }
            }
        }
        return result;
    }

    struct Target {
        Coord Pos;
        Coord Slope;
        int Quad;
        size_t Distance;

        constexpr bool operator<(const Target& other) const {
            if (Quad != other.Quad) return Quad < other.Quad;
            if (Slope != other.Slope) {
                auto lhs = static_cast<double>(Slope.Y) / (Slope.X == 0 ? 0.00001 : static_cast<double>(Slope.X));
                auto rhs = static_cast<double>(other.Slope.Y) / (other.Slope.X == 0 ? 0.00001 : static_cast<double>(other.Slope.X));
                return lhs < rhs; //probably not totally correct, but works
            }
            return Distance < other.Distance;
        }

        constexpr bool operator==(const Target& other) const {
            return Slope == other.Slope;
        }
    };

    constexpr Target MakeTarget(Coord start, Coord end) {
        auto relative = Coord{ end.X - start.X, end.Y - start.Y };
        auto gcd = Constexpr::FindGcd(Constexpr::Abs(relative.X), Constexpr::Abs(relative.Y));
        Target t;
        t.Pos = end;
        t.Slope = { relative.X / gcd, relative.Y / gcd };
        t.Distance = gcd;
        if (t.Slope.X < 0) t.Quad = t.Slope.Y <= 0 ? 3 : 2;
        else t.Quad = t.Slope.Y < 0 ? 0 : 1;
        return t;
    }

    constexpr std::vector<Target> FindMostTargets(const Map& map) {
        std::vector<Target> result;
        Constexpr::SmallSet<Target> targets;
        size_t count = 0;
        for (const auto& start : map) {
            targets.clear();
            for (const auto& end : map) {
                if (start == end) continue;
                auto t = MakeTarget(start, end);
                if (!targets.insert(t)) {
                    auto& existing = targets[t];
                    if (t.Distance < existing.Distance) { //only keep the closest target
                        existing = t;
                    }
                }
            }

            if (count < targets.size()) {
                count = targets.size();

                result = std::vector<Target>(targets.begin(), targets.end());
            }
        }

        return result;
    }

    PART_ONE() {
        auto map = ParseMap(lines);
        auto targets = FindMostTargets(map);
        return Constexpr::ToString(targets.size());
    }

    PART_TWO() {
        auto map = ParseMap(lines);
        auto targets = FindMostTargets(map);
        std::sort(targets.begin(), targets.end());

        auto result = (targets[199].Pos.X * 100) + targets[199].Pos.Y;
        return Constexpr::ToString(result);
    }

    TESTS() {
        std::vector<std::string> lines = {
            ".#..#",
            ".....",
            "#####",
            "....#",
            "...##"
        };
        
        if (PartOne(lines) != "8") return false;

        lines = {
            "......#.#.",
            "#..#.#....",
            "..#######.",
            ".#.#.###..",
            ".#..#.....",
            "..#....#.#",
            "#..#....#.",
            ".##.#..###",
            "##...#..#.",
            ".#....####"
        };
        if (PartOne(lines) != "33") return false;

        lines = {
            "#.#...#.#.",
            ".###....#.",
            ".#....#...",
            "##.#.#.#.#",
            "....#.#.#.",
            ".##..###.#",
            "..#...##..",
            "..##....##",
            "......#...",
            ".####.###."
        };
        if (PartOne(lines) != "35") return false;

        lines = {
            ".#..#..###",
            "####.###.#",
            "....###.#.",
            "..###.##.#",
            "##.##.#.#.",
            "....###..#",
            "..#.#..#.#",
            "#..#.#.###",
            ".##...##.#",
            ".....#.#.."
        };
        if (PartOne(lines) != "41") return false;

        lines = {
            ".#..##.###...#######",
            "##.############..##.",
            ".#.######.########.#",
            ".###.#######.####.#.",
            "#####.##.#.##.###.##",
            "..#####..#.#########",
            "####################",
            "#.####....###.#.#.##",
            "##.#################",
            "#####.##.###..####..",
            "..######..##.#######",
            "####.##.####...##..#",
            ".#####..#.######.###",
            "##...#.##########...",
            "#.##########.#######",
            ".####.#.###.###.#.##",
            "....##.##.###..#####",
            ".#.#.###########.###",
            "#.#.#.#####.####.###",
            "###.##.####.##.#..##"
        };

        if (PartOne(lines) != "210") return false;
        //if (PartTwo(lines) != "802") return false;

        return true;
    }
}