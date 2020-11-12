#ifndef __EXTRACTORSETTINGS_H__
#define __EXTRACTORSETTINGS_H__

namespace Extractor {
    struct ExtractorSettings {
        bool CountIncludes{true};
        bool ExtractTypes{false};
        bool ExtractFunctions{false};
    };
} // namespace Extractor
#endif // __EXTRACTORSETTINGS_H__