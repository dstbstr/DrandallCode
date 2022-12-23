#pragma once

#include "Common.h"

#include <iostream>

enum struct Op { Plus, Minus, Mul, Div};

std::string ToString(Op op) {
    switch (op) {
    case Op::Plus: return "+";
    case Op::Minus: return "-";
    case Op::Mul: return "*";
    case Op::Div: return "/";
    default: return "";
    }
}

struct Monkey {
    std::string Name = "";
    u64 Value = 0;
    bool ValueSet = false;
    std::string Lhs = "";
    std::string Rhs = "";
    Op Operation = Op::Plus;
};

constexpr Monkey ParseLine(const std::string& line) {
    Monkey monkey{};
    monkey.Name = line.substr(0, 4);
    if (line.size() < 10) {
        Constexpr::ParseNumber(line.substr(6), monkey.Value);
        monkey.ValueSet = true;
    }
    else {
        monkey.Lhs = line.substr(6, 4);
        monkey.Rhs = line.substr(13);
        auto op = line[11];
        switch (op) {
        case '+': monkey.Operation = Op::Plus; break;
        case '-': monkey.Operation = Op::Minus; break;
        case '/': monkey.Operation = Op::Div; break;
        case '*': monkey.Operation = Op::Mul; break;
        }
    }

    return monkey;
}

static_assert(ParseLine("root: pppw + sjmn").Name == "root");
static_assert(ParseLine("root: pppw + sjmn").Operation == Op::Plus);
static_assert(ParseLine("abcd: 42").Name == "abcd");
static_assert(ParseLine("abcd: 42").Value == 42);

constexpr u64 CalcValue(const Monkey& worker, const Monkey& lhs, const Monkey& rhs) {
    switch (worker.Operation) {
    case Op::Plus: return lhs.Value + rhs.Value;
    case Op::Minus: return lhs.Value - rhs.Value;
    case Op::Mul: return lhs.Value * rhs.Value;
    case Op::Div: return lhs.Value / rhs.Value;
    default: return 0;
    }
}

constexpr void RunMonkeys(std::vector<Monkey>& monkeys, const std::unordered_map<std::string, size_t>& monkeyMap, size_t rootIndex) {
    while (monkeys[rootIndex].ValueSet == false) {
        for (auto& monkey : monkeys) {
            if (monkey.ValueSet) continue;

            auto leftIndex = monkeyMap.at(monkey.Lhs);
            auto rightIndex = monkeyMap.at(monkey.Rhs);
            if (monkeys[leftIndex].ValueSet && monkeys[rightIndex].ValueSet) {
                monkey.Value = CalcValue(monkey, monkeys[leftIndex], monkeys[rightIndex]);
                monkey.ValueSet = true;
            }
        }
    }
}

u64 PartOne(const std::vector<std::string>& lines) {
    std::vector<Monkey> monkeys;
    size_t rootIndex = 0;
    for (const auto& line : lines) {
        monkeys.push_back(ParseLine(line));
    }

    std::unordered_map<std::string, size_t> monkeyMap;
    for (auto i = 0; i < monkeys.size(); i++) {
        monkeyMap[monkeys[i].Name] = i;
        if (monkeys[i].Name == "root") {
            rootIndex = i;
        }
    }

    RunMonkeys(monkeys, monkeyMap, rootIndex);
    return monkeys[rootIndex].Value;
}

void Reduce(std::string& equation) {
    const static auto re = std::regex(R"(\(?(\d+)([\/\-\+\*])(\d+)\)?)");
    std::smatch match;

    for (auto i = 0; i < equation.size(); i++) {
        if (equation[i] == ')') {
            auto openParen = equation.substr(0, i).find_last_of('(');
            auto expression = equation.substr(openParen, i - openParen);
            if (std::regex_search(expression, match, re)) {
                u64 combined = 0;
                u64 lhs;
                u64 rhs;
                ParseNumber(match[1].str(), lhs);
                ParseNumber(match[3].str(), rhs);
                switch (match[2].str()[0]) {
                case '+': combined = lhs + rhs; break;
                case '-': combined = lhs - rhs; break;
                case '*': combined = lhs * rhs; break;
                case '/': combined = lhs / rhs; break;
                }

                equation = std::regex_replace(equation, re, ToString(combined));
                return;
            }
        }
    }
}

u64 PartTwo(const std::vector<std::string>& lines) {
    std::vector<Monkey> monkeys;
    size_t rootIndex = 0;
    size_t humanIndex = 0;
    size_t leftMonkeyIndex = 0;
    size_t rightMonkeyIndex = 0;

    for (const auto& line : lines) {
        monkeys.push_back(ParseLine(line));
    }

    std::unordered_map<std::string, size_t> monkeyMap;
    for (auto i = 0; i < monkeys.size(); i++) {
        monkeyMap[monkeys[i].Name] = i;
        if (monkeys[i].Name == "root") {
            rootIndex = i;
        }
        else if (monkeys[i].Name == "humn") {
            humanIndex = i;
        }
    }

    leftMonkeyIndex = monkeyMap[monkeys[rootIndex].Lhs];
    rightMonkeyIndex = monkeyMap[monkeys[rootIndex].Rhs];

    
    u32 added = 1;
    while (added > 0) {
        added = 0;
        for (auto& monkey : monkeys) {
            if (monkey.ValueSet) continue;

            auto leftIndex = monkeyMap.at(monkey.Lhs);
            auto rightIndex = monkeyMap.at(monkey.Rhs);
            if (monkeys[leftIndex].ValueSet && monkeys[rightIndex].ValueSet) {
                if (leftIndex == humanIndex || rightIndex == humanIndex) continue;
                added++;
                monkey.Value = CalcValue(monkey, monkeys[leftIndex], monkeys[rightIndex]);
                monkey.ValueSet = true;
            }
        }
    }

    monkeys[humanIndex].ValueSet = false;
    monkeys[humanIndex].Value = 0;

    auto& currentMonkey = monkeys[rootIndex];
    currentMonkey.Operation = Op::Minus;

    while (true) {
        u64 targetValue = currentMonkey.Value;
        leftMonkeyIndex = monkeyMap[currentMonkey.Lhs];
        rightMonkeyIndex = monkeyMap[currentMonkey.Rhs];

        u64 knownValue = monkeys[leftMonkeyIndex].ValueSet ? monkeys[leftMonkeyIndex].Value : monkeys[rightMonkeyIndex].Value;
        auto& nextMonkey = monkeys[leftMonkeyIndex].ValueSet ? monkeys[rightMonkeyIndex] : monkeys[leftMonkeyIndex];
        u64 nextValue = 0;
        switch (currentMonkey.Operation) {
        case Op::Plus: nextValue = targetValue - knownValue; break;
        case Op::Minus: nextValue = targetValue + knownValue; break;
        case Op::Mul: nextValue = targetValue / knownValue; break;
        case Op::Div: nextValue = targetValue * knownValue; break;
        default: break;
        }

        switch (currentMonkey.Operation) {
        case Op::Plus: std::cout << nextValue << " + " << knownValue << " == " << targetValue << "\n"; break;
        case Op::Minus: std::cout << nextValue << " - " << knownValue << " == " << targetValue << "\n"; break;
        case Op::Mul: std::cout << nextValue << " * " << knownValue << " == " << targetValue << "\n"; break;
        case Op::Div: std::cout << nextValue << " / " << knownValue << " == " << targetValue << "\n"; break;
        default:break;
        }

        nextMonkey.Value = nextValue;
        nextMonkey.ValueSet = true;
        if (nextMonkey.Name == "humn") {
            break;
        }
        currentMonkey = nextMonkey;
    }

    return monkeys[humanIndex].Value;
    

    //8759966720571 is too high

    /*
    std::vector<std::string> tokens = {
        "(" + monkeys[rootIndex].Lhs + ")",
        "==",
        "(" + monkeys[rootIndex].Rhs + ")"
    };

    std::vector<std::string> nextTokens;
    while (true) {
        for (auto& token : tokens) {
            if (token.size() < 4) {
                nextTokens.push_back(token);
                continue;
            }

            auto monkey = monkeys[monkeyMap[token.substr(1, 4)]];
            if (monkey.Name == "humn") {
                nextTokens.push_back("X");
            }
            else if (monkey.ValueSet) {
                nextTokens.push_back(ToString(monkey.Value));
            }
            else {
                nextTokens.push_back("(");
                nextTokens.push_back("(" + monkey.Lhs + ")");
                nextTokens.push_back(ToString(monkey.Operation));
                nextTokens.push_back("(" + monkey.Rhs + ")");
                nextTokens.push_back(")");
            }
        }

        size_t oldSize = tokens.size();
        tokens = nextTokens;
        nextTokens.clear();
        if (oldSize == tokens.size()) {
            break;
        }
    }

    std::string equation = StrUtil::JoinVec("", tokens);

    std::cout << equation << "\n\n";
    size_t oldSize = equation.size();
    Reduce(equation);
    
    while (oldSize != equation.size()) {
        std::cout << equation << "\n\n";
        oldSize = equation.size();
        Reduce(equation);
    }

    return equation;
    */
    
    

    /*
    RunMonkeys(monkeys, monkeyMap, rootIndex);
    u64 i = 8759963400000;
    while (true) {
        if (i % 100000 == 0) {
            std::cout << i << '\n';
        }
        monkeys[humanIndex].Value = i;
        auto copy = monkeys;
        RunMonkeys(copy, monkeyMap, rootIndex);
        if (copy[leftMonkeyIndex].Value == copy[rightMonkeyIndex].Value) {
            return i;
        }

        i--;
    }
    */
    
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    //return PartTwo(lines);
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "root: pppw + sjmn",
        "dbpl: 5",
        "cczh: sllz + lgvd",
        "zczc: 2",
        "ptdq: humn - dvpt",
        "dvpt: 3",
        "lfqf: 4",
        "humn: 5",
        "ljgn: 2",
        "sjmn: drzm * dbpl",
        "sllz: 4",
        "pppw: cczh / lfqf",
        "lgvd: ljgn * ptdq",
        "drzm: hmdt - zczc",
        "hmdt: 32"
    };

    if(ParseLine("abcd: 42").Value != u64(42)) return false;
    if (PartOne(lines) != 152) return false;
    
    //auto equation = PartTwo(lines);
    //if (equation.empty()) return false;
    //if (PartTwo(lines) != 301) return false;

    return true;
}