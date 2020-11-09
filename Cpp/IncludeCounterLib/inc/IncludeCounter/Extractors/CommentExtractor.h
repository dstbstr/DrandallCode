#ifndef __COMMENTEXTRACTOR_H__
#define __COMMENTEXTRACTOR_H__

#include <string>

namespace IncludeCounter {
    namespace Extractors {
        namespace CommentExtractor {
            void StripComments(std::string& line, bool& isInBlockComment);
        }
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __COMMENTEXTRACTOR_H__