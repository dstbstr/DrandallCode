#include "2018/d10_StarMessage.h"

SOLUTION(2018, 10) {
    struct Star {
        Coord Pos;
        Coord Vel;
    };

    constexpr Star ParseLine(std::string_view line) {
        const char* numberSearch = "1234567890-";

        auto s1 = Constexpr::Split(line, "=");
        auto posStr = s1[1].substr(1, s1[1].find('>') - 1);
        auto velStr = s1[2].substr(1, s1[2].find('>') - 1);

        auto pStart = posStr.find_first_of(numberSearch);
        auto pComma = posStr.find(", ");
        auto px = posStr.substr(pStart, pComma - pStart);
        auto py = posStr.substr(posStr.find_first_of(numberSearch, pComma));

        auto vStart = velStr.find_first_of(numberSearch);
        auto vComma = velStr.find(", ");
        auto vx = velStr.substr(vStart, vComma - vStart);
        auto vy = velStr.substr(velStr.find_first_of(numberSearch, vComma));

        Star result;
        Constexpr::ParseNumber(px, result.Pos.X);
        Constexpr::ParseNumber(py, result.Pos.Y);
        Constexpr::ParseNumber(vx, result.Vel.X);
        Constexpr::ParseNumber(vy, result.Vel.Y);

        return result;
    }

    constexpr void TickStars(std::vector<Star>&stars, size_t ticks = 1) {
        for (auto& star : stars) {
            star.Pos.X += star.Vel.X * ticks;
            star.Pos.Y += star.Vel.Y * ticks;
        }
    }

    constexpr void GetLimits(const std::vector<Star> stars, Coord & min, Coord & max) {
        min = { 9999, 9999 };
        max = { -9999, -9999 };
        for (const auto& star : stars) {
            min.X = std::min(min.X, star.Pos.X);
            min.Y = std::min(min.Y, star.Pos.Y);
            max.X = std::max(max.X, star.Pos.X);
            max.Y = std::max(max.Y, star.Pos.Y);
        }
    }

    constexpr bool HasStar(const std::vector<Star>&stars, Coord pos) {
        return std::any_of(stars.begin(), stars.end(), [&pos](const Star& star) { return star.Pos == pos; });
    }

    constexpr bool PrintStarsIfFits(const std::vector<Star>& stars, std::string& outResult) {
        Coord min, max;
        GetLimits(stars, min, max);
        if (Constexpr::Abs(max.Y - min.Y) > 10) return false;
        if (Constexpr::Abs(max.X - min.X) > 100) return false;

        outResult.push_back('\n');
        for (auto y = min.Y; y <= max.Y; y++) {
            for (auto x = min.X; x <= max.X; x++) {
                Coord pos = { x, y };
                outResult.push_back(HasStar(stars, pos) ? '#' : '.');
            }
            outResult.push_back('\n');
        }

        return true;
    }

    constexpr auto Solve(const auto& lines, std::string& outMessage) {
        auto stars = ParseLines(lines, ParseLine);

        size_t seconds = 0;
        if (stars[0].Vel.X > 0) {
            seconds = Constexpr::Abs(stars[0].Pos.Y / stars[0].Vel.Y) - 50;
            TickStars(stars, seconds);
        }

        while (true) {
            if (PrintStarsIfFits(stars, outMessage)) {
                break;
            }

            TickStars(stars, 1);
            seconds++;
        }

        return seconds;
    }

    PART_ONE() {
        std::string result;
        Solve(Lines, result);
        return result;
    }

    PART_TWO() {
        std::string ignored;
        return Constexpr::ToString(Solve(Lines, ignored));
    }

    TESTS() {
        return true;
    }
}