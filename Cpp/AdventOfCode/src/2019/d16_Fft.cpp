#include "2019/d16_Fft.h"

SOLUTION(2019, 16) {
    constexpr std::vector<s32> GetMultipliers(size_t inputSize, size_t row) {
        //0 1 0 -1
        std::vector<s32> multipliers = { 0, 1, 0, -1 };
        std::vector<s32> result;
        result.reserve(inputSize);
        while (result.size() < inputSize) {
            for (auto m : multipliers) {
                for (auto i = 0; i < row; i++) {
                    result.push_back(m);
                }
            }
        }

        std::rotate(result.begin(), result.begin() + 1, result.end());
        result.erase(result.begin() + inputSize, result.end());
        return result;
    }

    static_assert(GetMultipliers(8, 1) == std::vector<s32>{1, 0, -1, 0, 1, 0, -1, 0});
    static_assert(GetMultipliers(8, 2) == std::vector<s32>{0, 1, 1, 0, 0, -1, -1, 0});
    static_assert(GetMultipliers(8, 3) == std::vector<s32>{0, 0, 1, 1, 1, 0, 0, 0});
    static_assert(GetMultipliers(8, 4) == std::vector<s32>{0, 0, 0, 1, 1, 1, 1, 0});
    static_assert(GetMultipliers(8, 5) == std::vector<s32>{0, 0, 0, 0, 1, 1, 1, 1});

    constexpr std::string RunPhase(const std::string & str) {
        auto inputSize = str.size();
        std::string result;
        result.reserve(inputSize);

        for (size_t row = 0; row < str.size(); row++) {
            auto multipliers = GetMultipliers(inputSize, row + 1);
            s32 val = 0;
            for (size_t col = 0; col < inputSize; col++) {
                val += (str[col] - '0') * multipliers[col];
            }
            if (val < 0) val = -val;
            result.push_back((val % 10) + '0');
        }
        return result;
    }

    static_assert(RunPhase("12345678") == "48226158");

    constexpr std::string RunPhase2(const std::string & str) {
        auto inputSize = str.size();
        std::string result;
        result.reserve(inputSize);
        std::vector<u32> numbers;
        numbers.reserve(str.size());
        for (auto c : str) {
            numbers.push_back(static_cast<u32>(c - '0'));
        }

        for (size_t row = 0; row < str.size(); row++) {
            s32 val = 0;
            size_t col = row;
            size_t groupSize = row + 1;
            while (col < inputSize) {
                auto limit = std::min(groupSize, inputSize - col);
                for (auto i = 0; i < limit; i++) {
                    //val += (str[col++] - '0');
                    val += numbers[col++];
                }
                col += groupSize; //skip zeros
                if (col >= inputSize) break;
                limit = std::min(groupSize, inputSize - col);
                for (auto i = 0; i < limit; i++) {
                    //val -= (str[col++] - '0');
                    val -= numbers[col++];
                }
                col += groupSize; //skip zeros
            }
            if (val < 0) val = -val;
            result.push_back((val % 10) + '0');
        }

        return result;
    }

    //static_assert(RunPhase2("12345678") == "48226158");

    auto Part1(const std::string & line) {
        std::string running = line;
        for (auto i = 0; i < 100; i++) {
            running = RunPhase2(running);
        }

        return running.substr(0, 8);
    }

    auto Part2(const std::string & line) {
        std::string running = "";
        running.reserve(line.size() * 10'000);
        size_t offset;
        Constexpr::ParseNumber(line.substr(0, 7), offset);

        for (auto i = 0; i < 10'000; i++) {
            running += line;
        }

        running = running.substr(offset);
        std::reverse(running.begin(), running.end());
        for (auto i = 0; i < 100; i++) {
            for (auto c = 1; c < running.size(); c++) {
                char val = (running[c - 1] - '0') + (running[c] - '0');
                val %= 10;
                running[c] = val + '0';
            }
        }

        std::reverse(running.begin(), running.end());
        return running.substr(0, 8);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Part1(lines[0]);
        return Part2(lines[0]);
    }

    bool RunTests() {
        if (RunPhase2("12345678") != "48226158") return false;
        if (Part1("80871224585914546619083218645595") != "24176176") return false;
        if (Part2("03036732577212944063491565474664") != "84462026") return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}