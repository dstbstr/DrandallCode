#pragma once

#include "Common.h"

struct Bot {
    Vec3<s32> Pos;
    s32 Radius;
};

constexpr Bot ParseBot(const std::string& line) {
    auto openAngle = line.find("<");
    auto closeAngle = line.find(">");
    auto posStr = line.substr(openAngle + 1, closeAngle - openAngle - 1);
    auto rStr = line.substr(closeAngle + 5);

    auto split = Constexpr::Split(posStr, ",");
    Bot bot;
    Constexpr::ParseNumber(split[0], bot.Pos.X);
    Constexpr::ParseNumber(split[1], bot.Pos.Y);
    Constexpr::ParseNumber(split[2], bot.Pos.Z);
    Constexpr::ParseNumber(rStr, bot.Radius);

    return bot;
}

static_assert(ParseBot("pos=<11,22,33>, r=44").Pos == Vec3<s32>{11, 22, 33});
static_assert(ParseBot("pos=<11,22,33>, r=44").Radius == 44);

constexpr std::vector<Bot> ParseLines(const std::vector<std::string>& lines) {
    std::vector<Bot> result;
    for (const auto& line : lines) {
        result.push_back(ParseBot(line));
    }

    return result;
}

auto PartOne(const std::vector<std::string>& lines) {
    auto bots = ParseLines(lines);
    Bot biggest = bots[0];
    for (const auto& bot : bots) {
        if (biggest.Radius < bot.Radius) {
            biggest = bot;
        }
    }

    u32 count = 0;
    for (const auto& bot : bots) {
        if (MDistance(bot.Pos, biggest.Pos) <= biggest.Radius) {
            count++;
        }
    }

    return count;
}

//Totally stole this solution.  Don't even know how it works
auto PartTwo(const std::vector<std::string>& lines) {
    auto bots = ParseLines(lines);
    struct Data {
        s32 Distance;
        s8 E;

        constexpr bool operator<(const Data& other) const {
            return other.Distance < Distance;
        }
    };

    Vec3<s32> origin = { 0, 0, 0 };
    std::priority_queue<Data> queue;
    for (const auto& bot : bots) {
        auto dist = MDistance(bot.Pos, origin);
        queue.push({ std::max(0, dist - bot.Radius), 1 });
        queue.push({ dist + bot.Radius, -1 });
    }

    s32 count = 0;
    s32 maxCount = 0;
    s32 result = 0;
    while (!queue.empty()) {
        auto [dist, e] = queue.top();
        queue.pop();
        count += e;
        if (count > maxCount) {
            result = dist;
            maxCount = count;
        }
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "pos=<10,12,12>, r=2",
        "pos=<12,14,12>, r=2",
        "pos=<16,12,12>, r=4",
        "pos=<14,14,14>, r=6",
        "pos=<50,50,50>, r=200",
        "pos=<10,10,10>, r=5"
    };

    if (PartTwo(lines) != 36) return false;
    return true;
}