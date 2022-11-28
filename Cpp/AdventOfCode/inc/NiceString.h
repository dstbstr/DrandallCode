#pragma once
#include <string>
#include <string_view>
#include <regex>
#include <iostream>

static constexpr auto naughtyPairs = {"ab", "cd", "pq", "xy"};

std::regex ThreeVowels = std::regex("\\w*[aeiou]\\w*[aeiou]\\w*[aeiou]\\w*");
std::regex DoubleLetter = std::regex("(\\w)\\1");
std::regex NaughtyLetters = std::regex("(?:ab)|(?:cd)|(?:pq)|(?:xy)");

bool IsNice(const std::string& input) {
    return std::regex_search(input, ThreeVowels) && 
        std::regex_search(input, DoubleLetter) &&
        !std::regex_search(input, NaughtyLetters);
}

bool RunTestCases() {
    return IsNice("ugknbfddgicrmopn") &&
        IsNice("aaa") &&
        !IsNice("jchzalrnumimnmhp") &&
        !IsNice("haegwjzuvuyypxyu") &&
        !IsNice("dvszwmarrgswjxmb");
}

std::regex DoublePair = std::regex("(\\w\\w)\\w*\\1");
std::regex SplitDouble = std::regex("(\\w)\\w\\1");

bool IsReallyNice(const std::string& input) {
    return std::regex_search(input, DoublePair) &&
        std::regex_search(input, SplitDouble);
}

bool RunReallyNiceTestCases() {
    return IsReallyNice("qjhvhtzxzqqjkmpb") &&
        IsReallyNice("xxyxx") &&
        !IsReallyNice("uurcxstgmygtbstg") &&
        !IsReallyNice("ieodomkazucvgmuy") &&
        !IsReallyNice("aaa");
}