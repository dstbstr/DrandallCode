#pragma once

#include "Common.h"

template<size_t MsgLength>
using CountType = std::array<std::array<u32, 26>, MsgLength>;

template<size_t MsgLength>
constexpr void UpdateLetterCounts(const std::string& line, CountType<MsgLength>& result) {
    for (size_t i = 0; i < line.size(); i++) {
        result[i][line[i] - 'a']++;
    }
}

template<size_t MsgLength>
constexpr std::string Decode(CountType<MsgLength>& counts) {
    std::string result = "";
    for (auto i = 0; i < MsgLength; i++) {
        u32 best = 0;
        u32 bestIndex = 0;
        for (auto count = 0; count < 26; count++) {
            if (counts[i][count] > best) {
                best = counts[i][count];
                bestIndex = count;
            }
        }

        result += static_cast<char>(bestIndex + 'a');
    }

    return result;
}

template<size_t MsgLength>
constexpr std::string DecodeMin(CountType<MsgLength>& counts) {
    std::string result = "";
    for (auto i = 0; i < MsgLength; i++) {
        u32 best = 999;
        u32 bestIndex = 0;
        for (auto count = 0; count < 26; count++) {
            if (counts[i][count] > 0 && counts[i][count] < best) {
                best = counts[i][count];
                bestIndex = count;
            }
        }
        result += static_cast<char>(bestIndex + 'a');
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    CountType<8> counts;
    for (auto& count : counts) {
        std::fill(count.begin(), count.end(), 0);
    }
    for (const auto& line : lines) {
        UpdateLetterCounts(line, counts);
    }
    //return Decode(counts);
    return DecodeMin(counts);
}

bool RunTests() {
    CountType<6> counts;
    for (auto& count : counts) {
        std::fill(count.begin(), count.end(), 0);
    }

    UpdateLetterCounts("eedadn", counts);
    UpdateLetterCounts("drvtee", counts);
    UpdateLetterCounts("eandsr", counts);
    UpdateLetterCounts("raavrd", counts);
    UpdateLetterCounts("atevrs", counts);
    UpdateLetterCounts("tsrnev", counts);
    UpdateLetterCounts("sdttsa", counts);
    UpdateLetterCounts("rasrtv", counts);
    UpdateLetterCounts("nssdts", counts);
    UpdateLetterCounts("ntnada", counts);
    UpdateLetterCounts("svetve", counts);
    UpdateLetterCounts("tesnvt", counts);
    UpdateLetterCounts("vntsnd", counts);
    UpdateLetterCounts("vrdear", counts);
    UpdateLetterCounts("dvrsen", counts);
    UpdateLetterCounts("enarar", counts);

    if (counts[0]['e' - 'a'] == 0) return false;

    if (Decode(counts) != "easter") return false;
    if (DecodeMin(counts) != "advent") return false;
    return true;
}