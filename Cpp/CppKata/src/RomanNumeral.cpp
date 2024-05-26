#include "RomanNumeral.h"

#include "Core/Utilities/StringUtils.h"

#include <regex>
#include <unordered_map>

namespace {
    std::unordered_map<std::string, std::string> SpecialCases{
        {"DCCCCLXXXXVIIII", "IM"}, // 999
        {"DCCCCLXXXX", "XM"}, // 990
        {"DCCCC", "CM"}, // 900
        {"CCCCLXXXXVIIII", "ID"}, // 499
        {"CCCCLXXXX", "XD"}, // 490
        {"CCCC", "CD"}, // 400
        {"LXXXXVIIII", "IC"}, // 99
        {"LXXXX", "XC"}, // 90
        {"XXXXVIIII", "IL"}, // 49
        {"XXXX", "XL"}, // 40
        {"VIIII", "IX"}, // 9
        {"IIII", "IV"} // 4
    };

    std::string GetSimpleNumeral(uint32_t input) {
        int remainder = input;
        std::string result = "";

        while(remainder >= 1000) {
            result += "M";
            remainder -= 1000;
        }
        while(remainder >= 500) {
            result += "D";
            remainder -= 500;
        }
        while(remainder >= 100) {
            result += "C";
            remainder -= 100;
        }
        while(remainder >= 50) {
            result += "L";
            remainder -= 50;
        }
        while(remainder >= 10) {
            result += "X";
            remainder -= 10;
        }
        while(remainder >= 5) {
            result += "V";
            remainder -= 5;
        }
        while(remainder > 0) {
            result += "I";
            remainder -= 1;
        }

        return result;
    }

    uint32_t Extract(std::string romanNumeral) {
        std::string running = romanNumeral;
        uint32_t result = 0;

        while(StrUtil::StartsWith(running, "M")) {
            result += 1000;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "D")) {
            result += 500;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "C")) {
            result += 100;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "L")) {
            result += 50;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "X")) {
            result += 10;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "V")) {
            result += 5;
            running = running.substr(1);
        }
        while(StrUtil::StartsWith(running, "I")) {
            result += 1;
            running = running.substr(1);
        }

        return result;
    }

    void Simplify(std::string& numeral) {
        for(const auto& [simple, correct]: SpecialCases) {
            if(numeral.find(simple) != numeral.npos) {
                numeral.replace(numeral.find(simple), simple.length(), correct);
            }
        }
    }

    void UnSimplify(std::string& numeral) {
        for(const auto& [simple, correct]: SpecialCases) {
            if(numeral.find(correct) != numeral.npos) {
                numeral.replace(numeral.find(correct), correct.length(), simple);
            }
        }
    }
} // namespace

namespace RomanNumeral {
    std::string ToRoman(uint32_t input) {
        auto simple = GetSimpleNumeral(input);
        Simplify(simple);
        return simple;
    }

    uint32_t FromRoman(std::string input) {
        static const std::regex validator = std::regex{"^[IVXLDCM]+$"};
        if(!std::regex_match(input, validator)) {
            throw std::runtime_error(input + " doesn't look like a roman numeral");
        }

        UnSimplify(input);
        return Extract(input);
    }
} // namespace RomanNumeral