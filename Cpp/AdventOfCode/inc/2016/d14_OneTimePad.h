#pragma once

#include "Common.h"

#include "md5Constexpr.h"

std::string Stretch(const std::string& hash) {
    auto running = hash;
    for (auto i = 0; i < 2016; i++) {
        running = ToHexLower(md5::compute(running.c_str()));
    }

    return running;
}

void PopulateCache(const std::string& salt, std::vector<std::pair<u32, std::string>>& threes, std::vector<std::pair<u32, std::string>>& fives, bool stretch) {
    static const auto triple = std::regex(R"((\w)\1\1)");
    static const auto fiver = std::regex(R"((\w)\1\1\1\1)");
    std::string candidate;
    std::string hash;
    std::string targetString;
    std::smatch match;

    for (int i = 0; i < 30000; i++) {
        candidate = salt + ToString(i);
        hash = ToHexLower(md5::compute(candidate.c_str()));
        if (stretch) {
            hash = Stretch(hash);
        }

        if (std::regex_search(hash, match, triple)) {
            targetString = "";
            while (targetString.size() < 5) {
                targetString += match[1].str();
            }
            threes.push_back(std::make_pair(i, targetString));
        }
        if (std::regex_search(hash, match, fiver)) {
            fives.push_back(std::make_pair(i, hash));
        }
    }
}

bool IsKey(u32 id, const std::string& targetString, const std::vector<std::pair<u32, std::string>>& fives) {
    size_t index = 0;
    while (fives[index].first <= id) {
        index++;
    }
    while (fives[index].first <= id + 1000) {
        if (fives[index].second.find(targetString) != fives[index].second.npos) {
            return true;
        }
        index++;
    }

    return false;
}

u32 FindKey64(const std::string& key, bool stretch) {
    std::vector<std::pair<u32, std::string>> threes;
    std::vector<std::pair<u32, std::string>> fives;
    PopulateCache(key, threes, fives, stretch);

    u32 foundKeys = 0;
    for (auto [id, target] : threes) {
        if (IsKey(id, target, fives)) {
            foundKeys++;
            if (foundKeys == 64) {
                return id;
            }
        }
    }

    return 0;

}

std::string Run(const std::vector<std::string>&) {
    return ToString(FindKey64("cuanljph", true));
}

bool RunTests() {
    if (FindKey64("abc", true) != 22551) return false;

    return true;
}