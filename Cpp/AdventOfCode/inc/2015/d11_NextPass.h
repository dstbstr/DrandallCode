#pragma once

#include <string>

#include "Platform/Types.h"

constexpr std::string Increment(const std::string& input) {
    std::string result = input;
    for(s32 i = static_cast<s32>(result.size()) - 1; i >= 0; i--) {
        if(result[i] == 'z') {
            result[i] = 'a';
        } else {
            //result[i] = static_cast<char>(result[i] + 1);
            result[i]++;
            break;
        }
    }

    return result;
}

static_assert(Increment("a") == "b");
static_assert(Increment("z") == "a");
static_assert(Increment("azzz") == "baaa");
static_assert(Increment("xyz") == "xza");

constexpr std::string ReplaceInvalid(const std::string& input) {
    std::string result = input;
    size_t index = 0;
    bool found = false;
    for(; index < result.size(); index++) {
        if(result[index] == 'i') {
            result[index] = 'j';
            found = true;
            break;
        }

        if(result[index] == 'o') {
            result[index] = 'p';
            found = true;
            break;
        }
        if(result[index] == 'l'){
            result[index] = 'm';
            found = true;
            break;
        } 
    }

    if(found) {
        index++;
        for(; index < result.size(); index++) {
            result[index] = 'a';
        }
    }

    return result;
}

static_assert(ReplaceInvalid("oil") == "paa");
static_assert(ReplaceInvalid("happy") == "happy");

constexpr bool ContainsStraight(const std::string& input) {
    for(auto i = 2; i < input.size(); i++) {
        if(input[i] == input[i-1] + 1 && input[i] == input[i-2] + 2) {
            return true;
        }
    }

    return false;
}

static_assert(ContainsStraight("abc") == true);
static_assert(ContainsStraight("aabbc") == false);
static_assert(ContainsStraight("aabcc") == true);
static_assert(ContainsStraight("cba") == false);
static_assert(ContainsStraight("aaa") == false);

constexpr bool ContainsTwoPairs(const std::string& input) {
    bool onePair = false;
    for(auto i = 1; i < input.size(); i++) {
        if(input[i] == input[i-1]) {
            if(onePair) {
                return true;
            } else {
                onePair = true;
                i++;
            }
        }
    }

    return false;
}

static_assert(ContainsTwoPairs("aabb") == true);
static_assert(ContainsTwoPairs("abcd") == false);
static_assert(ContainsTwoPairs("aaa") == false);
static_assert(ContainsTwoPairs("aaaa") == true);

constexpr bool IsValid(const std::string& input) {
    return ContainsStraight(input) && ContainsTwoPairs(input);
}

static_assert(IsValid("abbceffg") == false);
static_assert(IsValid("abcdffaa") == true);
static_assert(IsValid("ghjaabcc") == true);

constexpr std::string NextPass(std::string previous) {
    std::string result = previous;
    do {
        result = Increment(result);
        result = ReplaceInvalid(result);
    } while(!IsValid(result));

    return result;
}

//static_assert(NextPass("ghijklmn") == "ghjaabcc");
//static_assert(NextPass("vzbxkghb") == "vzbxxyzz");
//static_assert(NextPass("vzbxxyzz") == "vzcaabcc");
