#pragma once

#include <bitset>
#include <vector>
#include <charconv>
#include <unordered_map>

#include "Platform/Types.h"
#include "Utilities/StringUtils.h"

enum struct Command { On, Off, Toggle };
struct Coord {
    size_t X = 0;
    size_t Y = 0;

    Coord() = default;
    Coord(std::string_view input) {
        std::vector<std::string_view> split = StrUtil::Split(input, ",");

        std::from_chars(split[0].data(), split[0].data() + split[0].size(), X);
        std::from_chars(split[1].data(), split[1].data() + split[1].size(), Y);
    }
};

void ParseCommand(const std::string& line, Command& outCommand, Coord& outBottomLeft, Coord& outTopRight) {
    std::string running = line;
    if(StrUtil::StartsWith(running, "turn on")) {
        outCommand = Command::On;
        running = running.substr(std::strlen("turn on "));
    } else if(StrUtil::StartsWith(running, "turn off")) {
        outCommand = Command::Off;
        running = running.substr(std::strlen("turn off "));
    } else if (StrUtil::StartsWith(running, "toggle")) {
        outCommand = Command::Toggle;
        running = running.substr(std::strlen("toggle "));
    }

    std::vector<std::string_view> split = StrUtil::Split(running, "through");
    outBottomLeft = Coord(StrUtil::Trim(std::string(split[0])));
    outTopRight = Coord(StrUtil::Trim(std::string(split[1])));
}

std::vector<size_t> GetLightIndexes(Coord bottomLeft, Coord topRight) {
    std::vector<size_t> result;
    for(auto x = bottomLeft.X; x <= topRight.X; x++) {
        for(auto y = bottomLeft.Y; y <= topRight.Y; y++) {
            result.push_back(x * 1000 + y);
        }
    }
    return result;
}

class LightWar {
    std::bitset<1000000> state;

public:
    void ApplyCommand(const std::string& line) {
        Command command;
        Coord bottomLeft;
        Coord topRight;
        ParseCommand(line, command, bottomLeft, topRight);

        auto indexes = GetLightIndexes(bottomLeft, topRight);

        switch(command) {
            case Command::On:
                {
                    for(auto i : indexes) {
                        state.set(i);
                    }
                    break;
                }
            case Command::Off :
            {
                for(auto i : indexes) {
                    state.set(i, false);
                }
                break;
            }
            case Command::Toggle :
            {
                for(auto i : indexes) {
                    state.flip(i);
                }
                break;
            }
        }
    }

    u32 GetLitCount() {
        return static_cast<u32>(state.count());
    }

    bool RunTests() {
        ApplyCommand("turn on 0,0 through 1,1");
        ApplyCommand("toggle 1,1 through 2,2");
        return GetLitCount() == 6;
    }
};

struct BrightWar {
    u32 Result = 0;
    std::unordered_map<size_t, u32> lightIntensity{};

    void ApplyCommand(const std::string& line) {
        Command command;
        Coord bottomLeft;
        Coord topRight;
        ParseCommand(line, command, bottomLeft, topRight);
        auto indexes = GetLightIndexes(bottomLeft, topRight);

        switch(command) {
            case Command::On : {
                for(auto i : indexes) {
                    lightIntensity[i]++;
                }
                Result += static_cast<u32>(indexes.size());
                break;
            }
            case Command::Off: {
                for(auto i : indexes) {
                    if(lightIntensity[i] > 0) {
                        lightIntensity[i]--;
                        Result--;
                    }
                }
                break;
            }
            case Command::Toggle: {
                for(auto i : indexes) {
                    lightIntensity[i] += 2;
                }
                Result += static_cast<u32>((indexes.size() * 2));
                break;
            }
        }
    }

    u32 GetResult() {
        return Result;
    }

    bool RunTests() {
        ApplyCommand("turn off 0,0 through 1,1");
        ApplyCommand("turn on 0,0 through 1,1");
        if(GetResult() != 4) {
            return false;
        }
        ApplyCommand("turn off 0,0 through 1,1");
        if(GetResult() != 0) {
            return false;
        }
        ApplyCommand("toggle 0,0 through 1,1");
        if(GetResult() != 8) {
            return false;
        }
        ApplyCommand("turn off 0,0 through 1,1");
        if(GetResult() != 4) {
            return false;
        }
        ApplyCommand("turn off 0,0 through 1,1");
        if(GetResult() != 0) {
            return false;
        }
        ApplyCommand("turn off 0,0 through 1,1");
        if(GetResult() != 0) {
            return false;
        }
        return true;
    }
};