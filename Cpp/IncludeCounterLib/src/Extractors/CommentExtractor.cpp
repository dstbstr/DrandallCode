#include "IncludeCounter/Extractors/CommentExtractor.h"

namespace {
    bool RemoveFirstBlockComment(std::string& line, bool& isInBlockComment) {
        std::string original = line;
        auto blockCommentStart = line.find("/*");
        auto blockCommentEnd = line.find("*/", blockCommentStart + 2);
        line = original.substr(0, blockCommentStart);
        if(blockCommentEnd != std::string::npos) {
            line += original.substr(blockCommentEnd + 2);
            return true;
        } else {
            isInBlockComment = true;
            return false;
        }
    }
} // namespace

namespace IncludeCounter {
    namespace Extractors {
        namespace CommentExtractor {

            // Removes the majority of reasonable comments.  Does not handle escaped newlines, trigraphs, or quoted comments... you monsters
            void StripComments(std::string& line, bool& isInBlockComment) {
                if(isInBlockComment) {
                    auto commentEnd = line.find("*/");
                    if(commentEnd != std::string::npos) {
                        line = line.substr(commentEnd + 2);
                        isInBlockComment = false;
                    } else {
                        line = "";
                        return;
                    }
                }
                auto lineCommentStart = line.find("//");
                auto blockCommentStart = line.find("/*");

                if(lineCommentStart == std::string::npos && blockCommentStart == std::string::npos) {
                    return;
                }

                if(lineCommentStart != std::string::npos && (blockCommentStart == std::string::npos || lineCommentStart < blockCommentStart)) {
                    line = line.substr(0, lineCommentStart);
                    return;
                }

                if(RemoveFirstBlockComment(line, isInBlockComment)) {
                    StripComments(line, isInBlockComment);
                }
            }
        } // namespace CommentExtractor
    } // namespace Extractors
} // namespace IncludeCounter
