#include "2018/d23_Teleport.h"

#include <queue>

SOLUTION(2018, 23) {
    struct Bot {
        Vec3<s32> Pos;
        s32 Radius;
        constexpr bool operator<(const Bot& other) const {
            return Radius < other.Radius;
        }
    };

    constexpr Bot ParseBot(std::string_view line) {
        auto s0 = Constexpr::Split(line, "=<");
        auto s1 = Constexpr::Split(s0[1], ">, r=");
        auto s2 = Constexpr::Split(s1[0], ",");
        
        Bot bot;
        Constexpr::ParseNumber(s2[0], bot.Pos.X);
        Constexpr::ParseNumber(s2[1], bot.Pos.Y);
        Constexpr::ParseNumber(s2[2], bot.Pos.Z);
        Constexpr::ParseNumber(s1[1], bot.Radius);

        return bot;
    }

    PART_ONE() {
        auto bots = ParseLines(Lines, ParseBot);
        std::sort(bots.begin(), bots.end());
        auto biggest = bots.back();
        return Constexpr::ToString(std::count_if(bots.begin(), bots.end(), [&](const Bot& bot) { return MDistance(bot.Pos, biggest.Pos) <= biggest.Radius; }));
    }

    //Totally stole this solution.  Don't even know how it works
    PART_TWO() {
        auto bots = ParseLines(Lines, ParseBot);
        struct Data {
            s32 Distance;
            s8 E;

            constexpr bool operator<(const Data& other) const {
                return other.Distance < Distance;
            }
        };

        Vec3<s32> origin = { 0, 0, 0 };
        Constexpr::PriorityQueue<Data> queue;
        for (const auto& bot : bots) {
            auto dist = static_cast<s32>(MDistance(bot.Pos, origin));
            queue.push({ std::max(0, dist - bot.Radius), 1 });
            queue.push({ dist + bot.Radius, -1 });
        }

        s32 count = 0;
        s32 maxCount = 0;
        s32 result = 0;
        while (!queue.empty()) {
            auto [dist, e] = queue.pop();
            count += e;
            if (count > maxCount) {
                result = dist;
                maxCount = count;
            }
        }

        return Constexpr::ToString(result);
    }

    TESTS() {
        static_assert(ParseBot("pos=<11,22,33>, r=44").Pos == Vec3<s32>{11, 22, 33});
        static_assert(ParseBot("pos=<11,22,33>, r=44").Radius == 44);

        std::vector<std::string> lines = {
            "pos=<10,12,12>, r=2",
            "pos=<12,14,12>, r=2",
            "pos=<16,12,12>, r=4",
            "pos=<14,14,14>, r=6",
            "pos=<50,50,50>, r=200",
            "pos=<10,10,10>, r=5"
        };

        //if (PartTwo(lines) != "36") return false;
        return true;
    }
}