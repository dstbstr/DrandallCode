#pragma once

#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <numeric>
#include <execution>
#include <exception>

#include "Platform/Types.h"
#include "Utils.h"
#include "external/rapidjson/rapidjson.h"
#include "external/rapidjson/document.h"

std::vector<s32> ExtractNumbers(std::string json) {
    auto numberRegex = std::regex("-?\\d+");
    std::vector<s32> result;

    auto begin = std::sregex_iterator(json.begin(), json.end(), numberRegex);
    auto end = std::sregex_iterator{};

    for(std::sregex_iterator i = begin; i != end; ++i) {
        s32 num;
        ParseNumber(i->str(), num);
        result.push_back(num);
    }

    return result;
}

s32 SumNumbers(std::string json) {
    auto numbers = ExtractNumbers(json);

    return std::reduce(std::execution::par, numbers.cbegin(), numbers.cend(), 0, [](s32 lhs, s32 rhs) { return lhs + rhs;});
}

s32 CountAll(rapidjson::GenericArray<false, rapidjson::Value> arr);
s32 CountAll(rapidjson::GenericObject<false, rapidjson::Value> obj);

s32 Calculate(rapidjson::Value& val) {

    if(val.IsInt()) {
        return val.GetInt();
    } else if(val.IsArray()) {
        return CountAll(val.GetArray());
    } else if(val.IsObject()) {
        return CountAll(val.GetObject());
    }
    return 0;
}

s32 CountAll(rapidjson::GenericObject<false, rapidjson::Value> obj) {
    s32 result = 0;
    for(auto& elem : obj) {
        result += Calculate(elem.value);
    }

    return result;
}

s32 CountAll(rapidjson::GenericArray<false, rapidjson::Value> arr) {
    s32 result = 0;
    for(auto& elem : arr) {
        result += Calculate(elem);
    }

    return result;
}

s32 CountAll(const std::string& json) {
    using namespace rapidjson;
    Document document;
    document.Parse(json.c_str());
    if(document.HasParseError()) {
        throw std::logic_error("Failed to parse json");
    }

    s32 result = 0;
    if(document.IsArray()) {
        result += CountAll(document.GetArray());
    } else if(document.IsObject()) {
        result += CountAll(document.GetObject());
    }

    return result;
}

bool IsRed(const rapidjson::Value& value) {
    return value.IsString() && strcmp(value.GetString(), "red") == 0;
}

s32 CountNonReds(rapidjson::GenericArray<false, rapidjson::Value> arr);
s32 CountNonReds(rapidjson::GenericObject<false, rapidjson::Value> obj);

s32 CalculateNonReds(rapidjson::Value& val) {
    if (val.IsInt()) {
        return val.GetInt();
    } else if(val.IsArray()) {
        return CountNonReds(val.GetArray());
    } else if(val.IsObject()) {
        return CountNonReds(val.GetObject());
    }

    return 0;
}

s32 CountNonReds(rapidjson::GenericObject<false, rapidjson::Value> obj) {
    s32 result = 0;
    for(auto& elem : obj) {
        if(IsRed(elem.value)) {
            return 0;
        } else {
            result += CalculateNonReds(elem.value);
        }
    }

    return result;
}

s32 CountNonReds(rapidjson::GenericArray<false, rapidjson::Value> arr) {
    s32 result = 0;
    for(auto& elem : arr) {
        result += CalculateNonReds(elem);
    }

    return result;
}

s32 CountNonReds(std::string json) {
    using namespace rapidjson;
    Document document;
    document.Parse(json.c_str());
    if(document.HasParseError()) {
        throw std::logic_error("Failed to parse json");
    }

    s32 result = 0;
    if(document.IsArray()) {
        result += CountNonReds(document.GetArray());
    } else if(document.IsObject()) {
        result += CountNonReds(document.GetObject());
    }

    return result;
}

#define ASSERT(cond) if(!(cond)) return false;

bool RunJsTests() {
    ASSERT(CountNonReds("[1,2,3]") == 6);
    ASSERT(CountNonReds(R"([1,{"c":"red","b":2},3])") == 4);
    ASSERT(CountNonReds(R"({"d":"red","e":[1,2,3,4],"f":5})") == 0);
    ASSERT(CountNonReds(R"([1,"red",5])") == 6);
    return true;
}

#undef ASSERT