#include "Utilities/ExpressionEvaluator.h"

#include <regex>
namespace {
    constexpr char TrueVal[] = "true";
    constexpr char FalseVal[] = "false";
    static const std::regex spaceRegex(" +");

    std::string BoolToStr(bool val) {
        if(val) {
            return TrueVal;
        }
        return FalseVal;
    }

    bool ContainsInnerExpression(const std::string& line) {
        return line.find('(') != line.npos;
    }

    size_t ExtractInnerExpression(const std::string& line, std::string& outString) {
        auto closeParen = line.find_first_of(')');
        std::string substr = line.substr(0, closeParen);
        auto openParen = substr.find_last_of('(');

        outString = line.substr(openParen + 1, closeParen - openParen - 1);
        return openParen;
    }

    bool EvaluateValue(const std::string& value, const std::function<bool(std::string)>& predicate) {
        if(value == TrueVal || value == FalseVal) {
            return value == TrueVal;
        }
        if(value[0] == '!') {
            return !predicate(value.substr(1));
        }
        return predicate(value);
    }

    bool EvaluateExpression(const std::string& line, const size_t end, const std::function<bool(std::string)>& predicate) {
        auto symbolIndex = line.find_first_of("&|");
        std::string lhs = line.substr(0, symbolIndex);
        std::string rhs = line.substr(symbolIndex + 2, end - symbolIndex + 1);

        switch(line[symbolIndex]) {
        case '&': return EvaluateValue(lhs, predicate) && EvaluateValue(rhs, predicate);
        case '|': return EvaluateValue(lhs, predicate) || EvaluateValue(rhs, predicate);
        default: return false;
        }
    }

    void ReplaceInnerExpression(std::string& line, const std::function<bool(std::string)>& predicate) {
        auto symbolIndex = line.find_first_of("&|");
        if(symbolIndex == line.npos) {
            line = BoolToStr(EvaluateValue(line, predicate));
            return;
        }

        while(true) {
            symbolIndex = line.find_first_of("&|");
            auto nextSymbolIndex = line.find_first_of("&|", symbolIndex + 2);
            if(nextSymbolIndex == line.npos) {
                line = BoolToStr(EvaluateExpression(line, line.length(), predicate));
                return;
            } else {
                line = BoolToStr(EvaluateExpression(line, nextSymbolIndex, predicate)) + line.substr(nextSymbolIndex);
            }
        }
    }

    void Evaluate_Impl(std::string& line, const std::function<bool(std::string)>& predicate) {
        while(ContainsInnerExpression(line)) {
            std::string innerExpression;
            auto startIndex = ExtractInnerExpression(line, innerExpression);
            auto result = innerExpression;
            ReplaceInnerExpression(result, predicate);
            line.replace(startIndex, startIndex + innerExpression.length() + 2, result);
        }
        ReplaceInnerExpression(line, predicate);
    }

} // namespace

namespace ExpressionEvaluator {
    bool Evaluate(const std::string& line, const std::function<bool(std::string)>& predicate) {
        std::string copy = std::regex_replace(line, spaceRegex, ""); // remove all the spaces

        Evaluate_Impl(copy, predicate);
        return EvaluateValue(copy, predicate);
    }
} // namespace ExpressionEvaluator