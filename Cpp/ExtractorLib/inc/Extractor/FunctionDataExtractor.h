#pragma once

#include "Extractor/Data/FunctionData.h"
#include "Extractor/Data/OperatorOverloadData.h"
#include "Extractor/Data/SpecialFunctionData.h"

#include <iostream>
#include <regex>
#include <string>

namespace Extractor {
    namespace FunctionDataExtractor {
        bool IsAFunction(const std::string& line, std::smatch& outMatch);
        bool IsSpecialFunction(const std::string& line, std::smatch& outMatch);
        bool IsOperatorOverload(const std::string& line, std::smatch& outMatch);

        FunctionData ExtractFunction(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, const std::string& className, Visibility visibility);
        SpecialFunctionData ExtractSpecialFunction(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, Visibility visibility);
        OperatorOverloadData ExtractOperatorOverload(const std::string& line, const std::smatch& match, std::istream& stream, const std::string& ns, const std::string& className, Visibility visibility);
    } // namespace FunctionDataExtractor
} // namespace Extractor
