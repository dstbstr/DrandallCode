#pragma once

#include "Common.h"

enum struct Hand {
    Low, High
};

std::unordered_map<u32, std::vector<u32>> outputBins;
std::unordered_map<u32, std::vector<std::pair<u32, u32>>> comparisons;

struct Bot {
    u32 Id = 0;
    u32 Low = 0;
    u32 High = 0;

    void Give(Hand which, Bot& who) {
        if (which == Hand::Low) {
            who.Receive(Low);
            Low = 0;
        }
        else {
            who.Receive(High);
            High = 0;
        }
    }

    void Deposit(Hand which, u32 binId) {
        if (which == Hand::Low) {
            outputBins[binId].push_back(Low);
            Low = 0;
        }
        else {
            outputBins[binId].push_back(High);
            High = 0;
        }
    }

    void Receive(u32 chip) {
        if (chip > High) {
            Low = High;
            High = chip;
        }
        else {
            Low = chip;
        }
    }
};

struct Instruction {
    u32 GivingBotId = 0;
    u32 LowDestId = 0;
    u32 HighDestId = 0;
    bool IsLowBot = false;
    bool IsHighBot = false;
};

void ProcessInput(const std::string& line, std::vector<Bot>& bots) {
    static const auto re = std::regex(R"(value (\d+) goes to bot (\d+))");
    std::smatch match;
    u32 id;
    u32 chip;
    if (std::regex_search(line, match, re)) {
        ParseNumber(match[1].str(), chip);
        ParseNumber(match[2].str(), id);

        auto& bot = bots[id];
        bot.Receive(chip);
    }
}

Instruction ParseInstruction(const std::string& line) {
    static const auto re = std::regex(R"(bot (\d+) gives low to (\w+) (\d+) and high to (\w+) (\d+))");
    u32 givingBotId;
    u32 lowDestId;
    u32 highDestId;
    std::smatch match;
    if (!std::regex_search(line, match, re)) {
        throw std::logic_error("Failed to parse instruction");
    }

    ParseNumber(match[1].str(), givingBotId);
    ParseNumber(match[3].str(), lowDestId);
    ParseNumber(match[5].str(), highDestId);

    Instruction result;
    result.GivingBotId = givingBotId;
    result.LowDestId = lowDestId;
    result.HighDestId = highDestId;
    result.IsLowBot = match[2].str() == "bot";
    result.IsHighBot = match[4].str() == "bot";

    return result;
}

void ProcessInstruction(const Instruction& instruction, std::vector<Bot>& bots) {
    auto& givingBot = bots[instruction.GivingBotId];
    comparisons[instruction.GivingBotId].push_back(std::make_pair(givingBot.Low, givingBot.High));

    if (instruction.IsLowBot) {
        auto& destBot = bots[instruction.LowDestId];
        givingBot.Give(Hand::Low, destBot);
    }
    else {
        givingBot.Deposit(Hand::Low, instruction.LowDestId);
    }

    if (instruction.IsHighBot) {
        auto& destBot = bots[instruction.HighDestId];
        givingBot.Give(Hand::High, destBot);
    }
    else {
        givingBot.Deposit(Hand::High, instruction.HighDestId);
    }
}

bool CanProcess(const Instruction& instruction, std::vector<Bot>& bots) {
    auto& bot = bots[instruction.GivingBotId];
    return bot.Low > 0 && bot.High > 0;
}

void ProcessInput(const std::vector<std::string>& lines) {
    std::vector<Bot> bots;
    for (auto i = 0; i < 250; i++) {
        Bot bot;
        bot.Id = i;
        bots.push_back(bot);
    }

    std::vector<Instruction> instructions;
    for (const auto& line : lines) {
        if (StrUtil::StartsWith(line, "value")) {
            ProcessInput(line, bots);
        }
        else {
            instructions.push_back(ParseInstruction(line));
        }
    }

    while (!instructions.empty()) {
        for (s32 i = static_cast<s32>(instructions.size()) - 1; i >= 0; i--) {
            auto instruction = instructions[i];
            if (CanProcess(instruction, bots)) {
                ProcessInstruction(instruction, bots);
                instructions.erase(instructions.begin() + i);
            }
        }
    }
}

std::string RunFirst(const std::vector<std::string>& lines) {
    ProcessInput(lines);

    auto target = std::make_pair<u32, u32>(17, 61);
    for (auto& [key, values] : comparisons) {
        if (std::find(values.cbegin(), values.cend(), target) != values.cend()) {
            return ToString(key);
        }
    }

    return "Not Found";
}

std::string Run(const std::vector<std::string>& lines) {
    ProcessInput(lines);

    return ToString(outputBins[0][0] * outputBins[1][0] * outputBins[2][0]);
}

bool RunTests() {
    std::vector<std::string> lines;
    lines.push_back("value 5 goes to bot 2");
    lines.push_back("bot 2 gives low to bot 1 and high to bot 0");
    lines.push_back("value 3 goes to bot 1");
    lines.push_back("bot 1 gives low to output 1 and high to bot 0");
    lines.push_back("bot 0 gives low to output 2 and high to output 0");
    lines.push_back("value 2 goes to bot 2");

    ProcessInput(lines);

    if (outputBins[0][0] != 5) return false;
    if (outputBins[1][0] != 2) return false;
    if (outputBins[2][0] != 3) return false;

    if (std::find(comparisons[2].cbegin(), comparisons[2].cend(), std::make_pair<u32, u32>(2, 5)) == comparisons[2].cend()) return false;
    return true;
}