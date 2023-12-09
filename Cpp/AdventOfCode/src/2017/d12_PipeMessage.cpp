#include "2017/d12_PipeMessage.h"
#include "Algorithms/FloodFill.h"

SOLUTION(2017, 12) {
    constexpr std::vector<u32> ParseLine(std::string_view line) {
        auto rhs = std::string(Constexpr::Split(line, " <-> ")[1]);
        return ParseLineAsNumbers<u32>(rhs, ", ");
    }

    PART_ONE() {
        auto connections = ParseLines(lines, ParseLine);
        return Constexpr::ToString(FloodFill(0, [&connections](u32 pipe) { return connections[pipe]; }).size());
    }

    PART_TWO() {
        auto connections = ParseLines(lines, ParseLine);
        std::vector<u32> remainingGroups;
        for (auto i = 0; i < connections.size(); i++) {
            remainingGroups.push_back(i);
        }

        u32 groupCount = 0;
        while (!remainingGroups.empty()) {
            groupCount++;
            auto group = FloodFill(remainingGroups.back(), [&connections](u32 pipe) { return connections[pipe]; });
            std::erase_if(remainingGroups, [&group](u32 val) {
                return std::find(group.cbegin(), group.cend(), val) != group.cend();
                });
        }

        return Constexpr::ToString(groupCount);
    }

    TESTS() {
        static_assert(ParseLine("0 <-> 2").size() == 1);
        static_assert(ParseLine("0 <-> 2")[0] == 2u);
        static_assert(ParseLine("2 <-> 0, 3, 4").size() == 3);
        static_assert(ParseLine("2 <-> 0, 3, 4") == std::vector<u32>{0, 3, 4});
        
        return true;
    }
}