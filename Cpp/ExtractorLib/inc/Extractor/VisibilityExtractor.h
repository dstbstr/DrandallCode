#ifndef __VISIBILITYEXTRACTOR_H__
#define __VISIBILITYEXTRACTOR_H__

#include "Extractor/Data/Visibility.h"

#include <string>
namespace Extractor {
    namespace VisibilityExtractor {
        bool HasVisibility(const std::string& line);
        Visibility ExtractVisibility(std::string& line);
    } // namespace VisibilityExtractor
} // namespace Extractor
#endif // __VISIBILITYEXTRACTOR_H__