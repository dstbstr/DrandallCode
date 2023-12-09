#include "2020/d16_Tickets.h"

SOLUTION(2020, 16) {
    using Ticket = std::vector<size_t>;

    struct Field {
        std::string Name;
        std::pair<size_t, size_t> LowRange;
        std::pair<size_t, size_t> HighRange;

        constexpr bool Supports(size_t value) const {
            return (LowRange.first <= value && LowRange.second >= value) ||
                (HighRange.first <= value && HighRange.second >= value);
        }
    };

    constexpr std::pair<size_t, size_t> ParseRange(std::string_view range) {
        auto split = Constexpr::Split(range, "-");
        size_t min, max;
        Constexpr::ParseNumber(split[0], min);
        Constexpr::ParseNumber(split[1], max);
        return std::make_pair(min, max);
    }

    constexpr Field ParseField(std::string_view line) {
        Field field;
        auto split = Constexpr::Split(line, ": ");
        field.Name = split[0];

        auto split2 = Constexpr::Split(split[1], " or ");
        field.LowRange = ParseRange(split2[0]);
        field.HighRange = ParseRange(split2[1]);

        return field;
    }

    constexpr std::array<bool, 1000> FindInvalidNumbers(const std::vector<Field>&fields) {
        std::array<bool, 1000> result{};
        result.fill(true);
        for (const auto& field : fields) {
            for (auto i = field.LowRange.first; i <= field.LowRange.second; i++) {
                result[i] = false;
            }
            for (auto i = field.HighRange.first; i <= field.HighRange.second; i++) {
                result[i] = false;
            }
        }

        return result;
    }

    constexpr Ticket GetMyTicket(const auto& lines) {
        Ticket result;
        auto split = Constexpr::Split(lines[1], ",");
        for (const auto& numStr : split) {
            size_t num;
            Constexpr::ParseNumber(numStr, num);
            result.push_back(num);
        }

        return result;
    }

    constexpr std::vector<Ticket> GetNearbyTickets(const auto& lines) {
        std::vector<Ticket> result;
        Ticket currentTicket;
        for (const auto& line : lines) {
            if (line[0] < '0' || line[0] > '9') continue;
            auto split = Constexpr::Split(line, ",");
            for (auto numStr : split) {
                size_t num;
                Constexpr::ParseNumber(numStr, num);
                currentTicket.push_back(num);
            }
            result.push_back(currentTicket);
            currentTicket.clear();
        }

        return result;
    }

    constexpr std::vector<Ticket> GetValidTickets(const auto& lines, const std::vector<Field>&fields) {
        auto invalidNumbers = FindInvalidNumbers(fields);
        auto otherTickets = GetNearbyTickets(lines);

        std::vector<Ticket> result;
        for (const auto& ticket : otherTickets) {
            bool isValid = true;
            for (auto value : ticket) {
                isValid &= !invalidNumbers[value];
            }

            if (isValid) {
                result.push_back(ticket);
            }
        }

        return result;
    }

    constexpr Constexpr::SmallMap<std::string, size_t> UntangleFields(const std::vector<Field>&fields, const std::vector<Ticket>&tickets) {
        std::vector<std::vector<std::string>> available;
        for (auto i = 0; i < fields.size(); i++) {
            std::vector<std::string> v;
            available.push_back(v);
        }
        for (auto& v : available) {
            for (const auto& field : fields) {
                v.push_back(field.Name);
            }
        }

        for (const auto& ticket : tickets) {
            for (size_t i = 0; i < ticket.size(); i++) {
                for (const auto& field : fields) {
                    if (!field.Supports(ticket[i])) {
                        available[i].erase(std::remove(available[i].begin(), available[i].end(), field.Name));
                    }
                }
            }
        }

        while (std::count_if(available.begin(), available.end(), [](const std::vector<std::string>& v) {
            return v.size() > 1;
            })) {
            std::vector<std::string> toRemove;
            for (const auto& v : available) {
                if (v.size() == 1) {
                    toRemove.push_back(v[0]);
                }
            }
            for (auto& v : available) {
                if (v.size() > 1) {
                    v.erase(std::find_if(v.begin(), v.end(), [&toRemove](const std::string& s) {
                        return std::find(toRemove.cbegin(), toRemove.cend(), s) != toRemove.cend();
                        }));
                }
            }
        }

        Constexpr::SmallMap<std::string, size_t> result;
        for (size_t i = 0; i < available.size(); i++) {
            result[available[i][0]] = i;
        }

        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto fields = ParseLines(groups[0], ParseField);
        auto invalidNumbers = FindInvalidNumbers(fields);
        auto otherTickets = GetNearbyTickets(groups[2]);

        size_t result = 0;
        for (const auto& ticket : otherTickets) {
            for (auto value : ticket) {
                result += value * invalidNumbers[value];
            }
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto fields = ParseLines(groups[0], ParseField);
        auto myTicket = GetMyTicket(groups[1]);
        auto tickets = GetValidTickets(groups[2], fields);

        auto fieldMapping = UntangleFields(fields, tickets);

        size_t result = 1;
        for (const auto& [name, index] : fieldMapping) {
            if (StrUtil::StartsWith(name, "departure")) {
                result *= myTicket[index];
            }
        }

        return result;
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
           "class: 1-3 or 5-7",
           "row: 6-11 or 33-44",
           "seat: 13-40 or 45-50",
           "",
           "your ticket:",
           "7,1,14",
           "",
           "nearby tickets:",
           "7,3,47",
           "40,4,50",
           "55,2,20",
           "38,6,12"
        };

        if (SolvePartOne(lines) != 71) return false;

        lines = {
            "class: 0-1 or 4-19",
            "row: 0-5 or 8-19",
            "seat: 0-13 or 16-19",
            "",
            "your ticket:",
            "11,12,13",
            "",
            "nearby tickets:",
            "3,9,18",
            "15,1,5",
            "5,14,9"
        };

        if (SolvePartTwo(lines) != 1) return false;
        return true;
    }
}