#include "2016/d4_DecoyRoom.h"

SOLUTION(2016, 4) {
    struct Room {
        std::vector<std::string> Parts;
        u32 Id = 0;
        std::string Checksum;
    };

    constexpr Room ParseRoom(std::string_view line) {
        auto s1 = Constexpr::Split(line, "[");
        auto s2 = Constexpr::Split(s1[0], "-");
        Room result;
        Constexpr::ParseNumber(s2.back(), result.Id);
        result.Checksum = s1[1].substr(0, s1[1].size() - 1);
        for (auto i = 0; i < s2.size() - 1; i++) {
            result.Parts.push_back(std::string(s2[i]));
        }
        return result;
    }

    constexpr std::string DecryptRoom(const Room& room) {
        auto rot = room.Id % 26;
        std::vector<std::string> decrypted;
        for (const auto& part : room.Parts) {
            std::string s;
            for (auto c : part) {
                s.push_back(static_cast<char>(((c - 'a' + rot) % 26) + 'a'));
            }
            decrypted.push_back(s);
        }

        return Constexpr::JoinVec(' ', decrypted);
    }

    bool IsReal(const Room& room) {
        std::string expectedChecksum = "";
        auto counts = std::array<s32, 26>{};

        for (const auto& part : room.Parts) {
            for (auto c : part) {
                counts[static_cast<size_t>(c - 'a')]++;
            }
        }

        std::vector<s32> sortedCounts;
        for (auto count : counts) {
            sortedCounts.push_back(count);
        }

        std::sort(sortedCounts.begin(), sortedCounts.end(), [](s32 lhs, s32 rhs) {return lhs > rhs; });

        for (auto i = 0; i < 5; i++) {
            auto expectedCount = sortedCounts[i];
            for (auto j = 0; j < counts.size(); j++) {
                if (counts[j] == expectedCount) {
                    expectedChecksum += static_cast<char>(j + 'a');
                    counts[j] = -1;
                    break;
                }
            }
        }

        return room.Checksum == expectedChecksum;
    }

    PART_ONE() {
        auto rooms = ParseLines(lines, ParseRoom);
        return Constexpr::ToString(std::accumulate(rooms.begin(), rooms.end(), 0, [](u32 prev, const Room& room) {
            return prev + room.Id * IsReal(room);
            }));
    }
    PART_TWO() {
        auto rooms = ParseLines(lines, ParseRoom);
        auto result = std::find_if(rooms.begin(), rooms.end(), [](const Room& room) {
            return IsReal(room) && DecryptRoom(room) == "northpole object storage";
            });
        return Constexpr::ToString(result->Id);
    }

    TESTS() {
        auto room = ParseRoom("qzmt-zixmtkozy-ivhz-343[abcde]");
        if (DecryptRoom(room) != "very encrypted name") return false;
        
        return true;
    }
}
