#ifndef __VISIBILITYEXTRACTOR_H__
#define __VISIBILITYEXTRACTOR_H__

#include "IncludeCounter/Data/Visibility.h"

#include <string>
namespace IncludeCounter {
    namespace Extractors {
        namespace VisibilityExtractor {
            bool HasVisibility(const std::string& line);
            Visibility ExtractVisibility(std::string& line);
        } // namespace VisibilityExtractor
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __VISIBILITYEXTRACTOR_H__