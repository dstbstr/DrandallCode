#include "2021/d13_FoldingPaper.h"

SOLUTION(2021, 13) {
    constexpr std::vector<UCoord> ParseInput(const std::vector<std::string>&lines) {
        std::vector<UCoord> result;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, ",");
            UCoord coord;
            Constexpr::ParseNumber(split[0], coord.X);
            Constexpr::ParseNumber(split[1], coord.Y);
            result.push_back(coord);
        }
        return result;
    }

    static_assert(ParseInput({ "6,10", "0,14" }).size() == 2);
    static_assert(ParseInput({ "6,10", "0,14" })[0].X == 6);

    struct Fold {
        u32 Edge = 0;
        bool IsX = false;
    };

    constexpr void DoFold(std::vector<UCoord>&paper, Fold fold) {
        if (fold.IsX) {
            for (auto& pt : paper) {
                if (pt.X > fold.Edge) {
                    pt.X = fold.Edge - (pt.X - fold.Edge);
                }
            }
        }
        else {
            for (auto& pt : paper) {
                if (pt.Y > fold.Edge) {
                    pt.Y = fold.Edge - (pt.Y - fold.Edge);
                }
            }
        }

        std::sort(paper.begin(), paper.end());
        paper.erase(std::unique(paper.begin(), paper.end()), paper.end());
    }

    constexpr std::vector<Fold> ParseFolds(const std::vector<std::string>&lines) {
        std::vector<Fold> result;
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, "=");
            Fold fold;
            Constexpr::ParseNumber(split[1], fold.Edge);
            fold.IsX = split[0].back() == 'x';
            result.push_back(fold);
        }
        return result;
    }

    static_assert(ParseFolds({ "fold along y=7", "fold along x=5" }).size() == 2);
    static_assert(ParseFolds({ "fold along y=7", "fold along x=5" })[0].IsX == false);
    static_assert(ParseFolds({ "fold along y=7", "fold along x=5" })[0].Edge == 7);

    void PrintPaper(const std::vector<UCoord>&paper) {
        auto [min, max] = GetLimits(paper);
        for (auto y = min.Y; y <= max.Y; y++) {
            for (auto x = min.X; x <= max.X; x++) {
                UCoord pos = { x, y };
                if (std::find(paper.begin(), paper.end(), pos) == paper.end()) {
                    std::cout << '.';
                }
                else {
                    std::cout << '#';
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    constexpr auto Part1(const std::vector<std::string>&lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto paper = ParseInput(groups[0]);
        auto folds = ParseFolds(groups[1]);
        DoFold(paper, folds[0]);

        return paper.size();
    }

    constexpr bool TestPartOne() {
        std::vector<std::string> lines = {
            "6,10",
            "0,14",
            "9,10",
            "0,3",
            "10,4",
            "4,11",
            "6,0",
            "6,12",
            "4,1",
            "0,13",
            "10,12",
            "3,4",
            "3,0",
            "8,4",
            "1,10",
            "2,14",
            "8,10",
            "9,0",
            "",
            "fold along y = 7",
            "fold along x = 5"
        };
        return Part1(lines) == 17;
    }

    static_assert(TestPartOne());
    auto Part2(const std::vector<std::string>&lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto paper = ParseInput(groups[0]);
        auto folds = ParseFolds(groups[1]);
        for (const auto& fold : folds) {
            DoFold(paper, fold);
        }

        PrintPaper(paper);
        return paper.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        if (!TestPartOne()) return false;

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