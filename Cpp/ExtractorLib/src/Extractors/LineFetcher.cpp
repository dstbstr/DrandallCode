#include "Extractor/Private/LineFetcher.h"
#include "Extractor/CommentExtractor.h"
#include "Utilities/StringUtilities.h"

namespace {
    bool AppendNextNonBlankLine(std::istream& stream, std::string& outLine) {
        std::string nextLine;
        bool isInBlockComment = false;
        do {
            if(!std::getline(stream, nextLine)) {
                return false;
            }
            Extractor::CommentExtractor::StripComments(nextLine, isInBlockComment);
            nextLine = StrUtil::Trim(nextLine);
            outLine += nextLine;
        } while(isInBlockComment || nextLine.empty());

        return true;
    }

    bool IsLineComplete(const std::string& line) {
        auto lastChar = line[line.length() - 1];
        return line[0] == '#' ||
            lastChar == ';' ||
            lastChar == '}' ||
            lastChar == '{';
    }
}

namespace Extractor {
    namespace LineFetcher {
        bool JoinLines(std::string& line, std::istream& stream) {
            if(line.empty()) {
                if(!AppendNextNonBlankLine(stream, line)) {
                    return false;
                }
            }

            while(!IsLineComplete(line)) {
                if(!AppendNextNonBlankLine(stream, line)) {
                    return false;
                }
            }
            return true;
        }

        bool GetNextLine(std::istream& stream, std::string& outString) {
            outString = ""; //clear string before starting
            if(!AppendNextNonBlankLine(stream, outString)) {
                return false;
            }
            while(!IsLineComplete(outString)) {
                outString += " ";
                if(!AppendNextNonBlankLine(stream, outString)) {
                    return false;
                }
            }

            return true;
        }
    }
}