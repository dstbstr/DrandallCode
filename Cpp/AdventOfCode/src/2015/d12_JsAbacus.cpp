#pragma once

#include "2015/d12_JsAbacus.h"

SOLUTION(2015, 12) {
    struct Json {
        constexpr Json() {}
        constexpr Json(s32 number) : NumericValue(number), IsNumber(true) {}
        constexpr Json(const std::string& str) : StringValue(str), IsString(true) {}
        constexpr Json(std::string_view str) : StringValue(std::string(str)), IsString(true) {}

        std::vector<Json> Elements;
        Constexpr::SmallMap<std::string, Json> Objects;
        std::string StringValue{ "" };
        s32 NumericValue{ 0 };
        bool IsString{ false };
        bool IsNumber{ false };
    };

    constexpr Json ParseJson(std::string_view json, size_t& index) {
        if (json[index] == '[') { //Parse Array
            index++; //Consume open bracket
            Json result;
            while (json[index] != ']') {
                result.Elements.push_back(ParseJson(json, index));
            }

            index++; //Consume close bracket
            if (index < json.size() && json[index] == ',') index++; //may be in an array
            return result;
        }
        else if (json[index] == '{') { //Parse Object
            index++; //Consume open curly
            Json result;
            while (json[index] != '}') {
                auto key = ParseJson(json, index).StringValue;
                index++;
                auto value = ParseJson(json, index);
                result.Objects[key] = value;
            }
            index++; //Consome close curly
            if (index < json.size() && json[index] == ',') index++; //may be in an array
            return result;
        }
        else if (json[index] == '"') { //Parse String
            auto closeQuote = json.find("\"", index + 1);
            Json result = Json(json.substr(index + 1, (closeQuote - index) -1 ));
            index = closeQuote + 1;
            if (json[index] == ',') index++;
            return result;
        }
        else { //Parse Number
            auto nextComma = json.find(",", index);
            auto nextClose = json.find_first_of("]}", index);

            s32 val;
            if (nextClose < nextComma) {
                Constexpr::ParseNumber(json.substr(index, nextClose - index), val);
                index = nextClose;
            }
            else {
                Constexpr::ParseNumber(json.substr(index, nextComma - index), val);
                index = nextComma + 1;
            }
            return Json(val);
        }
    }

    constexpr size_t CountAll(const Json& json, std::string_view excludedValue) {
        if (json.IsNumber) return json.NumericValue;
        else if (json.IsString) return 0;
        else if (!json.Elements.empty()) {
            size_t result = 0;
            for (const auto& element : json.Elements) {
                result += CountAll(element, excludedValue);
            }
            return result;
        }
        else if (!json.Objects.is_empty()) {
            size_t result = 0;
            for (const auto& [key, value] : json.Objects) {
                if (value.IsString && value.StringValue == excludedValue) return 0;
                result += CountAll(value, excludedValue);
            }
            return result;
        }
        return 0;
    }

    PART_ONE() {
        size_t index = 0;
        auto root = ParseJson(Line, index);

        return Constexpr::ToString(CountAll(root, ""));
    }

    PART_TWO() {
        size_t index = 0;
        auto root = ParseJson(Line, index);

        return Constexpr::ToString(CountAll(root, "red"));
    }
    TESTS() {
        size_t index = 0;
        auto root = ParseJson("[1,2,3]", index);
        if (root.Elements.size() != 3) return false;
        if (!root.Elements[0].IsNumber || root.Elements[0].NumericValue != 1) return false;

        index = 0;
        root = ParseJson("[[[3]]]", index);
        if (root.Elements.size() != 1) return false;
        if (root.Elements[0].Elements[0].Elements[0].NumericValue != 3) return false;

        index = 0;
        root = ParseJson(R"(["abc","def","ghi"])", index);
        if (root.Elements.size() != 3) return false;
        if (!root.Elements[0].IsString) return false;
        if (root.Elements[0].StringValue != "abc") return false;

        index = 0;
        root = ParseJson(R"([["abc", "def"],[123, 345]])", index);
        if (root.Elements.size() != 2) return false;
        if (root.Elements[0].Elements.size() != 2) return false;
        if (root.Elements[0].Elements[0].StringValue != "abc") return false;
        if (root.Elements[1].Elements.size() != 2) return false;
        if (root.Elements[1].Elements[0].NumericValue != 123) return false;

        index = 0;
        root = ParseJson(R"({"a":2,"b":4})", index);
        if (root.Objects.size() != 2) return false;
        if (root.Objects.at("a").NumericValue != 2) return false;
        if (root.Objects.at("b").NumericValue != 4) return false;

        index = 0;
        root = ParseJson(R"({"a":{"b":4},"c":-1})", index);
        if (root.Objects.size() != 2) return false;
        if (root.Objects.at("a").Objects.size() != 1) return false;
        if (root.Objects.at("a").Objects.at("b").NumericValue != 4) return false;

        index = 0;
        root = ParseJson("[]", index);
        if (root.IsNumber) return false;
        if (root.IsString) return false;
        if (!root.Elements.empty()) return false;
        if (!root.Objects.is_empty()) return false;

        index = 0;
        root = ParseJson("{}", index);
        if (root.IsNumber) return false;
        if (root.IsString) return false;
        if (!root.Elements.empty()) return false;
        if (!root.Objects.is_empty()) return false;

        return true;
    }
}