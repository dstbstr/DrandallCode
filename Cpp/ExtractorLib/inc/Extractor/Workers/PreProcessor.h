#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__

#include "Extractor/Data/PreProcessorResult.h"

#include <iostream>
#include <unordered_map>

namespace Extractor {
    namespace PreProcessor {
        //go through all of the files once, find any #define statements.  Any #defines which are conditionally compiled will need to be re-evaluated on the second pass for each file
        void FirstPass(std::istream& stream, std::unordered_map<std::string, PreProcessorResult>& result);
        //Reprocesses a file, and adds any of the conditional defines
        void Reprocess(std::istream& stream, std::unordered_map<std::string, PreProcessorResult>& result);
    }
} // namespace Extractor
#endif // __PREPROCESSOR_H__