#pragma once
#include "Platform/Types.h"
#include "Utilities/StringUtils.h"

#include <unordered_map>
#include <charconv>
#include <memory>
#include <regex>

struct Operation;
using OpMap = std::unordered_map<std::string, std::shared_ptr<Operation>>;

bool IsNumber(std::string val) {
    static auto IsNumberRegex = std::regex("\\d+");
    return std::regex_match(val, IsNumberRegex);
}

u16 ParseNumber(std::string_view number) {
    u16 result;
    std::from_chars(number.data(), number.data() + number.size(), result);
    return result;
}

struct Operation {
    virtual ~Operation() = default;
    virtual u16 Eval(const OpMap& OperationMap) = 0;
    std::string Lhs;
    std::string Rhs;
    u16 Value = 0;
    bool ValueSet = false;
};

struct Op_Constant : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            if(IsNumber(Lhs)) {
                Value = ParseNumber(Lhs);
            } else {
                Value = OperationMap.at(Lhs)->Eval(OperationMap);
            }
            ValueSet = true;
        }
        return Value;
    }
};

struct Op_AND : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            Value = (OperationMap.at(Lhs)->Eval(OperationMap)) & (OperationMap.at(Rhs)->Eval(OperationMap));
            ValueSet = true;
        }
        return Value;
    }
};

struct Op_OR : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            Value = (OperationMap.at(Lhs)->Eval(OperationMap)) | (OperationMap.at(Rhs)->Eval(OperationMap));
            ValueSet = true;
        }
        return Value;
    }
};

struct Op_NOT : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            Value = ~(OperationMap.at(Lhs)->Eval(OperationMap));
            ValueSet = true;
        }
        return Value;
    }
};

struct Op_LShift : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            Value = (OperationMap.at(Lhs)->Eval(OperationMap)) << (OperationMap.at(Rhs)->Eval(OperationMap));
            ValueSet = true;
        }
        return Value;
    }
};

struct Op_RShift : Operation {
    u16 Eval(const OpMap& OperationMap) override {
        if(!ValueSet) {
            Value = (OperationMap.at(Lhs)->Eval(OperationMap)) >> (OperationMap.at(Rhs)->Eval(OperationMap));
            ValueSet = true;
        }
        return Value;
    }
};

class Circuit {
    std::unordered_map<std::string, std::shared_ptr<Operation>> Operations{};

    std::shared_ptr<Operation> ParseOperation(std::string_view op) {
        static auto IsNumber = std::regex("\\d+");
        auto asStr = std::string(op);

        if(op == "AND" ) {
            return std::make_shared<Op_AND>();
        } else if (op == "OR") {
            return std::make_shared<Op_OR>();
        } else if(op == "NOT") {
            return std::make_shared<Op_NOT>();
        } else if(op == "LSHIFT") {
            return std::make_shared<Op_LShift>();
        } else if(op == "RSHIFT") {
            return std::make_shared<Op_RShift>();
        } else {
            auto result = std::make_shared<Op_Constant>();
            result->Lhs = op;
            return result;
        }
    }

public:
    void AddLine(const std::string& line) {
        auto firstSplit = StrUtil::Split(line, "->");
        auto opStr = StrUtil::Trim(std::string(firstSplit[0]));
        auto wire = StrUtil::Trim(std::string(firstSplit[1]));

        auto secondSplit = StrUtil::Split(opStr, " ");

        if(secondSplit.size() == 1) {
            auto value = std::string(secondSplit[0]);
            auto op = ParseOperation(value);
            if(IsNumber(value)) {
                Operations.emplace(value, op);
            }

            Operations.emplace(wire, op);
        } else if(secondSplit.size() == 2) {
            auto op = ParseOperation(secondSplit[0]);
            auto otherWire = std::string(secondSplit[1]);

            if(IsNumber(otherWire)) {
                auto otherOp = ParseOperation(otherWire);
                Operations.emplace(otherWire, op);
            }

            op->Lhs = otherWire;
            Operations.emplace(wire, op);
        } else if(secondSplit.size() == 3) {
            auto op = ParseOperation(secondSplit[1]);
            if(op == nullptr) {
                throw std::logic_error("Bad 3 arg");
            }

            auto lhsStr = std::string(secondSplit[0]);
            auto rhsStr = std::string(secondSplit[2]);

            op->Lhs = lhsStr;
            op->Rhs = rhsStr;

            if(IsNumber(lhsStr)) {
                Operations.emplace(lhsStr, ParseOperation(lhsStr));
            }
            if(IsNumber(rhsStr)) {
                Operations.emplace(rhsStr, ParseOperation(rhsStr));
            }

            Operations.emplace(wire, op);
        } else {
            throw std::logic_error("Bad arg count");
        }
    }

    u16 GetSignal(const std::string& wireName) {
        if(Operations.find(wireName) == Operations.end()) {
            return 0;
        }
        return Operations[wireName]->Eval(Operations);
    }

    #define ASSERT_SIGNAL(wire, value) if(GetSignal(#wire) != value) return false;

    bool RunProvidedTests() {
        Operations.clear();

        AddLine("123 -> x");
        AddLine("456 -> y");
        AddLine("x AND y -> d");
        AddLine("x OR y -> e");
        AddLine("x LSHIFT 2 -> f");
        AddLine("y RSHIFT 2 -> g");
        AddLine("NOT x -> h");
        AddLine("NOT y -> i");

        ASSERT_SIGNAL(d, 72);
        ASSERT_SIGNAL(e, 507);
        ASSERT_SIGNAL(f, 492);
        ASSERT_SIGNAL(g, 114);
        ASSERT_SIGNAL(h, 65412);
        ASSERT_SIGNAL(i, 65079);
        ASSERT_SIGNAL(x, 123);
        ASSERT_SIGNAL(y, 456);
        return true;
    }

    bool RunConstantTests() {
        Operations.clear();

        AddLine("a -> b");
        AddLine("b -> c");
        AddLine("c -> d");
        AddLine("42 -> a");

        ASSERT_SIGNAL(42, 42);
        ASSERT_SIGNAL(a, 42);
        ASSERT_SIGNAL(b, 42);
        ASSERT_SIGNAL(c, 42);
        ASSERT_SIGNAL(d, 42);

        return true;
    }
    bool RunTests() {
        return RunProvidedTests() && RunConstantTests();
    }

    #undef ASSERT_SIGNAL
};