#ifndef __PREPROCESSORRESULT_H__
#define __PREPROCESSORRESULT_H__

#include <string>
#include <unordered_map>
#include <unordered_set>

struct PreProcessorResult {
    std::string FilePath;
    std::string FileName;
    std::unordered_map<std::string, std::string> Defines;
    std::unordered_set<std::string> ConditionalDefines;
};

#endif // __PREPROCESSORRESULT_H__