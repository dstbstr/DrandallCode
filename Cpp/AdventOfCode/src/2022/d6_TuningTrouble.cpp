#include "2022/d6_TuningTrouble.h"

SOLUTION(2022, 6) {
    enum struct MessageType { Signal = 4, Message = 14};

    constexpr size_t FindUnique(std::string_view input, MessageType messageType) {
        std::vector<char> allChars;
        size_t runLength = static_cast<size_t>(messageType);
        for (size_t i = 0; i < input.size() - runLength; i++) {
            allChars.clear();
            for (auto j = 0; j < runLength; j++) {
                if (std::find(allChars.begin(), allChars.end(), input[i + j]) == allChars.end()) {
                    allChars.push_back(input[i + j]);
                }
                else {
                    break;
                }
            }
            if (allChars.size() == runLength) {
                return i + runLength;
            }
        }

        return 0;
    }

    PART_ONE() {
        return Constexpr::ToString(FindUnique(Line, MessageType::Signal));
    }

    PART_TWO() {
        return Constexpr::ToString(FindUnique(Line, MessageType::Message));
    }

    TESTS() {
        static_assert(FindUnique("bvwbjplbgvbhsrlpgdmjqwftvncz", MessageType::Signal) == 5);
        static_assert(FindUnique("nppdvjthqldpwncqszvftbrmjlhg", MessageType::Signal) == 6);
        static_assert(FindUnique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", MessageType::Signal) == 10);
        static_assert(FindUnique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", MessageType::Signal) == 11);

        static_assert(FindUnique("mjqjpqmgbljsphdztnvjfqwrcgsmlb", MessageType::Message) == 19);
        
        if (FindUnique("bvwbjplbgvbhsrlpgdmjqwftvncz", MessageType::Signal) != 5) return false;
        if (FindUnique("nppdvjthqldpwncqszvftbrmjlhg", MessageType::Signal) != 6) return false;
        if (FindUnique("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", MessageType::Signal) != 10) return false;
        if (FindUnique("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", MessageType::Signal) != 11) return false;

        if (FindUnique("mjqjpqmgbljsphdztnvjfqwrcgsmlb", MessageType::Message) != 19) return false;
        return true;
    }
}