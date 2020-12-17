#include "Extractor/FilePreProcessor.h"

#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Utilities/PathUtils.h"
#include "Utilities/StringUtils.h"

#include <fstream>
#include <regex>

constexpr char DefinePrefix[] = "#define ";
static const std::regex DefinitionRegex("^#define (\\w+(?:\\([^\\)]*\\))?) ?(.+)?");

namespace Extractor {
    PreProcessorResult FilePreProcessor::Execute() const {
        PreProcessorResult result;
        result.FileName = PathUtils::GetFileName(m_FilePath);
        std::string includeGuardName = result.FileName.substr(0, result.FileName.find_first_of('.'));
        includeGuardName = StrUtil::ToUpper(includeGuardName);

        std::ifstream stream(m_FilePath, std::ifstream::in);
        if(!stream.is_open()) {
            return result;
        }
        static std::vector<std::string> emptyDefineList = {};

        IfDefExtractor ifdefExtractor(emptyDefineList, &stream);
        bool inIfDef = false;
        u8 ifDefDepth = 0;

        std::string line;
        std::smatch match;

        while(LineFetcher::GetNextLine(stream, line)) {
            if(ifdefExtractor.CanExtract(line)) {
                if(line.find(includeGuardName) != line.npos) {
                    // include guard
                } else if(line == "#endif") {
                    ifDefDepth--;
                    // inIfDef = false;
                } else if(StrUtil::StartsWith(line, "#if")) {
                    // inIfDef = true;
                    ifDefDepth++;
                }
                continue;
            }

            if(StrUtil::StartsWith(line, DefinePrefix)) {
                if(ifDefDepth > 0) {
                    result.HasConditionalDefines = true;
                } else {
                    std::regex_search(line, match, DefinitionRegex);
                    result.Defines[match[1]] = match[2];
                }
            }
        }

        return result;
    }
} // namespace Extractor