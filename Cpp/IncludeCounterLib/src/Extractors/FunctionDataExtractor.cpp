#include "IncludeCounter/Extractors/FunctionDataExtractor.h"

namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsLineAFunction(const std::string& line) {
                line;
                return false;
            }

            FunctionData Extract(std::istream& stream) {
                FunctionData result;
                stream;
                return result;
            }

        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter