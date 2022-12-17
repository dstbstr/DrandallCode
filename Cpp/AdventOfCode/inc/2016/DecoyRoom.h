#pragma once

#include "Common.h"

#include <sstream>
#include <iostream>

struct Room {
    std::string Name;
    std::string OriginalString;
    u32 Id = 0;
    std::string Checksum;
};

Room ParseRoom(std::string line) {
    static const auto re = std::regex(R"(([a-z-]+)(\d+)\[(\w+)\])");
    auto match = std::smatch{};
    std::regex_search(line, match, re);

    std::stringstream stream;

    for(auto c : match[1].str()) {
        if(c != '-') {
            stream << c;
        }
    }
    Room result;
    result.OriginalString = match[1].str();
    result.Name = stream.str();
    result.Checksum = match[3].str();
    ParseNumber(match[2].str(), result.Id);

    return result;
}

std::string DecryptRoom(Room room) {
    auto rot = room.Id % 26;
    std::stringstream stream;
    for(auto c : room.OriginalString) {
        if(c == '-') {
            stream << " ";
        } else {
            stream << static_cast<char>(((c - 'a' + rot) % 26) + 'a');
        }
    }

    return StrUtil::Trim(stream.str());
}

bool IsReal(const Room& room) {
    std::string expectedChecksum = "";
    auto counts = std::array<s32, 26>{};
    for(auto c : room.Name) {
        counts[static_cast<size_t>(c - 'a')]++;
    }

    std::vector<s32> sortedCounts;
    for(auto count : counts) {
        sortedCounts.push_back(count);
    }

    std::sort(sortedCounts.begin(), sortedCounts.end(), [](s32 lhs, s32 rhs){return lhs > rhs; });

    for(auto i = 0; i < 5; i++) {
        auto expectedCount = sortedCounts[i];
        for(auto j = 0; j < counts.size(); j++) {
            if(counts[j] == expectedCount) {
                expectedChecksum += static_cast<char>(j + 'a');
                counts[j] = -1;
                break;
            }
        }
    }

    return room.Checksum == expectedChecksum;
}

u32 RunFirst(std::vector<std::string> lines) {
    u32 result = 0;
    for(auto line : lines) {
        auto room = ParseRoom(line);
        if(IsReal(room)) {
            result += room.Id;
        }
    }
    return result;
}

u32 Run(std::vector<std::string> lines) {
    std::vector<Room> realRooms;
    for(auto line : lines) {
        auto room = ParseRoom(line);
        if(IsReal(room)) {
            realRooms.push_back(room);
        }
    }

    for(auto room : realRooms) {
        if(DecryptRoom(room) == "northpole object storage") {
            return room.Id;
        }
    }

    return 0;
}

bool RunTests() {
    auto lines = std::vector<std::string>{};
    /*
    lines.push_back("aaaaa-bbb-z-y-x-123[abxyz]");
    lines.push_back("a-b-c-d-e-f-g-h-987[abcde]");
    lines.push_back("not-a-real-room-404[oarel]");
    lines.push_back("totally-real-room-200[decoy]");

    if(Run(lines) != 1514) return false;
    */
    auto room = ParseRoom("qzmt-zixmtkozy-ivhz-343[abcde]");
    if(DecryptRoom(room) != "very encrypted name") return false;

    return true;
}