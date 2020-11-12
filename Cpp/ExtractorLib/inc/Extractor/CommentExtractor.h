#ifndef __COMMENTEXTRACTOR_H__
#define __COMMENTEXTRACTOR_H__

#include <string>

namespace Extractor {
    namespace CommentExtractor {
        void StripComments(std::string& line, bool& isInBlockComment);
    }
} // namespace Extractor
#endif // __COMMENTEXTRACTOR_H__