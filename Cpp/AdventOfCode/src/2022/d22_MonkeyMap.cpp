#include "2022/d22_MonkeyMap.h"

SOLUTION(2022, 22) {

    enum struct State { Open, Closed, None };
    enum Facing { Right, Down, Left, Up };
    enum struct CubeFace { Up, Down, Left, Right, Back, Front };

    using Map = std::vector<std::vector<State>>;

    constexpr std::string ReadPath(const auto& lines) {
        return std::string(lines[lines.size() - 1]);
    }

    constexpr void NormalizeLines(std::vector<std::string>&lines) {
        size_t longestLine = 0;

        //Skip the empty line and the path line
        for (auto i = 0; i < lines.size() - 2; i++) {
            longestLine = std::max(longestLine, lines[i].size());
        }

        for (auto i = 0; i < lines.size() - 2; i++) {
            lines[i] = lines[i].append(longestLine - lines[i].size(), ' ');
        }
    }

    constexpr Map ParseMap(const std::vector<std::string>&lines) {
        Map result;

        auto normalized = lines;
        NormalizeLines(normalized);
        for (const auto& line : normalized) {
            if (line.empty()) break;

            std::vector<State> currentLine;
            for (auto c : line) {
                currentLine.push_back(c == ' ' ? State::None : c == '.' ? State::Open : State::Closed);
            }
            result.push_back(currentLine);
        }

        return result;
    }

    using Bounds = std::vector<std::pair<size_t, size_t>>;

    constexpr Bounds GetYBounds(const Map & map) {
        Bounds result;

        for (auto row : map) {
            std::pair<size_t, size_t> rowIndexes;
            bool firstFound = false;
            bool lastFound = false;
            size_t first = 0;
            for (size_t col = 0; col < row.size(); col++) {
                if (row[col] != State::None && !firstFound) {
                    first = col;
                    firstFound = true;
                }
                else if (row[col] == State::None && firstFound) {
                    rowIndexes = std::make_pair(first, col - 1);
                    lastFound = true;
                    break;
                }
            }

            if (!lastFound) {
                rowIndexes = std::make_pair(first, row.size() - 1);
            }
            result.push_back(rowIndexes);
        }

        return result;
    }

    constexpr Bounds GetXBounds(const Map & map) {
        Bounds result;

        for (size_t col = 0; col < map[0].size(); col++) {
            std::pair<size_t, size_t> colIndexes;
            bool firstFound = false;
            bool lastFound = false;
            size_t first = 0;
            for (size_t row = 0; row < map.size(); row++) {
                auto state = map[row][col];
                if (state != State::None && !firstFound) {
                    first = row;
                    firstFound = true;
                }
                else if (state == State::None && firstFound) {
                    colIndexes = std::make_pair(first, row - 1);
                    lastFound = true;
                    break;
                }
            }
            if (!lastFound) {
                colIndexes = std::make_pair(first, map.size() - 1);
            }
            result.push_back(colIndexes);
        }

        return result;
    }

    constexpr std::vector<std::string> SplitPath(const std::string & path) {
        std::vector<std::string> result;
        std::string number = "";
        for (auto c : path) {
            if (c == 'R' || c == 'L') {
                result.push_back(number);
                result.push_back({ c });
                number = "";
            }
            else {
                number += c;
            }
        }

        result.push_back(number);
        return result;
    }

    /*
    RowCol FindStart(const Map& map, const Bounds& xBounds, const Bounds& yBounds) {
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = yBounds[row].first; col <= yBounds[row].second; col++) {
                if (map[row][col] == State::Open) {
                    return { row, col };
                }
            }
        }
        return {0, 0};
    }
    */

    constexpr RowCol FindStart(const Map & map, const Bounds & yBounds) {
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = yBounds[row].first; col <= yBounds[row].second; col++) {
                if (map[row][col] == State::Open) {
                    return { row, col };
                }
            }
        }
        return { 0, 0 };
    }

    constexpr void TravelUp(RowCol & pos, u32 distance, const Map & map, const Bounds & x) {
        for (u32 moves = 0; moves < distance; moves++) {
            if (x[pos.Col].first > pos.Row) {
                throw std::logic_error("We've moved off the map...");
            }

            if (x[pos.Col].first == pos.Row) {
                if (map[x[pos.Col].second][pos.Col] == State::Closed) {
                    break;
                }
                else {
                    pos.Row = x[pos.Col].second;
                }
            }
            else {
                if (map[pos.Row - 1][pos.Col] == State::Closed) {
                    break;
                }
                else {
                    pos.Row--;
                }
            }
        }
    }

    constexpr void TravelDown(RowCol & pos, u32 distance, const Map & map, const Bounds & x) {
        for (u32 moves = 0; moves < distance; moves++) {
            if (x[pos.Col].second < pos.Row) {
                throw std::logic_error("We've moved off the map...");
            }
            if (x[pos.Col].second == pos.Row) {
                if (map[x[pos.Col].first][pos.Col] == State::Closed) {
                    break;
                }
                else {
                    pos.Row = x[pos.Col].first;
                }
            }
            else {
                if (map[pos.Row + 1][pos.Col] == State::Closed) {
                    break;
                }
                else {
                    pos.Row++;
                }
            }
        }
    }

    constexpr void TravelRight(RowCol & pos, u32 distance, const Map & map, const Bounds & y) {
        for (u32 moves = 0; moves < distance; moves++) {
            if (y[pos.Row].second < pos.Col) {
                throw std::logic_error("We've moved off the map...");
            }

            if (y[pos.Row].second == pos.Col) {
                //wrap
                if (map[pos.Row][y[pos.Row].first] == State::Closed) {
                    break; //wall
                }
                else {
                    pos.Col = y[pos.Row].first;
                }
            }
            else {
                if (map[pos.Row][pos.Col + 1] == State::Closed) {
                    break; //wall
                }
                else {
                    pos.Col++;
                }
            }
        }
    }

    constexpr void TravelLeft(RowCol & pos, u32 distance, const Map & map, const Bounds & y) {
        for (u32 moves = 0; moves < distance; moves++) {
            if (y[pos.Row].first > pos.Col) {
                throw std::logic_error("We've moved off the map...");
            }

            if (y[pos.Row].first == pos.Col) {
                //wrap
                if (map[pos.Row][y[pos.Row].second] == State::Closed) {
                    break; //wall
                }
                else {
                    pos.Col = y[pos.Row].second;
                }
            }
            else {
                if (map[pos.Row][pos.Col - 1] == State::Closed) {
                    break;
                }
                else {
                    pos.Col--;
                }
            }
        }
    }

    constexpr void Travel(RowCol & pos, u32 distance, Facing facing, const Map & map, const Bounds & x, const Bounds & y) {
        switch (facing) {
        case Facing::Up: TravelUp(pos, distance, map, x); break;
        case Facing::Down: TravelDown(pos, distance, map, x); break;
        case Facing::Left: TravelLeft(pos, distance, map, y); break;
        case Facing::Right: TravelRight(pos, distance, map, y); break;
        }
    }

    struct CubeSide {
        std::vector<std::vector<bool>> Walls;
        RowCol Tl;
        Constexpr::SmallMap<Facing, std::pair<CubeFace, Facing>> Transforms;
    };

    struct Cube {
        Constexpr::SmallMap<CubeFace, CubeSide> Faces;
        size_t FaceSize = 0;
    };

    constexpr void AddWalls(Cube & cube, const auto& lines) {
        for (auto& [_, face] : cube.Faces) {
            for (size_t row = 0; row < cube.FaceSize; row++) {
                std::vector<bool> wallRow;
                for (size_t col = 0; col < cube.FaceSize; col++) {
                    wallRow.push_back(lines[row + face.Tl.Row][col + face.Tl.Col] == '#');
                }
                face.Walls.push_back(wallRow);
            }
        }
    }

    constexpr Cube MakeTestCube(const std::vector<std::string>&lines) {
        /*
        B
    D L U
        F R
        */
        Cube result;
        result.FaceSize = 4;
        CubeSide up;
        CubeSide down;
        CubeSide left;
        CubeSide right;
        CubeSide front;
        CubeSide back;

        back.Tl = { 0, 8 };
        down.Tl = { 4, 0 };
        left.Tl = { 4, 4 };
        up.Tl = { 4, 8 };
        front.Tl = { 8, 8 };
        right.Tl = { 8, 12 };

        up.Transforms[Up] = { CubeFace::Back, Up };
        up.Transforms[Down] = { CubeFace::Front, Down };
        up.Transforms[Left] = { CubeFace::Left, Left };
        up.Transforms[Right] = { CubeFace::Right, Down };

        down.Transforms[Up] = { CubeFace::Back, Down };
        down.Transforms[Down] = { CubeFace::Front, Up };
        down.Transforms[Left] = { CubeFace::Right, Up };
        down.Transforms[Right] = { CubeFace::Left, Right };

        left.Transforms[Up] = { CubeFace::Back, Right };
        left.Transforms[Down] = { CubeFace::Front, Right };
        left.Transforms[Left] = { CubeFace::Down, Left };
        left.Transforms[Right] = { CubeFace::Up, Right };

        right.Transforms[Up] = { CubeFace::Up, Left };
        right.Transforms[Down] = { CubeFace::Down, Right };
        right.Transforms[Left] = { CubeFace::Front, Left };
        right.Transforms[Right] = { CubeFace::Back, Left };

        front.Transforms[Up] = { CubeFace::Up, Up };
        front.Transforms[Down] = { CubeFace::Down, Up };
        front.Transforms[Left] = { CubeFace::Left, Up };
        front.Transforms[Right] = { CubeFace::Right, Right };

        back.Transforms[Up] = { CubeFace::Down, Down };
        back.Transforms[Down] = { CubeFace::Up, Down };
        back.Transforms[Left] = { CubeFace::Left, Down };
        back.Transforms[Right] = { CubeFace::Right, Left };

        result.Faces[CubeFace::Up] = up;
        result.Faces[CubeFace::Down] = down;
        result.Faces[CubeFace::Left] = left;
        result.Faces[CubeFace::Right] = right;
        result.Faces[CubeFace::Front] = front;
        result.Faces[CubeFace::Back] = back;

        AddWalls(result, lines);
        return result;

    }
    constexpr Cube MakeCube(const auto& lines) {
        /*
          B R
          U
        L F
        D
        */

        Cube result;
        result.FaceSize = 50;
        CubeSide up;
        CubeSide down;
        CubeSide left;
        CubeSide right;
        CubeSide front;
        CubeSide back;

        back.Tl = { 0, 50 };
        right.Tl = { 0, 100 };
        up.Tl = { 50, 50 };
        left.Tl = { 100, 0 };
        front.Tl = { 100, 50 };
        down.Tl = { 150, 0 };

        up.Transforms[Up] = { CubeFace::Back, Up };
        up.Transforms[Down] = { CubeFace::Front, Down };
        up.Transforms[Left] = { CubeFace::Left, Down };
        up.Transforms[Right] = { CubeFace::Right, Up };

        down.Transforms[Up] = { CubeFace::Left, Up };
        down.Transforms[Down] = { CubeFace::Right, Down };
        down.Transforms[Left] = { CubeFace::Back, Down };
        down.Transforms[Right] = { CubeFace::Front, Up };

        left.Transforms[Up] = { CubeFace::Up, Right };
        left.Transforms[Down] = { CubeFace::Down, Down };
        left.Transforms[Left] = { CubeFace::Back, Right };
        left.Transforms[Right] = { CubeFace::Front, Right };

        right.Transforms[Up] = { CubeFace::Down, Up };
        right.Transforms[Down] = { CubeFace::Up, Left };
        right.Transforms[Left] = { CubeFace::Back, Left };
        right.Transforms[Right] = { CubeFace::Front, Left };

        front.Transforms[Up] = { CubeFace::Up, Up };
        front.Transforms[Down] = { CubeFace::Down, Left };
        front.Transforms[Left] = { CubeFace::Left, Left };
        front.Transforms[Right] = { CubeFace::Right, Left };

        back.Transforms[Up] = { CubeFace::Down, Right };
        back.Transforms[Down] = { CubeFace::Up, Down };
        back.Transforms[Left] = { CubeFace::Left, Right };
        back.Transforms[Right] = { CubeFace::Right, Right };

        result.Faces[CubeFace::Up] = up;
        result.Faces[CubeFace::Down] = down;
        result.Faces[CubeFace::Left] = left;
        result.Faces[CubeFace::Right] = right;
        result.Faces[CubeFace::Front] = front;
        result.Faces[CubeFace::Back] = back;

        AddWalls(result, lines);
        return result;
    }

    constexpr void ChangeFace(RowCol & pos, Facing & currentFacing, CubeFace & currentFace, const Cube & cube) {
        auto [nextFace, nextFacing] = cube.Faces.at(currentFace).Transforms.at(currentFacing);
        auto nextPos = pos;

        switch (nextFacing) {
        case Up: nextPos.Row = cube.FaceSize - 1; break;
        case Down: nextPos.Row = 0; break;
        case Left: nextPos.Col = cube.FaceSize - 1; break;
        case Right: nextPos.Col = 0; break;
        default: break;
        }

        if (currentFacing != nextFacing) {
            if (currentFacing == Left && nextFacing == Right) {
                nextPos.Row = cube.FaceSize - pos.Row - 1;
            }
            else if (currentFacing == Right && nextFacing == Left) {
                nextPos.Row = cube.FaceSize - pos.Row - 1;
            }
            else if (currentFacing == Up && nextFacing == Down) {
                nextPos.Col = cube.FaceSize - pos.Col - 1;
            }
            else if (currentFacing == Down && nextFacing == Up) {
                nextPos.Col = cube.FaceSize - pos.Col - 1;
            }
            else if (currentFacing == Up && nextFacing == Right) {
                nextPos.Row = pos.Col;
            }
            else if (currentFacing == Up && nextFacing == Left) {
                nextPos.Row = cube.FaceSize - pos.Col - 1;
            }
            else if (currentFacing == Down && nextFacing == Left) {
                nextPos.Row = pos.Col;
            }
            else if (currentFacing == Down && nextFacing == Right) {
                nextPos.Row = cube.FaceSize - pos.Col - 1;
            }
            else if (currentFacing == Right && nextFacing == Up) {
                nextPos.Col = pos.Row;
            }
            else if (currentFacing == Right && nextFacing == Down) {
                nextPos.Col = cube.FaceSize - pos.Row - 1;
            }
            else if (currentFacing == Left && nextFacing == Down) {
                nextPos.Col = pos.Row;
            }
            else if (currentFacing == Left && nextFacing == Up) {
                nextPos.Col = cube.FaceSize - pos.Row - 1;
            }
            else {
                throw std::logic_error("Missed one");
            }
        }

        pos = nextPos;
        currentFacing = nextFacing;
        currentFace = nextFace;
    }

    constexpr void Travel(const Cube & cube, u32 distance, RowCol & pos, Facing & facing, CubeFace & currentFace) {
        RowCol nextPos = pos;
        Facing nextFacing = facing;
        CubeFace nextFace = currentFace;

        for (u32 moves = 0; moves < distance; moves++) {
            bool changingFace = (pos.Row == 0 && facing == Up) ||
                (pos.Row == cube.FaceSize - 1 && facing == Down) ||
                (pos.Col == 0 && facing == Left) ||
                (pos.Col == cube.FaceSize - 1 && facing == Right);

            if (changingFace) {
                ChangeFace(nextPos, nextFacing, nextFace, cube);
            }
            else {
                switch (facing) {
                case Up: nextPos.Row--; break;
                case Down: nextPos.Row++; break;
                case Left: nextPos.Col--; break;
                case Right: nextPos.Col++; break;
                }
            }

            if (cube.Faces.at(nextFace).Walls[nextPos.Row][nextPos.Col]) {
                break;
            }
            else {
                facing = nextFacing;
                currentFace = nextFace;
                pos = nextPos;
            }
        }
    }

    constexpr u32 GetResult(const RowCol& pos, Facing facing) {
        return static_cast<u32>(((pos.Row + 1) * 1000) + ((pos.Col + 1) * 4) + static_cast<u32>(facing));
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        const auto facingValues = 4;
        auto map = ParseMap(lines);
        auto x = GetXBounds(map);
        auto y = GetYBounds(map);
        auto path = SplitPath(ReadPath(lines));
        auto currentPos = FindStart(map, y);
        auto facing = Facing::Right;
        u32 distance;

        u32 commandsProcessed = 0;

        for (auto cmd : path) {
            if (cmd[0] == 'R') {
                facing = static_cast<Facing>((facing + 1) % facingValues);
            }
            else if (cmd[0] == 'L') {
                facing = static_cast<Facing>((facingValues + facing - 1) % facingValues);
            }
            else {
                Constexpr::ParseNumber(cmd, distance);
                Travel(currentPos, distance, facing, map, x, y);
            }

            commandsProcessed++;
        }

        return GetResult(currentPos, facing);
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(CopyToVector(lines)));
    }

    PART_TWO() {
        const auto facingValues = 4;
        auto cube = MakeCube(lines);
        //auto cube = MakeTestCube(lines);
        auto currentFace = CubeFace::Back;
        auto pos = RowCol{ 0, 0 };
        auto facing = Right;
        auto path = SplitPath(ReadPath(lines));
        u32 distance;

        for (auto cmd : path) {
            if (cmd[0] == 'R') {
                facing = static_cast<Facing>((facing + 1) % facingValues);
            }
            else if (cmd[0] == 'L') {
                facing = static_cast<Facing>((facingValues + facing - 1) % facingValues);
            }
            else {
                Constexpr::ParseNumber(cmd, distance);
                Travel(cube, distance, pos, facing, currentFace);
            }
        }

        auto finalFace = cube.Faces.at(currentFace);
        RowCol resultPos = { pos.Row + finalFace.Tl.Row, pos.Col + finalFace.Tl.Col };

        return Constexpr::ToString(GetResult(resultPos, facing));
    }

    TESTS() {
        static_assert(SplitPath("10R5L5R10L4R5L5") == std::vector<std::string>{"10", "R", "5", "L", "5", "R", "10", "L", "4", "R", "5", "L", "5"});
        
        std::vector<std::string> lines = {
            "        ...#",
            "        .#..",
            "        #...",
            "        ....",
            "...#.......#",
            "........#...",
            "..#....#....",
            "..........#.",
            "        ...#....",
            "        .....#..",
            "        .#......",
            "        ......#.",
            "",
            "10R5L5R10L4R5L5"
        };

        if (SolvePartOne(lines) != 6032) return false;
        //if (PartTwo(lines) != "5031") return false;

        return true;
    }
}