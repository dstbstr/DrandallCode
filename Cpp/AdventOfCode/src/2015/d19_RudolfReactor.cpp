#pragma once

#include "2015/d19_RudolfReactor.h"

SOLUTION(2015, 19) {
    using Transforms = Constexpr::SmallMap<std::string, std::vector<std::string>>;
    using RTransforms = Constexpr::SmallMap<std::string, std::string>;

    constexpr void ParseTransform(const std::string & line, Transforms& transforms, RTransforms& reverseTransforms) {
        auto s = Constexpr::Split(line, " => ");
        auto from = std::string(s[0]);
        auto to = std::string(s[1]);
        
        transforms[from].push_back(to);
        reverseTransforms[to] = from;
    }

    constexpr void ExtractMolecules(const std::string& initial, u32 strLen, const Transforms& transforms, Constexpr::BigSet<std::string>& outMolecules) {
        for (size_t i = 0; i < initial.size(); i++) {
            auto candidate = initial.substr(i, strLen);
            if (transforms.contains(candidate)) {
                for (const auto& replacement : transforms.at(candidate)) {
                    auto molecule = initial.substr(0, i) + replacement;
                    if (initial.size() > strLen + i) {
                        molecule += initial.substr(i + strLen);
                    }
                    outMolecules.insert(molecule);
                }
            }
        }
    }

    constexpr bool CollapseMolecule(std::string& molecule, const RTransforms& reverseTransforms) {
        const auto keys = reverseTransforms.GetKeys();
        for (const auto& key : keys) {
            if (molecule.find(key) != molecule.npos) {
                molecule = molecule.replace(molecule.find(key), key.size(), reverseTransforms.at(key));
                return true;
            }
        }

        return false;
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);
        Transforms transforms;
        RTransforms reverseTransforms;
        for (const auto& line : groups[0]) {
            ParseTransform(line, transforms, reverseTransforms);
        }

        Constexpr::BigSet<std::string> molecules;
        ExtractMolecules(groups[1][0], 1, transforms, molecules);
        ExtractMolecules(groups[1][0], 2, transforms, molecules);

        return Constexpr::ToString(molecules.size());
    }
    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);
        Transforms transforms;
        RTransforms reverseTransforms;
        for (const auto& line : groups[0]) {
            ParseTransform(line, transforms, reverseTransforms);
        }

        size_t steps = 0;
        std::string running = groups[1][0];
        while (true) {
            if (!CollapseMolecule(running, reverseTransforms)) break;
            steps++;
        }
        return Constexpr::ToString(steps);
    }
    TESTS() {
        std::vector<std::string> lines = {
            "H => HO",
            "H => OH",
            "O => HH",
            "",
            "HOH"
        };

        if (PartOne(lines) != "4") return false;
        return true;
    }
}