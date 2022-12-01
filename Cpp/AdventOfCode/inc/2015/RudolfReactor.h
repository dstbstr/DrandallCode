#pragma once

#include "Common.h"

class Reactor {
    std::unordered_map<std::string, std::vector<std::string>> transforms{};
    std::unordered_map<std::string, std::string> reverseTransforms{};
    std::unordered_set<std::string> molecules{};

public:
    void ParseTransform(const std::string& line) {
        //Al => ThRnFAr
        static const auto re = std::regex(R"((\w+) => (\w+))");
        auto match = std::smatch{};

        if(std::regex_search(line, match, re)) {
            transforms[match[1].str()].push_back(match[2].str());
            reverseTransforms[match[2].str()] = match[1].str();
        }
    }

    void PopulateTransforms(const std::string& initial, u32 strLen) {
        for(auto i = 0; i < initial.size(); i++) {
            auto candidate = initial.substr(i, strLen);
            if(transforms.find(candidate) != transforms.end()) {
                for(const auto& replacement : transforms[candidate]) {
                    std::string molecule = initial.substr(0, i);
                    molecule += replacement;
                    if(initial.size() > strLen + i) {
                        molecule += initial.substr(i + strLen);
                    }
                    molecules.insert(molecule);
                }
            }
        }
    }

    u32 CountMolecules() {
        return static_cast<u32>(molecules.size());
    }

    bool CollapseMolecule(std::string& molecule) {
        static const std::vector<std::string> keys = [&]() {
            auto result = std::vector<std::string>{};
            for(const auto&[key, value] : reverseTransforms) {
                result.push_back(key);
            }

            std::sort(result.begin(), result.end(), [](std::string lhs, std::string rhs) { return lhs.size() > rhs.size(); });
            return result;
        }();

        for(const auto& key : keys) {
            if(molecule.find(key) != molecule.npos) {
                molecule = molecule.replace(molecule.find(key), key.size(), reverseTransforms[key]);
                return true;
            }
        }
        return false;
    }
};

#define ASSERT(cond) if(!(cond)) return false;
bool RunTests() {
    {
        auto reactor = Reactor{};
        reactor.ParseTransform("H => HO");
        reactor.ParseTransform("H => OH");
        reactor.ParseTransform("O => HH");

        reactor.PopulateTransforms("HOH", 1);

        ASSERT(reactor.CountMolecules() == 4);
    }

    {
        auto reactor = Reactor{};
        reactor.ParseTransform("H => OO");
        reactor.PopulateTransforms("H2O", 1);

        ASSERT(reactor.CountMolecules() == 1);
    }

    {
        auto reactor = Reactor{};
        reactor.ParseTransform("AA => AB");
        reactor.PopulateTransforms("AAA", 2);
        ASSERT(reactor.CountMolecules() == 2);
    }

    {
        auto reactor = Reactor{};
        reactor.ParseTransform("H => HO");
        reactor.ParseTransform("H => OH");
        reactor.ParseTransform("O => HH");
        reactor.ParseTransform("e => H");
        reactor.ParseTransform("e => O");

        auto initial = std::string("HOH");
        ASSERT(reactor.CollapseMolecule(initial));
        ASSERT(initial == "HH");
        ASSERT(reactor.CollapseMolecule(initial));
        ASSERT(initial == "O");
        ASSERT(reactor.CollapseMolecule(initial));
        ASSERT(initial == "e")
    }
    return true;
}

#undef ASSERT