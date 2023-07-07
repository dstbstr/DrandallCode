#include "2016/d22_GridComputing.h"
#include "Algorithms/AStar.h"

SOLUTION(2016, 22) {
    struct Dir {
        UCoord Pos;
        u32 Size;
        u32 Used;
        u32 Available;
        u32 PercentFull;
    };

    constexpr Dir ParseLine(const std::string & line) {
        Dir result;
        auto xPos = line.find('x');
        auto yPos = line.find('y');
        Constexpr::ParseNumber(line.substr(xPos + 1, line.find('-', xPos) - xPos - 1), result.Pos.X);
        Constexpr::ParseNumber(line.substr(yPos + 1, line.find(' ', yPos) - yPos - 1), result.Pos.Y);

        auto split = Constexpr::Split(line, " ");
        Constexpr::ParseNumber(split[1].substr(0, split[1].size() - 1), result.Size);
        Constexpr::ParseNumber(split[2].substr(0, split[2].size() - 1), result.Used);
        Constexpr::ParseNumber(split[3].substr(0, split[3].size() - 1), result.Available);
        Constexpr::ParseNumber(split[4].substr(0, split[4].size() - 1), result.PercentFull);

        return result;
    }

    constexpr bool CanFitContents(const Dir & from, const Dir & to) {
        if (from.Used == 0) return false;
        if (from.Pos == to.Pos) return false;
        return from.Used <= to.Available;
    }

    PART_ONE() {
        auto skipOne = std::vector<std::string>{ lines.begin() + 1, lines.end() };
        auto dirs = ParseLines(skipOne, ParseLine);

        u32 fitCount = 0;
        for (auto i = 0; i < dirs.size() - 1; i++) {
            for (auto j = i; j < dirs.size(); j++) {
                if (CanFitContents(dirs[i], dirs[j]) || CanFitContents(dirs[j], dirs[i])) {
                    fitCount++;
                }
            }
        }
        return Constexpr::ToString(fitCount);
    }
    PART_TWO() {
        Constexpr::SmallMap<UCoord, Dir> map;
        
        UCoord origin{ 0, 0 };
        UCoord emptyDir{ 0, 0 };
        UCoord limits{ 0, 0 };
        for (auto i = 2; i < lines.size(); i++) {
            auto dir = ParseLine(lines[i]);
            map[dir.Pos] = dir;
            if (dir.Used == 0) {
                emptyDir = dir.Pos;
            }
            if (dir.Pos.Y > limits.Y) {
                limits.Y = dir.Pos.Y;
            }
            if (dir.Pos.X > limits.X) {
                limits.X = dir.Pos.X;
            }
        }

        UCoord goal = { limits.X, 0 };
        UCoord target = { 0, 0 };

        auto neighborFunc = [&map, &limits, &goal](const UCoord& pos) {
            auto neighbors = GetDirectNeighbors(pos, limits);
            std::vector<UCoord> result;
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&map, &pos, &goal](const UCoord& next) {
                return next != goal && map.at(next).Used <= map.at(pos).Size;
                });
            return result;
        };

        auto optimalPath = AStarMin<UCoord>(goal, target, neighborFunc);
        u32 steps = 0;

        for (auto i = 1; i < optimalPath.size(); i++) {
            target = optimalPath[i];
            auto path = AStarMin<UCoord>(emptyDir, target, neighborFunc);
            steps += static_cast<u32>(path.size());
            emptyDir = optimalPath[i - 1];
            goal = optimalPath[i];
        }

        return Constexpr::ToString(steps);
    }

    TESTS() {
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").Pos.X == 0);
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").Pos.Y == 0);
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").Size == 93);
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").Used == 67);
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").Available == 26);
        static_assert(ParseLine("/dev/grid/node-x0-y0     93T   67T    26T   72%").PercentFull == 72);

        static_assert(ParseLine("/dev/grid/node-x123-y234     93T   67T    26T   72%").Pos.X == 123);
        static_assert(ParseLine("/dev/grid/node-x123-y234     93T   67T    26T   72%").Pos.Y == 234);

        std::vector<std::string> lines = {
            "root@ebhq-gridcenter# df -h",
            "Filesystem            Size  Used  Avail  Use%",
            "/dev/grid/node-x0-y0   10T    8T     2T   80%",
            "/dev/grid/node-x0-y1   11T    6T     5T   54%",
            "/dev/grid/node-x0-y2   32T   28T     4T   87%",
            "/dev/grid/node-x1-y0    9T    7T     2T   77%",
            "/dev/grid/node-x1-y1    8T    0T     8T    0%",
            "/dev/grid/node-x1-y2   11T    7T     4T   63%",
            "/dev/grid/node-x2-y0   10T    6T     4T   60%",
            "/dev/grid/node-x2-y1    9T    8T     1T   88%",
            "/dev/grid/node-x2-y2    9T    6T     3T   66%"
        };

        if (PartTwo(lines) != "7") return false;
        return true;
    }
}