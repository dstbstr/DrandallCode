#include "2019/d20_PlutoMaze.h"
#include "Algorithms/AStar.h"

#include <map>

SOLUTION(2019, 20) {
    using WalkingMap = std::unordered_set<RowCol>;
    using PortalMap = std::unordered_map<RowCol, RowCol>;

    void ParseInput(const std::vector<std::string>&lines, WalkingMap & outWalkingMap, PortalMap & outPortalMap, RowCol & outEntrance, RowCol & outExit) {
        RowCol origin = { 0, 0 };
        std::array<RowCol, Constexpr::FromBase26("zz")> foundPortals{ origin };
        std::string key = "  ";
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                auto c = lines[row][col];
                if (c == ' ' || c == '#') continue;
                if (c == '.') {
                    outWalkingMap.insert({ row, col });
                    continue;
                }

                if (row == lines.size() - 1 || col == lines[row].size() - 1) continue;

                RowCol current = { row, col };

                if (std::isupper(lines[row + 1][col])) {
                    key[0] = static_cast<char>(std::tolower(lines[row][col]));
                    key[1] = static_cast<char>(std::tolower(lines[row + 1][col]));
                    if (row > 0 && lines[row - 1][col] == '.') {
                        current.Row--;
                    }
                    else {
                        current.Row += 2;
                    }
                }
                else if (std::isupper(lines[row][col + 1])) {
                    key[0] = static_cast<char>(std::tolower(lines[row][col]));
                    key[1] = static_cast<char>(std::tolower(lines[row][col + 1]));
                    if (col > 0 && lines[row][col - 1] == '.') {
                        current.Col--;
                    }
                    else {
                        current.Col += 2;
                    }
                }
                else {
                    continue;
                }

                if (key[0] == 'a' && key[1] == 'a') {
                    outEntrance = current;
                    continue;
                }
                else if (key[0] == 'z' && key[1] == 'z') {
                    outExit = current;
                    continue;
                }

                size_t index = Constexpr::FromBase26(key);
                if (foundPortals[index] == origin) {
                    foundPortals[index] = current;
                }
                else {
                    outPortalMap[current] = foundPortals[index];
                    outPortalMap[foundPortals[index]] = current;
                }
            }
        }
    }

    using DistanceMap = std::unordered_map<RowCol, std::unordered_map<RowCol, size_t>>;

    size_t FindDistance(const RowCol & start, const RowCol & end, const WalkingMap & walkingMap, const RowCol & limit) {

        auto n = [&](RowCol pos) {
            auto neighbors = GetDirectNeighbors(pos, limit);
            std::vector<RowCol> result;
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](const RowCol& n) {
                return walkingMap.contains(n);
                });
            return result;
        };

        auto path = AStarMin<RowCol>(start, end, n);
        return path.empty() ? 0 : path.size() - 1;
    }

    DistanceMap BuildDistanceMap(const PortalMap & portalMap, const WalkingMap & walkingMap, const RowCol & entrance, const RowCol & exit, const RowCol & limit) {
        DistanceMap result;

        auto distance = FindDistance(entrance, exit, walkingMap, limit);
        if (distance > 0) {
            result[entrance][exit] = distance;
            result[exit][entrance] = distance;
        }

        for (const auto& [from, to] : portalMap) {
            distance = FindDistance(entrance, from, walkingMap, limit);
            if (distance > 0) {
                result[entrance][from] = distance;
                result[from][entrance] = distance;
            }

            distance = FindDistance(exit, from, walkingMap, limit);
            if (distance > 0) {
                result[exit][from] = distance;
                result[from][exit] = distance;
            }
        }

        for (const auto& [f1, t1] : portalMap) {
            for (const auto& [f2, t2] : portalMap) {
                if (f1 == f2) continue;
                if (result.contains(f1) && result.at(f1).contains(f2)) continue;

                distance = FindDistance(f1, f2, walkingMap, limit);
                if (distance > 0) {
                    result[f1][f2] = distance;
                    result[f2][f1] = distance;
                }
            }
        }

        return result;
    }

    size_t FindShortestPath(const DistanceMap & dMap, const PortalMap & portalMap, RowCol entrance, RowCol exit) {
        struct State {
            RowCol Pos;
            size_t Steps;
        };
        std::map<RowCol, size_t> seen;
        std::queue<State> queue;
        queue.push({ entrance, 0 });

        size_t best = 99999;

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            auto neighbors = dMap.at(current.Pos);
            for (const auto& [to, distance] : neighbors) {
                auto nextDistance = current.Steps + distance + 1;
                if (to == exit) {
                    best = std::min(best, nextDistance);
                    continue;
                }
                if (to == entrance) continue;

                //teleport
                auto nextPos = portalMap.at(to);
                if (seen.contains(nextPos)) {
                    if (seen[nextPos] < nextDistance) continue;
                }
                seen[nextPos] = nextDistance;
                queue.push({ nextPos, nextDistance });
            }
        }

        return best - 1;
    }

    auto Part1(const std::vector<std::string>&lines) {
        PortalMap portalMap;
        WalkingMap walkingMap;
        RowCol entrance, exit;
        ParseInput(lines, walkingMap, portalMap, entrance, exit);

        RowCol limit = { lines.size(), lines[0].size() };
        auto dMap = BuildDistanceMap(portalMap, walkingMap, entrance, exit, limit);

        return FindShortestPath(dMap, portalMap, entrance, exit);
    }

    constexpr bool IsInnerPortal(RowCol pos, RowCol limit) {
        return pos.Row > 3 && pos.Col > 3 && pos.Row < limit.Row - 3 && pos.Col < limit.Col - 3;
    }

    size_t FindShortestRecursivePath(const DistanceMap & dMap, const PortalMap & portalMap, RowCol entrance, RowCol exit, RowCol limit) {
        struct State {
            RowCol Pos;
            size_t Steps;
            size_t Level;

            constexpr bool operator<(const State& other) const {
                return (other.Steps + other.Level * 5) < (Steps + Level * 5);
            }
        };

        std::map<std::pair<RowCol, size_t>, size_t> seen;
        std::priority_queue<State> queue;
        queue.push({ entrance, 0, 1 });

        size_t best = 99999;

        while (!queue.empty()) {
            auto current = queue.top();
            queue.pop();

            auto neighbors = dMap.at(current.Pos);
            for (const auto& [to, distance] : neighbors) {
                auto nextDistance = current.Steps + distance + 1;
                if (nextDistance > best) continue;
                if (to == exit) {
                    if (current.Level == 1) {
                        best = std::min(best, nextDistance);
                    }
                    continue;
                }
                if (to == entrance) continue;
                if (current.Level == 0) continue; //all other doors are locked

                //teleport
                auto nextLevel = current.Level;
                if (IsInnerPortal(to, limit)) {
                    nextLevel++;
                }
                else {
                    nextLevel--;
                }
                auto nextPos = portalMap.at(to);
                auto nextPair = std::make_pair(nextPos, nextLevel);
                if (seen.contains(nextPair)) {
                    if (seen[nextPair] < nextDistance) continue;
                }
                seen[nextPair] = nextDistance;

                queue.push({ nextPos, nextDistance, nextLevel });
            }
        }

        return best - 1;
    }

    auto Part2(const std::vector<std::string>&lines) {
        PortalMap portalMap;
        WalkingMap walkingMap;
        RowCol entrance, exit;
        ParseInput(lines, walkingMap, portalMap, entrance, exit);

        RowCol limit = { lines.size(), lines[0].size() };
        auto dMap = BuildDistanceMap(portalMap, walkingMap, entrance, exit, limit);

        return FindShortestRecursivePath(dMap, portalMap, entrance, exit, limit);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "         A         ",
            "         A         ",
            "  #######.#########",
            "  #######.........#",
            "  #######.#######.#",
            "  #######.#######.#",
            "  #######.#######.#",
            "  #####  B    ###.#",
            "BC...##  C    ###.#",
            "  ##.##       ###.#",
            "  ##...DE  F  ###.#",
            "  #####    G  ###.#",
            "  #########.#####.#",
            "DE..#######...###.#",
            "  #.#########.###.#",
            "FG..#########.....#",
            "  ###########.#####",
            "             Z     ",
            "             Z     ",
        };

        if (Part1(lines) != 23) return false;

        lines = {
            "                   A               ",
            "                   A               ",
            "  #################.#############  ",
            "  #.#...#...................#.#.#  ",
            "  #.#.#.###.###.###.#########.#.#  ",
            "  #.#.#.......#...#.....#.#.#...#  ",
            "  #.#########.###.#####.#.#.###.#  ",
            "  #.............#.#.....#.......#  ",
            "  ###.###########.###.#####.#.#.#  ",
            "  #.....#        A   C    #.#.#.#  ",
            "  #######        S   P    #####.#  ",
            "  #.#...#                 #......VT",
            "  #.#.#.#                 #.#####  ",
            "  #...#.#               YN....#.#  ",
            "  #.###.#                 #####.#  ",
            "DI....#.#                 #.....#  ",
            "  #####.#                 #.###.#  ",
            "ZZ......#               QG....#..AS",
            "  ###.###                 #######  ",
            "JO..#.#.#                 #.....#  ",
            "  #.#.#.#                 ###.#.#  ",
            "  #...#..DI             BU....#..LF",
            "  #####.#                 #.#####  ",
            "YN......#               VT..#....QG",
            "  #.###.#                 #.###.#  ",
            "  #.#...#                 #.....#  ",
            "  ###.###    J L     J    #.#.###  ",
            "  #.....#    O F     P    #.#...#  ",
            "  #.###.#####.#.#####.#####.###.#  ",
            "  #...#.#.#...#.....#.....#.#...#  ",
            "  #.#####.###.###.#.#.#########.#  ",
            "  #...#.#.....#...#.#.#.#.....#.#  ",
            "  #.###.#####.###.###.#.#.#######  ",
            "  #.#.........#...#.............#  ",
            "  #########.###.###.#############  ",
            "           B   J   C               ",
            "           U   P   P               ",
        };

        if (Part1(lines) != 58) return false;

        lines = {
    "             Z L X W       C                 ",
    "             Z P Q B       K                 ",
    "  ###########.#.#.#.#######.###############  ",
    "  #...#.......#.#.......#.#.......#.#.#...#  ",
    "  ###.#.#.#.#.#.#.#.###.#.#.#######.#.#.###  ",
    "  #.#...#.#.#...#.#.#...#...#...#.#.......#  ",
    "  #.###.#######.###.###.#.###.###.#.#######  ",
    "  #...#.......#.#...#...#.............#...#  ",
    "  #.#########.#######.#.#######.#######.###  ",
    "  #...#.#    F       R I       Z    #.#.#.#  ",
    "  #.###.#    D       E C       H    #.#.#.#  ",
    "  #.#...#                           #...#.#  ",
    "  #.###.#                           #.###.#  ",
    "  #.#....OA                       WB..#.#..ZH",
    "  #.###.#                           #.#.#.#  ",
    "CJ......#                           #.....#  ",
    "  #######                           #######  ",
    "  #.#....CK                         #......IC",
    "  #.###.#                           #.###.#  ",
    "  #.....#                           #...#.#  ",
    "  ###.###                           #.#.#.#  ",
    "XF....#.#                         RF..#.#.#  ",
    "  #####.#                           #######  ",
    "  #......CJ                       NM..#...#  ",
    "  ###.#.#                           #.###.#  ",
    "RE....#.#                           #......RF",
    "  ###.###        X   X       L      #.#.#.#  ",
    "  #.....#        F   Q       P      #.#.#.#  ",
    "  ###.###########.###.#######.#########.###  ",
    "  #.....#...#.....#.......#...#.....#.#...#  ",
    "  #####.#.###.#######.#######.###.###.#.#.#  ",
    "  #.......#.......#.#.#.#.#...#...#...#.#.#  ",
    "  #####.###.#####.#.#.#.#.###.###.#.###.###  ",
    "  #.......#.....#.#...#...............#...#  ",
    "  #############.#.#.###.###################  ",
    "               A O F   N                     ",
    "               A A D   M                     "
        };

        if (Part2(lines) != 396) return false;
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