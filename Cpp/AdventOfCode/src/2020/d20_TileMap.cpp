#include "2020/d20_TileMap.h"
#include "Facing.h"

SOLUTION(2020, 20) {
    using PixelGrid = std::vector<std::vector<bool>>;

    struct Tile {
        size_t Id;
        PixelGrid Pixels;
        std::array<size_t, 4> EdgeCounts{};
    };

    constexpr void SetEdgeCounts(Tile & tile) {
        const auto& pixels = tile.Pixels;
        tile.EdgeCounts[Up] = std::count(pixels[0].begin(), pixels[0].end(), true);
        tile.EdgeCounts[Down] = std::count(pixels.back().begin(), pixels.back().end(), true);
        size_t firstCol = 0;
        size_t lastCol = 0;
        size_t rowSize = pixels[0].size() - 1;
        for (size_t row = 0; row < pixels.size(); row++) {
            firstCol += pixels[row][0];
            lastCol += pixels[row][rowSize];
        }
        tile.EdgeCounts[Left] = firstCol;
        tile.EdgeCounts[Right] = lastCol;
    }

    constexpr Tile ParseTile(const auto& lines) {
        Tile result;
        Constexpr::ParseNumber(lines[0].substr(5, 4), result.Id);
        for (auto row = 1; row < lines.size(); row++) {
            std::vector<bool> currentRow;
            for (auto col = 0; col < lines[row].size(); col++) {
                currentRow.push_back(lines[row][col] == '#');
            }
            result.Pixels.push_back(currentRow);
        }

        SetEdgeCounts(result);
        return result;
    }

    constexpr std::vector<bool> GetEdge(const PixelGrid & pixels, Facing dir) {
        switch (dir) {
        case Up: return pixels[0];
        case Down: return pixels.back();
        }

        std::vector<bool> result;
        if (dir == Left) {
            for (const auto& row : pixels) {
                result.push_back(row[0]);
            }
        }
        else {
            for (const auto& row : pixels) {
                result.push_back(row.back());
            }
        }

        return result;
    }

    constexpr Facing GetOtherEdge(Facing dir) {
        switch (dir) {
        case Down: return Up;
        case Up: return Down;
        case Left: return Right;
        case Right: return Left;
        }
        return dir;
    }

    constexpr bool DoEdgesMatch(const Tile & t1, const Tile & t2, Facing e1, Facing e2) {
        auto lhs = GetEdge(t1.Pixels, e1);
        auto rhs = GetEdge(t2.Pixels, e2);
        if (lhs == rhs) return true;
        std::reverse(lhs.begin(), lhs.end());
        return lhs == rhs;
    }

    constexpr bool DoEdgesMatch(const Tile & t1, const Tile & t2, Facing edge) {
        return DoEdgesMatch(t1, t2, edge, GetOtherEdge(edge));
    }

    constexpr Constexpr::SmallMap<size_t, std::vector<size_t>> FindNeighbors(const Constexpr::SmallMap<size_t, Tile>& tiles) {
        Constexpr::SmallMap<size_t, std::vector<size_t>> result;
        for (const auto& [lId, lTile] : tiles) {
            for (const auto& [rId, rTile] : tiles) {
                if (lId == rId) continue;
                for (size_t i = 0; i < lTile.EdgeCounts.size(); i++) {
                    for (size_t j = 0; j < rTile.EdgeCounts.size(); j++) {
                        if (lTile.EdgeCounts[i] == rTile.EdgeCounts[j]) {
                            auto d1 = static_cast<Facing>(i);
                            auto d2 = static_cast<Facing>(j);
                            if (DoEdgesMatch(lTile, rTile, d1, d2)) {
                                result[lId].push_back(rId);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    constexpr bool DoEdgesMatchExact(const Tile & t1, const Tile & t2, Facing edge) {
        Facing otherEdge = edge;
        switch (edge) {
        case Down: otherEdge = Up; break;
        case Up: otherEdge = Down; break;
        case Left: otherEdge = Right; break;
        case Right: otherEdge = Left; break;
        }

        return GetEdge(t1.Pixels, edge) == GetEdge(t2.Pixels, otherEdge);
    }

    using TileMap = Constexpr::SmallMap<size_t, Tile>;
    using NeighborMap = Constexpr::SmallMap<size_t, std::vector<size_t>>;
    using TileGrid = std::vector<std::vector<Tile>>;

    void PrintTile(const PixelGrid & pixels) {
        for (const auto& row : pixels) {
            for (const auto& col : row) {
                std::cout << (col ? '#' : '.');
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void PrintTileGrid(const TileGrid & tileGrid) {
        const size_t tileHeight = tileGrid.at(0).at(0).Pixels.size();
        const size_t tileWidth = tileGrid.at(0).at(0).Pixels.at(0).size();
        for (auto gridRow = 0; gridRow < tileGrid.size(); gridRow++) {
            for (auto tileRow = 0; tileRow < tileHeight; tileRow++) {
                for (auto gridCol = 0; gridCol < tileGrid.at(gridRow).size(); gridCol++) {
                    for (auto tileCol = 0; tileCol < tileWidth; tileCol++) {
                        auto val = tileGrid.at(gridRow).at(gridCol).Pixels.at(tileRow).at(tileCol);
                        std::cout << (val ? '#' : '.');
                    }
                    std::cout << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }

    constexpr void FitFirstPiece(TileMap & tiles, const NeighborMap & neighbors, TileGrid & tileGrid) {
        auto firstPiece = std::find_if(neighbors.begin(), neighbors.end(), [](const auto& kvp) { return kvp.second.size() == 2; });

        auto& lhs = tiles.at(firstPiece->first);
        auto n = neighbors.at(lhs.Id);
        auto& rhs = tiles.at(n[0]);
        while (!DoEdgesMatch(lhs, rhs, Right)) {
            lhs.Pixels = Constexpr::Rotate(lhs.Pixels);
        }
        if (!DoEdgesMatchExact(lhs, rhs, Right)) {
            rhs.Pixels = Constexpr::FlipX(rhs.Pixels);
        }
        if (!DoEdgesMatchExact(lhs, rhs, Right)) {
            throw "Something went wrong";
        }

        tileGrid.push_back({ lhs, rhs });
    }

    constexpr bool OrientPiece(const Tile & fixedPiece, Tile & toFit, Facing edge) {
        bool found = false;
        for (auto i = 0; i < 4; i++) {
            if (DoEdgesMatch(fixedPiece, toFit, edge)) {
                found = true;
                break;
            }
            toFit.Pixels = Constexpr::Rotate(toFit.Pixels);
        }
        if (!found) return false;
        if (!DoEdgesMatchExact(fixedPiece, toFit, edge)) {
            if (edge == Right) {
                toFit.Pixels = Constexpr::FlipX(toFit.Pixels);
            }
            else {
                toFit.Pixels = Constexpr::FlipY(toFit.Pixels);
            }
        }

        return true;
    }

    constexpr void FitFirstRow(TileMap & tiles, const NeighborMap & neighbors, TileGrid & tileGrid, Constexpr::SmallSet<size_t>&seen) {
        while (true) {
            const Tile& tile = tileGrid[0].back();
            auto n = neighbors.at(tile.Id);

            size_t otherTileId = n[0];
            for (auto nId : n) {
                if (seen.contains(nId)) continue;
                otherTileId = nId;
                if (OrientPiece(tile, tiles.at(otherTileId), Right)) break;
            }

            tileGrid[0].push_back(tiles.at(otherTileId));
            seen.insert(otherTileId);
            if (neighbors.at(otherTileId).size() == 2) break;
        }
    }

    constexpr void FlipFirstRowIfNeeded(TileMap & tiles, const NeighborMap & neighbors, TileGrid & tileGrid) {
        const auto& firstTile = tileGrid.at(0).at(0);
        auto n = neighbors.at(firstTile.Id);
        auto& nextTile = [&]() ->Tile& {
            return tileGrid.at(0).at(1).Id == n[0] ? tiles.at(n[1]) : tiles.at(n[0]);
        }();

        bool matchesUp = false;
        for (auto i = 0; i < 4; i++) {
            if (DoEdgesMatch(firstTile, nextTile, Down)) {
                break;
            }
            if (DoEdgesMatch(firstTile, nextTile, Up)) {
                matchesUp = true;
                break;
            }
            nextTile.Pixels = Constexpr::Rotate(nextTile.Pixels);
        }
        if (matchesUp) {
            for (auto& tile : tileGrid[0]) {
                tile.Pixels = Constexpr::FlipX(tile.Pixels);
                tiles.at(tile.Id).Pixels = Constexpr::FlipX(tiles.at(tile.Id).Pixels);
            }
            nextTile.Pixels = Constexpr::FlipX(nextTile.Pixels);
        }
        if (!DoEdgesMatchExact(firstTile, nextTile, Down)) {
            nextTile.Pixels = Constexpr::FlipY(nextTile.Pixels);
            if (!DoEdgesMatchExact(firstTile, nextTile, Down)) {
                throw "Something went wrong";
            }
        }
    }

    constexpr void FitRemainingRows(TileMap & tiles, const NeighborMap & neighbors, TileGrid & tileGrid, Constexpr::SmallSet<size_t>&seen) {
        auto rowSize = tileGrid[0].size();
        for (auto row = 1; row < rowSize; row++) {
            std::vector<Tile> currentRow;
            size_t upId = tileGrid[row - 1][0].Id;
            auto n = neighbors.at(upId);
            size_t leftId = *std::find_if(n.begin(), n.end(), [&seen](size_t neighbor) { return !seen.contains(neighbor); });
            OrientPiece(tiles.at(upId), tiles.at(leftId), Down);
            currentRow.push_back(tiles.at(leftId));
            seen.insert(leftId);

            for (auto col = 1; col < rowSize; col++) {
                upId = tileGrid[row - 1][col].Id;
                leftId = currentRow.back().Id;

                auto upNeighbors = neighbors.at(upId);
                auto nextId = *std::find_if(upNeighbors.begin(), upNeighbors.end(), [&seen](size_t id) { return !seen.contains(id); });

                OrientPiece(tiles.at(upId), tiles.at(nextId), Down);
                OrientPiece(tiles.at(leftId), tiles.at(nextId), Right);
                currentRow.push_back(tiles.at(nextId));
                seen.insert(nextId);
            }
            tileGrid.push_back(currentRow);
        }
    }

    constexpr PixelGrid CompressImage(const TileGrid & tileGrid) {
        PixelGrid result;
        for (size_t gridRow = 0; gridRow < tileGrid.size(); gridRow++) {
            for (size_t tileRow = 1; tileRow < tileGrid.at(gridRow).at(0).Pixels.size() - 1; tileRow++) {
                std::vector<bool> resultRow;
                for (size_t gridCol = 0; gridCol < tileGrid.at(gridRow).size(); gridCol++) {
                    for (size_t tileCol = 1; tileCol < tileGrid.at(gridRow).at(gridCol).Pixels.at(tileRow).size() - 1; tileCol++) {
                        resultRow.push_back(tileGrid.at(gridRow).at(gridCol).Pixels.at(tileRow).at(tileCol));
                    }
                }
                result.push_back(resultRow);
            }
        }
        return result;
    }

    constexpr PixelGrid FindSeaMonsters(PixelGrid & image) {
        std::vector<RowCol> positions = {
            {0, 18},
            {1, 0}, {1, 5}, {1, 6}, {1, 11}, {1, 12}, {1, 17}, {1, 18}, {1, 19},
            {2, 1}, {2, 4}, {2, 7}, {2, 10}, {2, 13}, {2, 16}
        };

        PixelGrid result = image;
        for (size_t row = 0; row < result.size(); row++) {
            for (size_t col = 0; col < result[row].size(); col++) {
                result[row][col] = true;
            }
        }
        auto markSeaMonster = [&](size_t dX, size_t dY) {
            for (const auto& pos : positions) {
                if (!image.at(pos.Row + dY).at(pos.Col + dX)) return;
            }
            for (const auto& pos : positions) {
                result[pos.Row + dY][pos.Col + dX] = false;
            }
        };

        for (size_t flipX = 0; flipX < 2; flipX++) {
            result = Constexpr::FlipX(result);
            image = Constexpr::FlipX(image);
            for (size_t flipY = 0; flipY < 2; flipY++) {
                result = Constexpr::FlipY(result);
                image = Constexpr::FlipY(image);
                for (size_t rotation = 0; rotation < 4; rotation++) {
                    result = Constexpr::Rotate(result);
                    image = Constexpr::Rotate(image);
                    for (size_t row = 0; row < image.size() - 3; row++) {
                        for (size_t col = 0; col < image[row].size() - 20; col++) {
                            markSeaMonster(col, row);
                        }
                    }
                }
            }
        }

        return result;
    }

    constexpr size_t CalculateRoughness(const PixelGrid & image, const PixelGrid & mask) {
        size_t result = 0;
        for (size_t row = 0; row < image.size(); row++) {
            for (size_t col = 0; col < image[row].size(); col++) {
                result += image[row][col] && mask[row][col];
            }
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        Constexpr::SmallMap<size_t, Tile> tiles;
        for (const auto& group : groups) {
            auto tile = ParseTile(group);
            tiles[tile.Id] = tile;
        }

        auto neighbors = FindNeighbors(tiles);
        size_t result = 1;
        for (const auto& [id, matches] : neighbors) {
            if (matches.size() == 2) {
                result *= id;
            }
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        TileMap tiles;
        for (const auto& group : groups) {
            auto tile = ParseTile(group);
            tiles[tile.Id] = tile;
        }

        auto neighbors = FindNeighbors(tiles);
        TileGrid tileGrid;
        FitFirstPiece(tiles, neighbors, tileGrid);
        Constexpr::SmallSet<size_t> seen;

        for (const auto& tile : tileGrid[0]) {
            seen.insert(tile.Id);
        }

        FitFirstRow(tiles, neighbors, tileGrid, seen);
        FlipFirstRowIfNeeded(tiles, neighbors, tileGrid);
        FitRemainingRows(tiles, neighbors, tileGrid, seen);

        auto compressedImage = CompressImage(tileGrid);
        auto mask = FindSeaMonsters(compressedImage);

        return CalculateRoughness(compressedImage, mask);
    }
    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Tile 2311:",
            "..##.#..#.",
            "##..#.....",
            "#...##..#.",
            "####.#...#",
            "##.##.###.",
            "##...#.###",
            ".#.#.#..##",
            "..#....#..",
            "###...#.#.",
            "..###..###",
            "",
            "Tile 1951:",
            "#.##...##.",
            "#.####...#",
            ".....#..##",
            "#...######",
            ".##.#....#",
            ".###.#####",
            "###.##.##.",
            ".###....#.",
            "..#.#..#.#",
            "#...##.#..",
            "",
            "Tile 1171:",
            "####...##.",
            "#..##.#..#",
            "##.#..#.#.",
            ".###.####.",
            "..###.####",
            ".##....##.",
            ".#...####.",
            "#.##.####.",
            "####..#...",
            ".....##...",
            "",
            "Tile 1427:",
            "###.##.#..",
            ".#..#.##..",
            ".#.##.#..#",
            "#.#.#.##.#",
            "....#...##",
            "...##..##.",
            "...#.#####",
            ".#.####.#.",
            "..#..###.#",
            "..##.#..#.",
            "",
            "Tile 1489:",
            "##.#.#....",
            "..##...#..",
            ".##..##...",
            "..#...#...",
            "#####...#.",
            "#..#.#.#.#",
            "...#.#.#..",
            "##.#...##.",
            "..##.##.##",
            "###.##.#..",
            "",
            "Tile 2473:",
            "#....####.",
            "#..#.##...",
            "#.##..#...",
            "######.#.#",
            ".#...#.#.#",
            ".#########",
            ".###.#..#.",
            "########.#",
            "##...##.#.",
            "..###.#.#.",
            "",
            "Tile 2971:",
            "..#.#....#",
            "#...###...",
            "#.#.###...",
            "##.##..#..",
            ".#####..##",
            ".#..####.#",
            "#..#.#..#.",
            "..####.###",
            "..#.#.###.",
            "...#.#.#.#",
            "",
            "Tile 2729:",
            "...#.#.#.#",
            "####.#....",
            "..#.#.....",
            "....#..#.#",
            ".##..##.#.",
            ".#.####...",
            "####.#.#..",
            "##.####...",
            "##..#.##..",
            "#.##...##.",
            "",
            "Tile 3079:",
            "#.#.#####.",
            ".#..######",
            "..#.......",
            "######....",
            "####.#..#.",
            ".#...#.##.",
            "#.#####.##",
            "..#.###...",
            "..#.......",
            "..#.###..."
        };

        if (SolvePartOne(lines) != 20899048083289) return false;
        if (SolvePartTwo(lines) != 273) return false;
        return true;
    }
}