#include "2022/d9_Rope.h"
#include <sstream>

SOLUTION(2022, 9) {
    enum struct Direction { Up, Down, Left, Right };

    constexpr void MoveTail(const Coord & head, Coord & tail) {
        const auto dx = head.X - tail.X;
        const auto dy = head.Y - tail.Y;
        if (dx == 0 && dy == 0) return;

        if (dx == 2) {
            tail.X++; //right
            if (dy > 0) tail.Y++; //UpRight
            else if (dy < 0) tail.Y--; //DownRight
        }
        else if (dx == -2) {
            tail.X--; //Left
            if (dy > 0) tail.Y++; //UpLeft
            else if (dy < 0) tail.Y--; //DownLeft
        }
        else if (dy == 2) {
            tail.Y++; //Up
            if (dx > 0) tail.X++; //UpRight
            else if (dx < 0) tail.X--; //UpLeft
        }
        else if (dy == -2) {
            tail.Y--; //Down
            if (dx > 0) tail.X++; //DownRight
            else if (dx < 0) tail.X--; //DownLeft
        }
    }

    constexpr bool TestMoveTail(Coord head, Coord initialTail, Coord expectedTail) {
        MoveTail(head, initialTail);
        return initialTail == expectedTail;
    }

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

    constexpr void Move(Coord & head, Direction dir) {
        switch (dir) {
        case Direction::Up: head.Y++; break;
        case Direction::Down: head.Y--; break;
        case Direction::Left: head.X--; break;
        case Direction::Right: head.X++; break;
        }
    }

    void PrintKnots(const std::vector<Coord>&knots, u32 xSize, u32 ySize) {
        std::stringstream stream;
        for (u32 y = 0; y < ySize; y++) {
            for (u32 x = 0; x < xSize; x++) {
                stream << ".";
            }
        }

        std::cout << "\n\n";
        auto str = stream.str();
        for (auto index = 0; index < knots.size(); index++) {
            const auto& knot = knots[index];
            auto pos = knot.Y * ySize + knot.X;
            if (index == 0) {
                str[pos] = 'H';
            }
            else if (str[pos] == '.') {
                str[pos] = static_cast<char>('0' + index);
            }
        }
        if (str[0] == '.') {
            str[0] = 's';
        }

        for (s32 y = ySize - 1; y >= 0; y--) {
            for (u32 x = 0; x < xSize; x++) {
                auto pos = y * ySize + x;
                std::cout << str[pos];
            }
            std::cout << "\n";
        }
    }

    u32 CountUniqueTailPositions(const std::vector<std::string>&lines, size_t knotCount) {
        std::unordered_set<Coord> seen;
        std::vector<Coord> knots;
        for (auto i = 0; i < knotCount; i++) {
            knots.push_back({ 0, 0 });
        }

        //PrintKnots(knots, 6, 5);
        for (const auto& line : lines) {
            Direction dir = Direction::Up;
            if (line[0] == 'U') dir = Direction::Up;
            else if (line[0] == 'D') dir = Direction::Down;
            else if (line[0] == 'L') dir = Direction::Left;
            else if (line[0] == 'R') dir = Direction::Right;

            u32 count;
            Constexpr::ParseNumber(line.substr(2), count);
            for (u32 i = 0; i < count; i++) {
                Move(knots[0], dir);
                for (size_t knot = 0; knot < knots.size() - 1; knot++) {
                    MoveTail(knots[knot], knots[knot + 1]);
                }
                seen.insert(knots[knots.size() - 1]);
                //PrintKnots(knots, 6, 5);
            }
        }

        return static_cast<u32>(seen.size());
    }

    std::string Run(const std::vector<std::string>&lines) {
        return Constexpr::ToString(CountUniqueTailPositions(lines, 10));
    }

    bool RunTests() {
        /*
        if(!TestMoveTail({ 0, 0 }, { 0, 0 }, { 0, 0 })) return false;

        if(!TestMoveTail({ 1, 0 }, { 0, 0 }, { 0, 0 })) return false;
        if(!TestMoveTail({ 0, 1 }, { 0, 0 }, { 0, 0 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 1, 0 }, { 1, 0 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 0, 1 }, { 0, 1 })) return false;

        if(!TestMoveTail({ 1, 1 }, { 0, 0 }, { 0, 0 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 1, 1 }, { 1, 1 })) return false;
        if(!TestMoveTail({ 1, 0 }, { 0, 1 }, { 0, 1 })) return false;
        if(!TestMoveTail({ 0, 1 }, { 1, 0 }, { 1, 0 })) return false;

        if(!TestMoveTail({ 2, 0 }, { 0, 0 }, { 1, 0 })) return false;
        if(!TestMoveTail({ 0, 2 }, { 0, 0 }, { 0, 1 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 2, 0 }, { 1, 0 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 0, 2 }, { 0, 1 })) return false;

        if(!TestMoveTail({ 2, 1 }, { 0, 0 }, { 1, 1 })) return false;
        if(!TestMoveTail({ 1, 2 }, { 0, 0 }, { 1, 1 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 2, 1 }, { 1, 0 })) return false;
        if(!TestMoveTail({ 0, 0 }, { 1, 2 }, { 0, 1 })) return false;
        */

        /*
        std::vector<std::string> lines;
        lines.push_back("R 4");
        lines.push_back("U 4");
        lines.push_back("L 3");
        lines.push_back("D 1");
        lines.push_back("R 4");
        lines.push_back("D 1");
        lines.push_back("L 5");
        lines.push_back("R 2");

        if (CountUniqueTailPositions(lines, 10) != 1) return false;
        */

        std::vector<std::string> lines;
        lines.push_back("R 5");
        lines.push_back("U 8");
        lines.push_back("L 8");
        lines.push_back("D 3");
        lines.push_back("R 17");
        lines.push_back("D 10");
        lines.push_back("L 25");
        lines.push_back("U 20");

        if (CountUniqueTailPositions(lines, 10) != 36) return false;

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