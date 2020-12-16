#ifndef __EXPRESSIONEVALUATOR_H__
#define __EXPRESSIONEVALUATOR_H__

#include <functional>
#include <string>

namespace ExpressionEvaluator {
    bool Evaluate(const std::string& line, std::function<bool(std::string)> predicate);
}
#endif // __EXPRESSIONEVALUATOR_H__