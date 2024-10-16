#pragma once

#include "Extractor/Data/Visibility.h"

#include <string>
namespace Extractor {
    namespace VisibilityExtractor {
        bool HasVisibility(const std::string& line);
        Visibility ExtractVisibility(std::string& line);
    } // namespace VisibilityExtractor
} // namespace Extractor
