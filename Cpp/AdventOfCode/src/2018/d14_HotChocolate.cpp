#include "2018/d14_HotChocolate.h"

SOLUTION(2018, 14) {
    constexpr void Tick(std::string & current, size_t & r1, size_t & r2) {
        char lhs = current[r1] - '0';
        char rhs = current[r2] - '0';
        char sum = lhs + rhs;
        if (sum < 10) {
            current += static_cast<char>(('0' + sum));
        }
        else {
            current += '1';
            current += static_cast<char>(('0' + (sum - 10)));
        }

        r1 = (lhs + r1 + 1) % current.size();
        r2 = (rhs + r2 + 1) % current.size();
    }

    PART_ONE() {
        size_t count;
        Constexpr::ParseNumber(lines[0], count);
        std::string current = "37";
        current.reserve(count);
        size_t r1 = 0;
        size_t r2 = 1;
        while (current.size() < count + 10) {
            Tick(current, r1, r2);
        }

        return current.substr(count, 10);
    }

    PART_TWO() {
        const auto& target = lines[0];
        std::string current = "37";
        current.reserve(10'000'000);
        size_t r1 = 0;
        size_t r2 = 1;
        do {
            for (auto i = 0; i < 10000; i++) {
                Tick(current, r1, r2);
            }
        } while (current.find(target, current.size() - 10000) == current.npos);

        return Constexpr::ToString(current.find(target));
    }

    TESTS() {
        if (PartOne({ "5" }) != "0124515891") return false;
        if (PartOne({"9"}) != "5158916779") return false;
        if (PartOne({"18"}) != "9251071085") return false;
        if (PartOne({ "2018" }) != "5941429882") return false;

        if (PartTwo({"51589"}) != "9") return false;
        if (PartTwo({"01245"}) != "5") return false;
        if (PartTwo({"92510"}) != "18") return false;
        if (PartTwo({ "59414" }) != "2018") return false;

        return true;
    }
}