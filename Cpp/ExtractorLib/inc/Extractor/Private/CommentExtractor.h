#pragma once

#include <string>

namespace Extractor {
    namespace CommentExtractor {
        void StripComments(std::string& line, bool& isInBlockComment);
    }
} // namespace Extractor
