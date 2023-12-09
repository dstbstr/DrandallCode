#include "2020/d24_Lobby.h"
#include "HexGrid.h"

SOLUTION(2020, 24) {

    constexpr std::vector<PointyHex::Direction> GetDirections(std::string_view line) {
        using namespace PointyHex;
        std::vector<Direction> result;
        for (auto i = 0; i < line.size(); i++) {
            auto c = line[i];
            if (c == 'e') result.push_back(Direction::E);
            else if (c == 'w') result.push_back(Direction::W);
            else {
                auto c2 = line[++i];
                if (c == 'n') {
                    if (c2 == 'e') result.push_back(Direction::NE);
                    else result.push_back(Direction::NW);
                }
                else {
                    if (c2 == 'e') result.push_back(Direction::SE);
                    else result.push_back(Direction::SW);
                }
            }
        }
        return result;
    }
    using Grid = Constexpr::SmallSet<PointyHex::Pos>;

    constexpr Grid GetInitialGrid(const auto& lines) {
        Grid result{};
        const PointyHex::Pos origin = { 0, 0, 0 };

        for (const auto& line : lines) {
            auto directions = GetDirections(line);
            auto pos = origin;
            for (auto dir : directions) {
                Move(pos, dir);
            }
            if (result.contains(pos)) {
                result.erase(pos);
            }
            else {
                result.insert(pos);
            }
        }

        return result;
    }

    constexpr void Next(Grid& grid) {
        Constexpr::SmallMap<PointyHex::Pos, u8> neighborCounts;
        for (auto pos : grid) {
            auto neighbors = GetAllNeighbors(pos);
            for (auto n : neighbors) {
                neighborCounts[n]++;
            }
        }
        Grid next;
        for (auto& [pos, count] : neighborCounts) {
            if (count == 2 || (count == 1 && grid.contains(pos))) {
                next.insert(pos);
            }
        }
        grid = next;
    }

    constexpr size_t Run(const auto& lines, size_t days) {
        auto grid = GetInitialGrid(lines);
        for (auto i = 0; i < days; i++) {
            Next(grid);
        }
        return grid.size();
    }

    PART_ONE() {
        return Constexpr::ToString(Run(lines, 0));
    }

    PART_TWO() {
        return Constexpr::ToString(Run(lines, 100));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "sesenwnenenewseeswwswswwnenewsewsw",
            "neeenesenwnwwswnenewnwwsewnenwseswesw",
            "seswneswswsenwwnwse",
            "nwnwneseeswswnenewneswwnewseswneseene",
            "swweswneswnenwsewnwneneseenw",
            "eesenwseswswnenwswnwnwsewwnwsene",
            "sewnenenenesenwsewnenwwwse",
            "wenwwweseeeweswwwnwwe",
            "wsweesenenewnwwnwsenewsenwwsesesenwne",
            "neeswseenwwswnwswswnw",
            "nenwswwsewswnenenewsenwsenwnesesenew",
            "enewnwewneswsewnwswenweswnenwsenwsw",
            "sweneswneswneneenwnewenewwneswswnese",
            "swwesenesewenwneswnwwneseswwne",
            "enesenwswwswneneswsenwnewswseenwsese",
            "wnwnesenesenenwwnenwsewesewsesesew",
            "nenewswnwewswnenesenwnesewesw",
            "eneswnwswnwsenenwnwnwwseeswneewsenese",
            "neswnwewnwnwseenwseesewsenwsweewe",
            "wseweeenwnesenwwwswnew"
        };

        if (Run(lines, 0) != 10) return false;
        if (Run(lines, 1) != 15) return false;
        if (Run(lines, 100) != 2208) return false;
        return true;
    }
}