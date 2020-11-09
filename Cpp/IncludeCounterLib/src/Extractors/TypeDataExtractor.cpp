#include "IncludeCounter/Extractors/TypeDataExtractor.h"

namespace IncludeCounter {
    namespace Extractors {
        namespace TypeDataExtractor {
            bool IsAType(const std::string& line) {
                line;
                return false;
            }
            TypeData Extract(std::istream& stream) {
                TypeData result;
                stream;
                return result;
            }

        } // namespace TypeDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter