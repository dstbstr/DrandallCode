#include "2021/d13_FoldingPaper.h"

SOLUTION(2021, 13) {
    constexpr std::vector<UCoord> ParseInput(const auto& lines) {
        std::vector<UCoord> result;
        for (const auto& line : lines) {
            result.push_back(UCoord(line));
        }
        return result;
    }

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

    constexpr Fold ParseFold(std::string_view line) {
        auto s = Constexpr::Split(line, "=");
        Fold fold;
        Constexpr::ParseNumber(s[1], fold.Edge);
        fold.IsX = s[0].back() == 'x';
        return fold;
    }

    constexpr std::string PrintPaper(const std::vector<UCoord>& paper) {
        auto [min, max] = GetLimits(paper);
        std::string result = "\n";
        for (auto y = min.Y; y <= max.Y; y++) {
            for (auto x = min.X; x <= max.X; x++) {
                UCoord pos = { x, y };
                if (std::find(paper.begin(), paper.end(), pos) == paper.end()) {
                    result.push_back('.');
                }
                else {
                    result.push_back('#');
                }
            }
            result.push_back('\n');
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto paper = ParseInput(groups[0]);
        auto folds = ParseLines(groups[1], ParseFold);
        DoFold(paper, folds[0]);

        return paper.size();
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);
        auto paper = ParseInput(groups[0]);
        auto folds = ParseLines(groups[1], ParseFold);
        for (const auto& fold : folds) {
            DoFold(paper, fold);
        }

        return PrintPaper(paper);
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
        return SolvePartOne(lines) == 17;
    }

    TESTS() {
        static_assert(ParseInput(std::array<std::string, 2>{ "6,10", "0,14" }).size() == 2);
        static_assert(ParseInput(std::array<std::string, 2>{ "6,10", "0,14" })[0].X == 6);
        
        static_assert(ParseFold("fold along y=7").Edge == 7);
        static_assert(ParseFold("fold along y=7").IsX == false);
        static_assert(ParseFold("fold along x=3").IsX == true);

        static_assert(TestPartOne());

        return true;
    }
}