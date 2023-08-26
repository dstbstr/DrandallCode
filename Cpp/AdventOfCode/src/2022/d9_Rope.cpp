#include "2022/d9_Rope.h"
#include "Facing.h"

SOLUTION(2022, 9) {

    constexpr void MoveTail(const Coord & head, Coord & tail) {
        const auto dx = head.X - tail.X;
        const auto dy = head.Y - tail.Y;
        if (dx == 0 && dy == 0) return;

        if (dx == 2) {
            tail.X++; //right
            if (dy < 0) tail.Y--; //UpRight
            else if (dy > 0) tail.Y++; //DownRight
        }
        else if (dx == -2) {
            tail.X--; //Left
            if (dy < 0) tail.Y--; //UpLeft
            else if (dy > 0) tail.Y++; //DownLeft
        }
        else if (dy == -2) {
            tail.Y--; //Up
            if (dx > 0) tail.X++; //UpRight
            else if (dx < 0) tail.X--; //UpLeft
        }
        else if (dy == 2) {
            tail.Y++; //Down
            if (dx > 0) tail.X++; //DownRight
            else if (dx < 0) tail.X--; //DownLeft
        }
    }

    constexpr size_t CountUniqueTailPositions(const auto& lines, size_t knotCount) {
        Constexpr::BigSet<Coord, 50'000> seen;
        seen.SetSentinel({ 9999, 9999 });

        std::vector<Coord> knots;
        for (auto i = 0; i < knotCount; i++) {
            knots.push_back({ 0, 0 });
        }

        for (const auto& line : lines) {
            auto dir = ParseFacing(line[0]);
            u32 count;
            Constexpr::ParseNumber(line.substr(2), count);
            for (u32 i = 0; i < count; i++) {
                Move(knots[0], dir);
                for (size_t knot = 0; knot < knots.size() - 1; knot++) {
                    MoveTail(knots[knot], knots[knot + 1]);
                }
                seen.insert(knots.back());
            }
        }

        return seen.size();
    }

    PART_ONE() {
        return Constexpr::ToString(CountUniqueTailPositions(Lines, 2));
    }

    PART_TWO() {
        return Constexpr::ToString(CountUniqueTailPositions(Lines, 10));
    }

    constexpr bool TestMoveTail(Coord head, Coord initialTail, Coord expectedTail) {
        MoveTail(head, initialTail);
        return initialTail == expectedTail;
    }

    TESTS() {
        static_assert(TestMoveTail({ 0, 0 }, { 0, 0 }, { 0, 0 }));

        static_assert(TestMoveTail({ 1, 0 }, { 0, 0 }, { 0, 0 }));
        static_assert(TestMoveTail({ 0, 1 }, { 0, 0 }, { 0, 0 }));
        static_assert(TestMoveTail({ 0, 0 }, { 1, 0 }, { 1, 0 }));
        static_assert(TestMoveTail({ 0, 0 }, { 0, 1 }, { 0, 1 }));

        static_assert(TestMoveTail({ 1, 1 }, { 0, 0 }, { 0, 0 }));
        static_assert(TestMoveTail({ 0, 0 }, { 1, 1 }, { 1, 1 }));
        static_assert(TestMoveTail({ 1, 0 }, { 0, 1 }, { 0, 1 }));
        static_assert(TestMoveTail({ 0, 1 }, { 1, 0 }, { 1, 0 }));

        static_assert(TestMoveTail({ 2, 0 }, { 0, 0 }, { 1, 0 }));
        static_assert(TestMoveTail({ 0, 2 }, { 0, 0 }, { 0, 1 }));
        static_assert(TestMoveTail({ 0, 0 }, { 2, 0 }, { 1, 0 }));
        static_assert(TestMoveTail({ 0, 0 }, { 0, 2 }, { 0, 1 }));

        static_assert(TestMoveTail({ 2, 1 }, { 0, 0 }, { 1, 1 }));
        static_assert(TestMoveTail({ 1, 2 }, { 0, 0 }, { 1, 1 }));
        static_assert(TestMoveTail({ 0, 0 }, { 2, 1 }, { 1, 0 }));
        static_assert(TestMoveTail({ 0, 0 }, { 1, 2 }, { 0, 1 }));

        static_assert(TestMoveTail({ 2, 2 }, { 0, 0 }, { 1, 1 }));

        std::vector<std::string> lines = {
            "R 4",
            "U 4",
            "L 3",
            "D 1",
            "R 4",
            "D 1",
            "L 5",
            "R 2"
        };
        if (CountUniqueTailPositions(lines, 2) != 13) return false;

        lines = {
            "R 5",
            "U 8",
            "L 8",
            "D 3",
            "R 17",
            "D 10",
            "L 25",
            "U 20"
        };
        if (CountUniqueTailPositions(lines, 10) != 36) return false;
        
        return true;
    }
}