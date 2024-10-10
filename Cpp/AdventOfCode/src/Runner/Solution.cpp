#include "Runner/Solution.h"

std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, SolutionFunc>>>& GetSolutions() {
    static std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, SolutionFunc>>> Solutions{};
    return Solutions;
}
std::unordered_map<size_t, std::unordered_map<size_t, std::function<bool()>>>& GetTests() {
    static std::unordered_map<size_t, std::unordered_map<size_t, std::function<bool()>>> Tests{};
    return Tests;
}
