#pragma once

#include "Core/Constexpr/ConstexprCollections.h"
#include "Core/Constexpr/ConstexprStrUtils.h"

#include <functional>
#include <string>
#include <string_view>

namespace ShuntingYard {
    enum struct Associativity { Left, Right };

    template<typename T>
    struct OperatorData {
        size_t Precedence = 0;
        Associativity Associativity = Associativity::Left;
        std::function<T(const std::string&, const std::string&)> Run;
    };

    namespace Detail {
        template<typename T>
        Constexpr::SmallMap<std::string, OperatorData<T>> DefaultOperatorData{
            { "+", {2, Associativity::Left, [](const std::string& lhs, const std::string& rhs) {
                T a, b;
                Constexpr::ParseNumber(lhs, a);
                Constexpr::ParseNumber(rhs, b);
                return a + b;
            }} },
            {"-", {2, Associativity::Left, [](const std::string& lhs, const std::string& rhs) {
                T a, b;
                Constexpr::ParseNumber(lhs, a);
                Constexpr::ParseNumber(rhs, b);
                return a - b;
            }}},
            {"*", {3, Associativity::Left, [](const std::string& lhs, const std::string& rhs) {
                T a, b;
                Constexpr::ParseNumber(lhs, a);
                Constexpr::ParseNumber(rhs, b);
                return a * b;
            }}},
            { "/", {3, Associativity::Left, [](const std::string& lhs, const std::string& rhs) {
                T a, b;
                Constexpr::ParseNumber(lhs, a);
                Constexpr::ParseNumber(rhs, b);
                return a / b;
            }}}
        };

        constexpr bool IsNumber(char c) {
            return c >= '0' && c <= '9';
        }

        template<typename T>
        constexpr void ProcessToken(const std::string& token, Constexpr::Queue<std::string>& outputQueue, Constexpr::Stack<std::string>& operatorStack, const Constexpr::SmallMap<std::string, OperatorData<T>>& operators) {
            auto firstOpenParen = token.find("(");
            auto firstCloseParen = token.find(")");
            auto firstNum = token.find_first_of("0123456789");
            auto firstSymbol = token.find_first_not_of("()0123456789");

            std::vector<size_t> positions = { firstOpenParen, firstCloseParen, firstNum, firstSymbol };
            positions.erase(std::remove_if(positions.begin(), positions.end(), [token](size_t pos) { return pos == token.npos; }), positions.end());

            if (positions.size() == 1) {
                if (firstOpenParen != token.npos) {
                    for (auto i = 0; i < token.size(); i++) {
                        operatorStack.push("(");
                    }
                }
                else if (firstNum != token.npos) outputQueue.push(token);
                else if (firstCloseParen != token.npos) {
                    for (auto i = 0; i < token.size(); i++) {
                        while (true) {
                            auto next = operatorStack.pop();
                            if (next == "(") break;
                            outputQueue.push(next);
                        }
                        /*
                        while (operatorStack.top() != "(") {
                            if (operatorStack.is_empty()) throw "Mismatched parens";
                            outputQueue.push(operatorStack.top());
                            operatorStack.pop();
                        }
                        operatorStack.pop();
                        */
                    }
                }
                else if (firstSymbol != token.npos) {
                    auto o1 = operators.at(token);
                    auto shouldPop = [&]() {
                        if (operatorStack.is_empty()) return false;
                        auto next = operatorStack.top();
                        if (next == "(") return false;
                        auto o2 = operators.at(std::string(next));
                        if (o1.Precedence != o2.Precedence) return o2.Precedence > o1.Precedence;
                        return o1.Associativity == Associativity::Left;
                    };

                    while (shouldPop()) {
                        outputQueue.push(operatorStack.pop());
                    }
                    operatorStack.push(token);
                }
            }
            else {
                std::sort(positions.begin(), positions.end());
                ProcessToken(token.substr(0, positions[1]), outputQueue, operatorStack, operators); //peel off just the first argument
                ProcessToken(token.substr(positions[1]), outputQueue, operatorStack, operators);
            }
        }
    }

    template<typename T>
    constexpr T Evaluate(const std::string& input, const Constexpr::SmallMap<std::string, OperatorData<T>>& operators) {
        auto tokens = Constexpr::Split(input, " ");
        Constexpr::Queue<std::string> outputQueue;
        Constexpr::Stack<std::string> operatorStack;

        for (const auto& token : tokens) {
            Detail::ProcessToken(std::string(token), outputQueue, operatorStack, operators);
        }
        while (!operatorStack.is_empty()) {
            outputQueue.push(operatorStack.top());
            operatorStack.pop();
        }

        //evaluate
        Constexpr::Stack<std::string> evaluateStack;
        while (!outputQueue.is_empty()) {
            auto token = outputQueue.front();
            outputQueue.pop();

            if (Detail::IsNumber(token[0])) {
                evaluateStack.push(token);
            }
            else {
                auto op = operators.at(token);
                auto lhs = evaluateStack.top();
                evaluateStack.pop();
                auto rhs = evaluateStack.top();
                evaluateStack.pop();

                evaluateStack.push(Constexpr::ToString(op.Run(lhs, rhs)));
            }
        }

        T result;
        Constexpr::ParseNumber(evaluateStack.top(), result);
        return result;
    }

    template<typename T>
    constexpr T Evaluate(const std::string& input) {
        return Evaluate(input, Detail::DefaultOperatorData<T>);
    }

    namespace Tests {
        bool Run();
    }
}