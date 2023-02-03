#pragma once

#include "Common.h"

struct Ingrediant {
    std::string Name;
    u64 Cost;
};

using Produces = std::unordered_map<std::string, u64>;
using Ingrediants = std::unordered_map<std::string, std::vector<Ingrediant>>;
using Extras = std::unordered_map<std::string, u64>;

void ParseLines(const std::vector<std::string>& lines, Produces& outProduces, Ingrediants& outIngrediants) {
    for (const auto& line : lines) {
        auto s1 = StrUtil::Split(line, " => ");
        std::vector<Ingrediant> ingrediants;
        auto s2 = StrUtil::Split(s1[0], ", ");
        for (const auto elem : s2) {
            auto s3 = StrUtil::Split(elem, " ");
            u64 count;
            Constexpr::ParseNumber(s3[0], count);
            Ingrediant ingrediant = { std::string(s3[1]), count };
            ingrediants.push_back(ingrediant);
        }
        
        u64 quantity;
        auto s4 = StrUtil::Split(s1[1], " ");
        Constexpr::ParseNumber(s4[0], quantity);
        outIngrediants[std::string(s4[1])] = ingrediants;
        outProduces[std::string(s4[1])] = quantity;
    }
}

u64 TakeFromExtras(Extras& extras, const Ingrediant& ingrediant, u64 count) {
    const auto& material = ingrediant.Name;
    if (extras.contains(material)) {
        auto& extra = extras.at(material);
        if (count <= extra) {
            extra -= count;
            count = 0;
        }
        else {
            count -= extra;
            extras.erase(material);
        }
    }

    return count;
}

u64 GetProduction(u64 batchSize, u64 needed) {
    if (needed % batchSize == 0) { //need 7 As, produce 10 per batch
        return needed;
    }
    else {
        //7 / 10 = 0 + 1 == 1 * 10 = 10
        return ((needed / batchSize) + 1) * batchSize;
    }
}

u64 ProduceFuel(const Produces& produces, const Ingrediants& ingrediants, Extras& extras, u32 fuelToProduce) {
    u64 oreNeeded = 0;
    std::vector<Ingrediant> current;
    current.push_back(Ingrediant{ "FUEL", fuelToProduce });

    while (!current.empty()) {
        auto producing = current.back();
        current.pop_back();
        if (producing.Name == "ORE") {
            oreNeeded += producing.Cost;
        }
        else {
            auto producedPerCost = produces.at(producing.Name);
            const auto needed = TakeFromExtras(extras, producing, producing.Cost);
            if (needed == 0) continue;

            for (const auto& ingrediant : ingrediants.at(producing.Name)) {
                auto batchCost = ingrediant.Cost;

                auto toProduce = GetProduction(producedPerCost, needed);
                auto costToProduce = (toProduce * batchCost) / producedPerCost;
                auto overage = toProduce - needed;
                if (overage > 0) {
                    extras[producing.Name] = overage;
                }
                current.push_back({ ingrediant.Name, costToProduce });
            }
        }
    }

    return oreNeeded;
}

auto PartOne(const std::vector<std::string>& lines) {
    Produces produces;
    Ingrediants ingrediants;
    Extras extras;
    ParseLines(lines, produces, ingrediants);
    return ProduceFuel(produces, ingrediants, extras, 1);
}

auto PartTwo(const std::vector<std::string>& lines) {
    Produces produces;
    Ingrediants ingrediants;
    Extras extras;
    ParseLines(lines, produces, ingrediants);

    u64 remainingOre = 1'000'000'000'000;
    u32 fuelToProduce = 1'000'000;
    u32 totalFuelProduced = 0;
    while (fuelToProduce > 0) {
        Extras copyExtras = extras;
        auto oreCost = ProduceFuel(produces, ingrediants, copyExtras, fuelToProduce);
        if (oreCost <= remainingOre) {
            remainingOre -= oreCost;
            totalFuelProduced += fuelToProduce;
            extras = copyExtras;
        }
        else {
            fuelToProduce /= 10;
        }
    }

    return totalFuelProduced;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {  
    std::vector<std::string> lines = {
        "10 ORE => 10 A",
        "1 ORE => 1 B",
        "7 A, 1 B => 1 C",
        "7 A, 1 C => 1 D",
        "7 A, 1 D => 1 E",
        "7 A, 1 E => 1 FUEL"
    };

    if (PartOne(lines) != 31) return false;
    

    lines = {
        "9 ORE => 2 A",
        "8 ORE => 3 B",
        "7 ORE => 5 C",
        "3 A, 4 B => 1 AB",
        "5 B, 7 C => 1 BC",
        "4 C, 1 A => 1 CA",
        "2 AB, 3 BC, 4 CA => 1 FUEL"
    };
    if (PartOne(lines) != 165) return false;

    lines = {
        "157 ORE => 5 NZVS",
        "165 ORE => 6 DCFZ",
        "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL",
        "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ",
        "179 ORE => 7 PSHF",
        "177 ORE => 5 HKGWZ",
        "7 DCFZ, 7 PSHF => 2 XJWVT",
        "165 ORE => 2 GPVTF",
        "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT"
    };
    if (PartOne(lines) != 13312) return false;
    if (PartTwo(lines) != 82892753) return false;

    lines = {
        "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG",
        "17 NVRVD, 3 JNWZP => 8 VPVL",
        "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL",
        "22 VJHF, 37 MNCFX => 5 FWMGM",
        "139 ORE => 4 NVRVD",
        "144 ORE => 7 JNWZP",
        "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC",
        "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV",
        "145 ORE => 6 MNCFX",
        "1 NVRVD => 8 CXFTF",
        "1 VJHF, 6 MNCFX => 4 RFSQX",
        "176 ORE => 6 VJHF"
    };
    if (PartOne(lines) != 180697) return false;
    if (PartTwo(lines) != 5586022) return false;

    lines = {
        "171 ORE => 8 CNZTR",
        "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL",
        "114 ORE => 4 BHXH",
        "14 VRPVC => 6 BMBT",
        "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL",
        "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT",
        "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW",
        "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW",
        "5 BMBT => 4 WPTQ",
        "189 ORE => 9 KTJDG",
        "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP",
        "12 VRPVC, 27 CNZTR => 2 XDBXC",
        "15 KTJDG, 12 BHXH => 5 XCVML",
        "3 BHXH, 2 VRPVC => 7 MZWV",
        "121 ORE => 7 VRPVC",
        "7 XCVML => 6 RJRHP",
        "5 BHXH, 4 VRPVC => 5 LTCX"
    };
    if (PartOne(lines) != 2210736) return false;
    if (PartTwo(lines) != 460664) return false;

    return true;
}