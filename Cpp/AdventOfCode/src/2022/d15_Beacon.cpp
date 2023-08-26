#include "2022/d15_Beacon.h"

SOLUTION(2022, 15) {
    struct Sensor {
        Coord Pos;
        Coord Beacon;
        size_t Distance;

        constexpr bool IsInRange(Coord pos) const {
            return MDistance(Pos, pos) <= Distance;
        }

        constexpr bool CanContainUnseenPoints(Coord min, Coord max) const {
            std::vector<Coord> corners = { min, {min.X, max.Y}, {max.X, min.Y}, max };
            auto biggest = RunAllReturnMax(corners, [&](Coord corner) {
                return MDistance(Pos, corner);
                });
            return biggest > Distance;
        }
    };

    //Sensor at x=220580, y=684270: closest beacon is at x=436611, y=263737
    constexpr Sensor ParseSensor(std::string_view line) {
        auto s = Constexpr::Split(line, "=");
        Sensor result;
        Constexpr::ParseNumber(s[1].substr(0, s[1].find(',')), result.Pos.X);
        Constexpr::ParseNumber(s[2].substr(0, s[2].find(':')), result.Pos.Y);
        Constexpr::ParseNumber(s[3].substr(0, s[3].find(',')), result.Beacon.X);
        Constexpr::ParseNumber(s[4], result.Beacon.Y);
        result.Distance = MDistance(result.Pos, result.Beacon);
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines, s64 targetRow) {
        auto sensors = ParseLines(lines, ParseSensor);
        Constexpr::SmallSet<Coord> occupied;
        s64 minX{ 99999999999 }, maxX{ 0 }, maxRange{ 0 };
        for (const auto& sensor : sensors) {
            occupied.insert(sensor.Pos);
            occupied.insert(sensor.Beacon);
            minX = std::min(minX, std::min(sensor.Pos.X, sensor.Beacon.X));
            maxX = std::max(maxX, std::max(sensor.Pos.X, sensor.Beacon.X));
            maxRange = std::max(maxRange, static_cast<s64>(sensor.Distance));
        }
        
        size_t result = 0;
        for (auto x = minX - maxRange; x <= maxX + maxRange; x++) {
            Coord pos{ x, targetRow };
            if (occupied.contains(pos)) continue;
            result += std::any_of(sensors.begin(), sensors.end(), [pos](const Sensor& s) { return s.IsInRange(pos); });
        }

        return result;
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines, 2'000'000));
    }

    constexpr Coord SolvePartTwo(const auto& lines, Coord min, Coord max) {
        auto sensors = ParseLines(lines, ParseSensor);

        Constexpr::Stack<std::pair<Coord, Coord>> qStack;
        qStack.push(std::make_pair(min, max));
        while (!qStack.is_empty()) {
            auto [a, b] = qStack.pop();
            if (a == b) {
                if (std::all_of(sensors.begin(), sensors.end(), [a](const Sensor& s) { return !s.IsInRange(a); })) {
                    return a;
                }
            }
            else {
                Coord mid = { (a.X + b.X) / 2, (a.Y + b.Y) / 2 };
                std::vector<std::pair<Coord, Coord>> quads = {
                    {a, mid},
                    {{mid.X + 1, a.Y}, {b.X, mid.Y}},
                    {{a.X, mid.Y + 1}, {mid.X, b.Y}},
                    {{mid.X + 1, mid.Y + 1}, b}
                };
                for (const auto& quad : quads) {
                    if (quad.first.X > quad.second.X || quad.first.Y > quad.second.Y) continue;
                    if (std::all_of(sensors.begin(), sensors.end(), [quad](const Sensor& s) { return s.CanContainUnseenPoints(quad.first, quad.second); })) {
                        qStack.push(quad);
                    }
                }
            }
        }
        return { 0, 0 };
    }
    PART_TWO() {
        auto missingPoint = SolvePartTwo(Lines, { 0, 0 }, { 4'000'000, 4'000'000 });
        return Constexpr::ToString(missingPoint.X * 4'000'000 + missingPoint.Y);
    }

    TESTS() {
        std::vector < std::string> lines = {
            "Sensor at x=2, y=18: closest beacon is at x=-2, y=15",
            "Sensor at x=9, y=16: closest beacon is at x=10, y=16",
            "Sensor at x=13, y=2: closest beacon is at x=15, y=3",
            "Sensor at x=12, y=14: closest beacon is at x=10, y=16",
            "Sensor at x=10, y=20: closest beacon is at x=10, y=16",
            "Sensor at x=14, y=17: closest beacon is at x=10, y=16",
            "Sensor at x=8, y=7: closest beacon is at x=2, y=10",
            "Sensor at x=2, y=0: closest beacon is at x=2, y=10",
            "Sensor at x=0, y=11: closest beacon is at x=2, y=10",
            "Sensor at x=20, y=14: closest beacon is at x=25, y=17",
            "Sensor at x=17, y=20: closest beacon is at x=21, y=22",
            "Sensor at x=16, y=7: closest beacon is at x=15, y=3",
            "Sensor at x=14, y=3: closest beacon is at x=15, y=3",
            "Sensor at x=20, y=1: closest beacon is at x=15, y=3"
        };

        if (SolvePartOne(lines, 10) != 26) return false;
        if (SolvePartTwo(lines, { 0, 0 }, { 20, 20 }) != Coord{14, 11}) return false;

        return true;
    }
}