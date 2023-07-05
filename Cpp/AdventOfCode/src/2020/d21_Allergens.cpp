#include "2020/d21_Allergens.h"

SOLUTION(2020, 21) {
    struct Food {
        std::vector<std::string> Ingrediants;
        std::vector<std::string> Allergens;
    };

    constexpr Food ParseFood(const std::string & line) {
        auto s1 = Constexpr::Split(line, "(contains ");
        auto s2 = Constexpr::Split(s1[0], " ");
        auto s3 = Constexpr::Split(s1[1], ", ");
        Food result;
        for (auto i : s2) {
            result.Ingrediants.push_back(std::string(i));
        }
        for (auto a : s3) {
            if (a.back() == ')') {
                result.Allergens.push_back(std::string(a.substr(0, a.size() - 1)));
            }
            else {
                result.Allergens.push_back(std::string(a));
            }
        }

        return result;
    }

    constexpr std::vector<Food> ParseInput(const std::vector<std::string>&lines) {
        std::vector<Food> result;
        for (const auto& line : lines) {
            result.push_back(ParseFood(line));
        }
        return result;
    }

    std::unordered_map<std::string, std::vector<std::string>> FindAllergens(const std::vector<Food>&foods) {
        std::unordered_map<std::string, std::vector<std::string>> allergens;

        //while not all allergens have 1 ingrediant
        while (true) {
            for (const auto& food : foods) {
                for (const auto& allergen : food.Allergens) {
                    if (allergens.contains(allergen)) {
                        //remove any ingrediants which don't appear in this food's ingrediant list
                        auto& potentials = allergens.at(allergen);
                        potentials.erase(std::remove_if(potentials.begin(), potentials.end(), [&](const std::string& ingrediant) {
                            return std::find(food.Ingrediants.begin(), food.Ingrediants.end(), ingrediant) == food.Ingrediants.end();
                            }), potentials.end());

                        if (potentials.size() == 1) {
                            auto toRemove = potentials[0];
                            for (auto& [a, ingrediants] : allergens) {
                                if (a == allergen) continue;
                                ingrediants.erase(std::remove(ingrediants.begin(), ingrediants.end(), toRemove), ingrediants.end());
                            }
                        }
                    }
                    else {
                        allergens[allergen] = food.Ingrediants;
                    }
                }
            }

            if (std::count_if(allergens.cbegin(), allergens.cend(), [](const auto& kvp) {
                return kvp.second.size() > 1;
                }) == 0) {
                break;
            }
        }

        return allergens;
    }
    auto Part1(const std::vector<std::string>&lines) {
        auto foods = ParseInput(lines);
        auto allergens = FindAllergens(foods);

        std::vector<std::string> toAvoid;
        for (const auto& [allergen, ingrediants] : allergens) {
            toAvoid.push_back(ingrediants[0]);
        }

        size_t result = 0;
        for (const auto& food : foods) {
            for (const auto& ingrediant : food.Ingrediants) {
                result += std::find(toAvoid.begin(), toAvoid.end(), ingrediant) == toAvoid.end();
            }
        }

        return result;
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto foods = ParseInput(lines);
        auto allergens = FindAllergens(foods);

        std::vector<std::string> allergenNames;
        for (const auto& [allergen, ingrediants] : allergens) {
            allergenNames.push_back(allergen);
        }

        std::sort(allergenNames.begin(), allergenNames.end());

        std::vector<std::string> dangerous;
        dangerous.resize(allergenNames.size());

        for (const auto& [allergen, ingrediants] : allergens) {
            size_t targetIndex = 0;
            for (; targetIndex < allergenNames.size(); targetIndex++) {
                if (allergenNames[targetIndex] == allergen) break;
            }
            dangerous[targetIndex] = ingrediants[0];
        }

        std::string result = StrUtil::JoinVec(',', dangerous);
        return result;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Part2(lines);
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "mxmxvkd kfcds sqjhc nhms(contains dairy, fish)",
            "trh fvjkl sbzzf mxmxvkd(contains dairy)",
            "sqjhc fvjkl(contains soy)",
            "sqjhc mxmxvkd sbzzf(contains fish)"
        };

        if (Part1(lines) != 5) return false;
        if (Part2(lines) != "mxmxvkd,sqjhc,fvjkl") return false;
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