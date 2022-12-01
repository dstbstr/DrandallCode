#pragma once

#include "Common.h"

#include <optional>

struct Sue {
    std::optional<u32> Children;
    std::optional<u32> Cats;
    std::optional<u32> Samoyeds;
    std::optional<u32> Pomeranians;
    std::optional<u32> Akitas;
    std::optional<u32> Vizslas;
    std::optional<u32> Goldfish;
    std::optional<u32> Trees;
    std::optional<u32> Cars;
    std::optional<u32> Perfumes;
};

std::optional<u32> FindProperty(const std::string& line, const std::regex& re) {
    auto match = std::smatch{};
    if(std::regex_search(line, match, re)) {
        u32 num;
        ParseNumber(match[1].str(), num);
        return num;
    }

    return {};
}

Sue ParseSue(const std::string& line) {
    static const auto childRe = std::regex("children: (\\d+)");
    static const auto catRe = std::regex("cats: (\\d+)");
    static const auto samRe = std::regex("samoyeds: (\\d+)");
    static const auto pomRe = std::regex("pomeranians: (\\d+)");
    static const auto akitaRe = std::regex("akitas: (\\d+)");
    static const auto vizRe = std::regex("vizslas: (\\d+)");
    static const auto fishRe = std::regex("goldfish: (\\d+)");
    static const auto treeRe = std::regex("trees: (\\d+)");
    static const auto carRe = std::regex("cars: (\\d+)");
    static const auto perfumeRe = std::regex("perfumes: (\\d+)");

    Sue sue;

    sue.Children = FindProperty(line, childRe);
    sue.Cats = FindProperty(line, catRe);
    sue.Samoyeds = FindProperty(line, samRe);
    sue.Pomeranians = FindProperty(line, pomRe);
    sue.Akitas = FindProperty(line, akitaRe);
    sue.Vizslas = FindProperty(line, vizRe);
    sue.Goldfish = FindProperty(line, fishRe);
    sue.Trees = FindProperty(line, treeRe);
    sue.Cars = FindProperty(line, carRe);
    sue.Perfumes = FindProperty(line, perfumeRe);

    return sue;
}

#define CHECK_PROPERTY(_Prop) if(sue.##_Prop##.has_value()) { if(sue.##_Prop##.value() != targetSue.##_Prop##.value()) return false; }
constexpr bool IsPossibleMatch(const Sue& sue) {
    const Sue targetSue = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

    CHECK_PROPERTY(Children);
    CHECK_PROPERTY(Cats);
    CHECK_PROPERTY(Samoyeds);
    CHECK_PROPERTY(Pomeranians);
    CHECK_PROPERTY(Akitas);
    CHECK_PROPERTY(Vizslas);
    CHECK_PROPERTY(Goldfish);
    CHECK_PROPERTY(Trees);
    CHECK_PROPERTY(Cars);
    CHECK_PROPERTY(Perfumes);

    return true;
}

constexpr bool IsRealMatch(const Sue& sue) {
    const Sue targetSue = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

    CHECK_PROPERTY(Children);
    CHECK_PROPERTY(Samoyeds);
    CHECK_PROPERTY(Akitas);
    CHECK_PROPERTY(Vizslas);
    CHECK_PROPERTY(Cars);
    CHECK_PROPERTY(Perfumes);

    if(sue.Cats.has_value() && sue.Cats.value() <= targetSue.Cats.value()) return false;
    if(sue.Trees.has_value() && sue.Trees.value() <= targetSue.Trees.value()) return false;
    if(sue.Pomeranians.has_value() && sue.Pomeranians.value() >= targetSue.Pomeranians.value()) return false;
    if(sue.Goldfish.has_value() && sue.Goldfish.value() >= targetSue.Goldfish.value()) return false;

    return true;    
}

#undef CHECK_PROPERTY

#define ASSERT(cond) if(!(cond)) return false;
bool RunTests() {
    auto sue = ParseSue("Sue 1: goldfish: 6, trees: 9, akitas: 0");
    ASSERT(sue.Goldfish == 6u);
    ASSERT(sue.Trees == 9u);
    ASSERT(sue.Akitas == 0u);
    ASSERT(sue.Children.has_value() == false);

    Sue targetSue = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};
    ASSERT(IsPossibleMatch(targetSue));
    ASSERT(!IsPossibleMatch(sue));

    return true;
}
#undef ASSERT