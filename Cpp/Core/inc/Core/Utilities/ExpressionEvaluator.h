#pragma once

#include <functional>
#include <string>

namespace ExpressionEvaluator {
    bool Evaluate(const std::string& line, const std::function<bool(std::string)>& predicate);
}