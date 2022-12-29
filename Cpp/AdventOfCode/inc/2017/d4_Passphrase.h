#pragma once

#include "Common.h"

constexpr bool IsValid(const std::string& line) {
    std::vector<std::string> words;
    auto split = Constexpr::Split(line, " ");
    for (const auto& sv : split) {
        words.push_back(std::string(sv));
    }
    std::sort(words.begin(), words.end());
    for (size_t i = 0; i < words.size() - 1; i++) {
        if (words[i] == words[i + 1]) return false;
    }

    return true;
}

static_assert(IsValid("aa bb cc"));
static_assert(!IsValid("aa bb aa"));
static_assert(IsValid("aa aaa"));

constexpr bool IsSecure(const std::string& line) {
    std::vector<std::string> words;
    auto split = Constexpr::Split(line, " ");
    for (const auto& sv : split) {
        auto word = std::string(sv);
        std::sort(word.begin(), word.end());
        words.push_back(word);
    }
    std::sort(words.begin(), words.end());
    for (size_t i = 0; i < words.size() - 1; i++) {
        if (words[i] == words[i + 1]) return false;
    }

    return true;
}

static_assert(IsSecure("abcde fghij"));
static_assert(!IsSecure("abcde xyz ecdab"));
static_assert(IsSecure("a ab abc abd abf abj"));

auto PartOne(const std::vector<std::string>& lines) {
    u32 count = 0;
    for (const auto& line : lines) {
        count += IsValid(line);
    }

    return count;
}

auto PartTwo(const std::vector<std::string>& lines) {
    u32 count = 0;
    for (const auto& line : lines) {
        count += IsSecure(line);
    }
    
    return count;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}