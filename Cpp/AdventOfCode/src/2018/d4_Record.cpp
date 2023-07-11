#include "2018/d4_Record.h"
SOLUTION(2018, 4) {

    struct DateTime {
        u8 Month{ 0 };
        u8 Day{ 0 };
        u8 Hour{ 0 };
        u8 Minute{ 0 };

        constexpr bool operator<(const DateTime& other) const {
            return Month != other.Month ? Month < other.Month :
                Day != other.Day ? Day < other.Day :
                Hour != other.Hour ? Hour < other.Hour :
                Minute < other.Minute;
        }

        constexpr bool operator==(const DateTime& other) const {
            return Month == other.Month &&
                Day == other.Day &&
                Hour == other.Hour &&
                Minute == other.Minute;
        }
    };

    enum struct Action { Start, Awake, Sleep };
    struct Event {
        u32 GuardId = 0;
        DateTime EventTime;
        Action EventAction;

        constexpr bool operator<(const Event& other) const {
            return EventTime < other.EventTime;
        }
    };

    constexpr DateTime ParseDateTime(const std::string & dtString) {
        auto split = Constexpr::Split(dtString, " ");
        auto dateSplit = Constexpr::Split(split[0], "-");
        auto timeSplit = Constexpr::Split(split[1], ":");

        DateTime result;
        Constexpr::ParseNumber(dateSplit[1], result.Month);
        Constexpr::ParseNumber(dateSplit[2], result.Day);
        Constexpr::ParseNumber(timeSplit[0], result.Hour);
        Constexpr::ParseNumber(timeSplit[1], result.Minute);

        return result;
    }

    constexpr Event ParseEvent(const std::string & line) {
        auto dtString = line.substr(1, line.find(']') - 1);
        Event event;
        event.EventTime = ParseDateTime(dtString);

        auto split = Constexpr::Split(line, " ");
        if (split.size() == 4) {
            event.EventAction = (split[2] == "wakes" ? Action::Awake : Action::Sleep);
        }
        else {
            Constexpr::ParseNumber(split[3].substr(1), event.GuardId);
            event.EventAction = Action::Start;
        }

        return event;
    }

    constexpr void PopulateSleepData(const std::vector<Event>& events, Constexpr::SmallMap<u32, u32>& guardSleepAmounts, Constexpr::SmallMap<u32, std::array<u8, 60>>& guardSleepTimes) {
        u32 currentGuard = 0;
        DateTime sleepStart;
        auto UpdateSleepTimes = [&](DateTime sleepStop) {
            for (auto minute = sleepStart.Minute; (minute % 60) != sleepStop.Minute; minute++) {
                guardSleepAmounts[currentGuard]++;
                guardSleepTimes[currentGuard][minute]++;
            }
        };
        for (const auto& event : events) {
            switch (event.EventAction) {
            case Action::Start: currentGuard = event.GuardId; break;
            case Action::Sleep: sleepStart = event.EventTime; break;
            case Action::Awake: UpdateSleepTimes(event.EventTime); break;
            default: break;
            }
        }
    }

    PART_ONE() {
        Constexpr::SmallMap<u32, u32> guardSleepAmounts{};
        Constexpr::SmallMap<u32, std::array<u8, 60>> guardSleepTimes{};
        auto events = ParseLines(lines, ParseEvent);
        std::sort(events.begin(), events.end());
        PopulateSleepData(events, guardSleepAmounts, guardSleepTimes);

        u32 mostSleep = 0;
        u32 sleepiestGuard = 0;

        for (const auto& [guardId, sleepAmount] : guardSleepAmounts) {
            if (mostSleep < sleepAmount) {
                sleepiestGuard = guardId;
                mostSleep = sleepAmount;
            }
        }

        u32 resultMinute = 0;
        u32 minuteCount = 0;
        for (auto minute = 0; minute < 60; minute++) {
            if (minuteCount < guardSleepTimes[sleepiestGuard][minute]) {
                minuteCount = guardSleepTimes[sleepiestGuard][minute];
                resultMinute = minute;
            }
        }

        return Constexpr::ToString(resultMinute * sleepiestGuard);
    }

    PART_TWO() {
        Constexpr::SmallMap<u32, u32> guardSleepAmounts{};
        Constexpr::SmallMap<u32, std::array<u8, 60>> guardSleepTimes{};
        auto events = ParseLines(lines, ParseEvent);
        std::sort(events.begin(), events.end());

        PopulateSleepData(events, guardSleepAmounts, guardSleepTimes);

        u32 resultGuardId = 0;
        u32 resultMinute = 0;
        u32 minuteCount = 0;
        for (const auto& [guardId, times] : guardSleepTimes) {
            for (auto minute = 0; minute < 60; minute++) {
                if (minuteCount < times[minute]) {
                    resultGuardId = guardId;
                    resultMinute = minute;
                    minuteCount = times[minute];
                }
            }
        }

        return Constexpr::ToString(resultMinute * resultGuardId);
    }

    TESTS() {
        static_assert(ParseDateTime("1518-10-05 00:56").Month == 10);
        static_assert(ParseDateTime("1518-10-05 00:56").Day == 5);
        static_assert(ParseDateTime("1518-10-05 00:56").Hour == 0);
        static_assert(ParseDateTime("1518-10-05 00:56").Minute == 56);

        static_assert(ParseEvent("[1518-11-01 00:00] Guard #10 begins shift").EventTime == DateTime{ 11, 1, 0, 0 });
        static_assert(ParseEvent("[1518-11-01 00:00] Guard #10 begins shift").GuardId == 10);
        static_assert(ParseEvent("[1518-11-01 00:00] Guard #10 begins shift").EventAction == Action::Start);
        static_assert(ParseEvent("[1518-10-05 00:56] falls asleep").EventTime == DateTime{ 10, 5, 0, 56 });
        static_assert(ParseEvent("[1518-10-05 00:56] falls asleep").EventAction == Action::Sleep);
        static_assert(ParseEvent("[1518-05-09 00:32] wakes up").EventAction == Action::Awake);

        std::vector<std::string> lines = {
            "[1518-11-01 00:00] Guard #10 begins shift",
            "[1518-11-01 00:05] falls asleep",
            "[1518-11-01 00:25] wakes up",
            "[1518-11-01 00:30] falls asleep",
            "[1518-11-01 00:55] wakes up",
            "[1518-11-01 23:58] Guard #99 begins shift",
            "[1518-11-02 00:40] falls asleep",
            "[1518-11-02 00:50] wakes up",
            "[1518-11-03 00:05] Guard #10 begins shift",
            "[1518-11-03 00:24] falls asleep",
            "[1518-11-03 00:29] wakes up",
            "[1518-11-04 00:02] Guard #99 begins shift",
            "[1518-11-04 00:36] falls asleep",
            "[1518-11-04 00:46] wakes up",
            "[1518-11-05 00:03] Guard #99 begins shift",
            "[1518-11-05 00:45] falls asleep",
            "[1518-11-05 00:55] wakes up"
        };

        if (PartOne(lines) != "240") return false;
        if (PartTwo(lines) != "4455") return false;
        return true;

        return true;
    }
}