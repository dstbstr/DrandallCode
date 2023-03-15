#include "AStar.h"
#include "Coord.h"

namespace AStarTests {

    bool RunTests() {
        Coord n1 = { 10, 0 };
        Coord n2 = { 2, 3 };
        Coord n3 = { 8, 20 };
        Coord n4 = { 10, 10 };

        auto heuristic = [&](const Coord& coord) { return MDistance(coord, n4); };
        auto complete = [&](const Coord& coord) { return coord == n4; };

        auto neighborFunc = [&](const Coord& node) -> std::vector<Coord> {
            if (node == n1) return { n2, n3 };
            if (node == n2) return { n1, n4 };
            if (node == n3) return { n1, n4 };
            if (node == n4) return { n2, n3 };
            return {};
        };
        auto costFunc = [](const Coord& lhs, const Coord& rhs) -> u32 {
            return MDistance(lhs, rhs);
        };

        auto path = AStarMin<Coord, CoordHash>(n1, costFunc, complete, heuristic, neighborFunc);

        if (path.size() != 3) return false;
        if (path[0] != n1) return false;
        if (path[2] != n4) return false;
        if (path[1] != n2) return false;

        return true;
    }
}