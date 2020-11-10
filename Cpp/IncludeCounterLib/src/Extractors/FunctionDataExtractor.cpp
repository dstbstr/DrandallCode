#include "IncludeCounter/Extractors/FunctionDataExtractor.h"

namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsLineAFunction(const std::string& line) {
                line;
                return false;
            }

            FunctionData Extract(std::istream& stream, Visibility visibility) {
                FunctionData result;
                result.Visibility = visibility;
                stream;
                return result;
            }

        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter