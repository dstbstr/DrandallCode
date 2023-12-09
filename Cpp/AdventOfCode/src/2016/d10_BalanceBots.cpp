#include "2016/d10_BalanceBots.h"

SOLUTION(2016, 10) {
    struct Bot {
        size_t Low = 0;
        size_t High = 0;

        constexpr bool IsFull() const {
            return Low > 0 && High > 0;
        }

        void Receive(size_t chip) {
            if (chip > High) {
                Low = High;
                High = chip;
            }
            else {
                Low = chip;
            }
        }
    };

    constexpr auto GenInstruction(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        bool isInput = s[0] == "value";
        s32 botId = -1, lowBotId = -1, highBotId = -1, lowBinId = -1, highBinId = -1, value = -1;
        if (s[0] == "value") {
            Constexpr::ParseNumber(s[1], value);
            Constexpr::ParseNumber(s[5], botId);
        }
        else {
            Constexpr::ParseNumber(s[1], botId);
            if (s[5] == "bot") {
                Constexpr::ParseNumber(s[6], lowBotId);
            }
            else {
                Constexpr::ParseNumber(s[6], lowBinId);
            }
            if (s[10] == "bot") {
                Constexpr::ParseNumber(s[11], highBotId);
            }
            else {
                Constexpr::ParseNumber(s[11], highBinId);
            }
        }

        return [=](std::vector<Bot>& bots, std::vector<size_t>& outBins) {
            if (isInput) {
                bots[botId].Receive(value);
                return true;
            }
            else {
                auto& bot = bots[botId];
                if (!bot.IsFull()) return false;

                if (lowBotId >= 0) {
                    bots[lowBotId].Receive(bot.Low);
                }
                else {
                    outBins[lowBinId] = bot.Low;
                }

                if (highBotId >= 0) {
                    bots[highBotId].Receive(bot.High);
                }
                else {
                    outBins[highBinId] = bot.High;
                }

                return true;
            }
        };
    }

    constexpr void Impl(const auto& lines, std::vector<Bot>& outBots, std::vector<size_t>& outBins) {
        auto instructions = ParseLines(lines, GenInstruction);

        outBots.resize(lines.size());
        outBins.resize(lines.size());
        std::vector<size_t> finishedIndexes;
        size_t prev = 0;

        while (true) {
            for (size_t i = 0; i < instructions.size(); i++) {
                if (std::find(finishedIndexes.begin(), finishedIndexes.end(), i) != finishedIndexes.end()) continue;
                if (instructions[i](outBots, outBins)) {
                    finishedIndexes.push_back(i);
                }
            }
            if (finishedIndexes.size() == instructions.size()) break;
            if (finishedIndexes.size() == prev) throw "Not Found";
            prev = finishedIndexes.size();

            std::sort(finishedIndexes.begin(), finishedIndexes.end());
        }
    }

    PART_ONE() {
        std::vector<Bot> bots;
        std::vector<size_t> bins;
        Impl(lines, bots, bins);
        for (size_t i = 0; i < bots.size(); i++) {
            if (bots[i].Low == 17 && bots[i].High == 61) {
                return Constexpr::ToString(i);
            }
        }

        return "Not Found";
    }

    PART_TWO() {
        std::vector<Bot> bots;
        std::vector<size_t> bins;
        Impl(lines, bots, bins);

        return Constexpr::ToString(bins[0] * bins[1] * bins[2]);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "value 5 goes to bot 2",
            "bot 2 gives low to bot 1 and high to bot 0",
            "value 3 goes to bot 1",
            "bot 1 gives low to output 1 and high to bot 0",
            "bot 0 gives low to output 2 and high to output 0",
            "value 2 goes to bot 2"
        };

        std::vector<Bot> bots;
        std::vector<size_t> bins;
        Impl(lines, bots, bins);

        if (bins[0] != 5) return false;
        if (bins[1] != 2) return false;
        if (bins[2] != 3) return false;

        return true;
    }
}