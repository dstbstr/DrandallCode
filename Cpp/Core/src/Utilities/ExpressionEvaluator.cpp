#include "Utilities/ExpressionEvaluator.h"

namespace {
    constexpr char TrueVal[] = "true";
    constexpr char FalseVal[] = "false";

    std::string BoolToStr(bool val) {
        if(val) {
            return TrueVal;
        }
        return FalseVal;
    }

    bool ContainsInnerExpression(const std::string& line) {
        return line.find('(') != line.npos;
    }

    std::string ExtractInnerExpression(const std::string& line) {
        auto closeParen = line.find_first_of(')');
        std::string substr = line.substr(0, closeParen);
        auto openParen = substr.find_last_of('(');

        return line.substr(openParen + 1, closeParen - openParen);
    }

    bool EvaluateValue(const std::string& value, std::function<bool(std::string)> predicate) {
        if(value == TrueVal || value == FalseVal) {
            return value == TrueVal;
        }
        if(value[0] == '!') {
            return !predicate(value.substr(1));
        }
        return predicate(value);
    }

    void ReplaceInnerExpression(std::string& line, std::function<bool(std::string)> predicate) {
        auto symbolIndex = line.find_first_of("&|");
        if(symbolIndex == line.npos) {
            line = BoolToStr(EvaluateValue(line, predicate));
            return;
        }

        while(true) {
            symbolIndex = line.find_first_of("&|");
            auto nextSymbolIndex = line.find_first_of("&|", symbolIndex + 2);
            if(nextSymbolIndex == line.npos) {
                line = BoolToStr(EvaluateValue(line, predicate));
                return;
            }

            std::string lhs = line.substr(0, symbolIndex);
            std::string rhs = line.substr(symbolIndex + 1, nextSymbolIndex - symbolIndex + 1);
            bool result = false;
            switch(line[symbolIndex]) {
            case '&': result = EvaluateValue(lhs, predicate) && EvaluateValue(rhs, predicate); break;
            case '|': result = EvaluateValue(lhs, predicate) || EvaluateValue(lhs, predicate); break;
            }

            line = BoolToStr(result) + line.substr(nextSymbolIndex);
        }
    }

    void EvaluateExpression(std::string& line, std::function<bool(std::string)> predicate) {
        while(ContainsInnerExpression(line)) {
            auto innerExpression = ExtractInnerExpression(line);
            auto result = innerExpression;
            ReplaceInnerExpression(result, predicate);
            line.replace(line.begin)
        }
    }

} // namespace

namespace ExpressionEvaluator {
    bool Evaluate(const std::string& line, std::function<bool(std::string)> predicate) {
        bool result = false;
        std::string copy = line;
        while(ContainsInnerExpression(copy)) {
            ReplaceInnerExpression(copy, predicate);
        }

        return EvaluateValue(copy, predicate);
    }
} // namespace ExpressionEvaluator